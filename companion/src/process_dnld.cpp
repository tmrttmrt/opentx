/*
 * Copyright (C) OpenTX
 *
 * Based on code named
 *   th9x - http://code.google.com/p/th9x
 *   er9x - http://code.google.com/p/er9x
 *   gruvin9x - http://code.google.com/p/gruvin9x
 *
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "process_dnld.h"
#include "progresswidget.h"
#include "eeprominterface.h"
#include <QFile>
#include <QMessageBox>
#include <QProcess>
#include <QTimer>
#include <QEventLoop>

#if defined _MSC_VER || !defined __GNUC__
#include <Windows.h>
#include <WinBase.h>
#include <tlhelp32.h>
#define sleep(x) Sleep(x*1000)
#else
#include <unistd.h>
#endif

DnldProcess::DnldProcess(const QString &cmd, const QStringList &args, ProgressWidget *progress, enum DnldPhase dnldPhase):
progress(progress),
cmd(cmd),
args(args),
process(new QProcess(this)),
hasErrors(false),
dnldPhase(dnldPhase),
itemsProcessed(0)
#if !__GNUC__
, killTimer(NULL)
#endif
{
  connect(process, SIGNAL(started()),this, SLOT(onStarted()));
  connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(onReadyReadStandardOutput()));
  connect(process, SIGNAL(readyReadStandardError()), this, SLOT(onReadyReadStandardError()));
  connect(process, SIGNAL(finished(int)), this, SLOT(onFinished(int)));
}

DnldProcess::~DnldProcess()
{
#if !__GNUC__
  delete killTimer;
#endif
}

bool DnldProcess::run()
{
  if (!QFile::exists(cmd)) {
    QMessageBox::critical(NULL, CPN_STR_APP_NAME, tr("Executable %1 not found").arg(cmd));
    return false;
  }

#if !__GNUC__
  if (cmd.toLower().contains("sam-ba")) {
    killTimer = new QTimer(this);
    connect(killTimer, SIGNAL(timeout()), this, SLOT(onKillTimerElapsed()));
    killTimer->start(2000);
  }
#endif

  QEventLoop loop;
  connect(this, SIGNAL(finished()), &loop, SLOT(quit()));
  process->start(cmd, args);
  loop.exec();

  if (hasErrors) {
    return false;
  }
  return true;
}

void DnldProcess::onStarted()
{
  progress->lock(true);
  progress->addText(cmd + " " + args.join(" "));
  progress->addSeparator();
  progress->setMaximum(31);
}

#if !__GNUC__
bool killProcessByName(const char *szProcessToKill)
{
  HANDLE hProcessSnap;
  HANDLE hProcess;
  PROCESSENTRY32 pe32;

  hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);  // Takes a snapshot of all the processes

  if (hProcessSnap == INVALID_HANDLE_VALUE) {
    return false;
  }

  pe32.dwSize = sizeof(PROCESSENTRY32);

  if (!Process32First(hProcessSnap, &pe32)) {
    CloseHandle(hProcessSnap);
    return false;
  }

  do {
    if (!strcmp(pe32.szExeFile,szProcessToKill)) {    //  checks if process at current position has the name of to be killed app
      hProcess = OpenProcess(PROCESS_TERMINATE,0, pe32.th32ProcessID);  // gets handle to process
      TerminateProcess(hProcess, 0);   // Terminate process by handle
      CloseHandle(hProcess);  // close the handle
    }
  } while (Process32Next(hProcessSnap, &pe32));  // gets next member of snapshot

  CloseHandle(hProcessSnap);  // closes the snapshot handle
  return true;
}
#endif

void DnldProcess::onKillTimerElapsed()
{
#if !__GNUC__
  // trick to accelerate SAM-BA startup
  killProcessByName("tasklist.exe");
#endif
}

void DnldProcess::analyseStandardOutput(const QString &text)
{
  currStdoutLine.append(text);
  qDebug() << text;
}

void DnldProcess::analyseStandardError(const QString &text)
{
  currStderrLine.append(text);
  while(currStderrLine.contains("\n")){
    int nlPos = currStderrLine.indexOf("\n");
    QString line = currStderrLine.mid(0, nlPos);
    qDebug() << line;
    currStderrLine = currStderrLine.mid(nlPos+1);
    if (line.at(0) == QChar('[')) {
        QStringList list = line.split(QRegExp("[\\[/]"), QString::SkipEmptyParts);
        int pos = list.at(0).toInt();
        progress->setValue(pos + 1);
    } 
    else if (line.at(0) == QChar('c')) {
      QRegularExpression re("curl:\\s+\\((\\d+)\\)");
      QRegularExpressionMatch match = re.match(line);
      if (match.hasMatch()) {
        QString matched = match.captured(1);
        int errc = matched.toInt();
        qDebug() << "errno:" << QString::number(errc);
        if(78 != errc) {
          hasErrors = true;
          process->kill();
        }
      }
      else {
        re.setPattern("curl:.*model-\\d+\\.bin");
        match = re.match(line);
        if (match.hasMatch()) {
            itemsProcessed++;
            progress->setValue(itemsProcessed);
        }
      }
    } 
    else if (WRITING == dnldPhase && line.at(0) == QChar('\r')) {
      QRegularExpression re("\\s*#+\\s*100\\.");
      QRegularExpressionMatch match = re.match(line);
      if (match.hasMatch()) {
        itemsProcessed++;
        progress->setValue(itemsProcessed);
      }
    }
  } 
}

void DnldProcess::onReadyReadStandardOutput()
{
  QString text = QString(process->readAllStandardOutput());
  progress->addText(text);
  analyseStandardOutput(text);
}

void DnldProcess::onReadyReadStandardError()
{
  QString text = QString(process->readAllStandardError());
  progress->addText(text);
  analyseStandardError(text);
}

void DnldProcess::errorWizard()
{
  QString output = progress->getText();

  QMessageBox::warning(NULL, "Companion - Tip of the day", tr("ESP32 test."));
}

void DnldProcess::onFinished(int code=0)
{
  progress->addSeparator();
  qDebug() << "ret:" << QString::number(code);
  switch(code){
    case 78:
    case 26:
      if (hasErrors) {
        progress->setInfo(tr("Failed (exit code = %1)").arg(code));
        break;
      }
      progress->setInfo(tr("Done (exit code = %1)").arg(code));
      break;
    default:
      progress->setInfo(tr("Failed (exit code = %1)").arg(code));
  }
  progress->setValue(progress->maximum());
  progress->lock(false);
  emit finished();
}

