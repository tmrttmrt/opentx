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
#include "opentxinterface.h"
#include "binraw.h"
#include <QFile>
#include <bitset>

// TODO should be RleFormat, RawFormat etc.

bool BinRawFormat::load(RadioData & radioData)
{
  QFile file(filename);

  int size = file.size();

  if (!file.open(QFile::ReadOnly)) {
    qDebug() << "Unable to open" << filename << file.errorString();
    return false;
  }

  QByteArray fileContents = file.readAll();
  qDebug() << "File" << filename << "read, size:" << fileContents.size();
  OpenTxEepromInterface * loadInterface = loadRadioSettingsFromByteArray(radioData.generalSettings, fileContents);
  if (!loadInterface) {
    return false;
  }
  board = loadInterface->getBoard();
  
  QFileInfo finfo(filename);
  for(int modelIndex = 0; modelIndex < getCurrentFirmware()->getCapability(Models); modelIndex++){
    QFile file(finfo.absolutePath()+QString("/model-%1.ebin").arg(modelIndex));
    if(!file.exists()) continue;
    if (!file.open(QFile::ReadOnly)) {
      qDebug() << "Unable to open" << filename << file.errorString();
      return false;
    }
    QByteArray modelBuffer = file.readAll();;
    if ((int)radioData.models.size() <= modelIndex) {
      radioData.models.resize(modelIndex + 1);
    }
    if (!loadModelFromByteArray(radioData.models[modelIndex], modelBuffer)) {
      setError(tr("Error loading models"));
      return false;
    }
    strncpy(radioData.models[modelIndex].filename, qPrintable(file.fileName()), sizeof(radioData.models[modelIndex].filename));
    radioData.models[modelIndex].used = true;
  }
  return true;
}

bool BinRawFormat::write(const RadioData & radioData)
{
  QFile file(filename);
  if (!file.open(QFile::WriteOnly)) {
    qDebug() << "Unable to open" << filename << file.errorString();
    return false;
  }
  QByteArray radioSettingsData; // radio.bin
  size_t numModels = radioData.models.size();
  writeRadioSettingsToByteArray(radioData.generalSettings, radioSettingsData);
  if (radioSettingsData.size() != file.write(radioSettingsData, radioSettingsData.size())) {
    qDebug() << "Unable to write" << filename << file.errorString();
    return false;
  }
  file.close();
  QFileInfo finfo(filename);
  for (size_t m=0; m<numModels; m++) {
    const ModelData & model = radioData.models[m];
    if (model.isEmpty()) continue;
    QFile file(finfo.absolutePath()+QString("/model-%1.ebin").arg(m));
    if (!file.open(QFile::WriteOnly)) {
      qDebug() << "Unable to open" << file.fileName()  << file.errorString();
      return false;
    }

    QByteArray modelData;
    writeModelToByteArray(model, modelData);
    if (modelData.size() != file.write(modelData, modelData.size())) {
      qDebug() << "Unable to write" << file.fileName() << file.errorString();
      return false;
    }
    file.close();
  }
  return true;
}

bool BinRawFormat::extract(RadioData & radioData, const QByteArray & eeprom)
{
  std::bitset<NUM_ERRORS> errors;

  foreach(EEPROMInterface * eepromInterface, eepromInterfaces) {
    std::bitset<NUM_ERRORS> result((unsigned long long)eepromInterface->load(radioData, (uint8_t *)eeprom.data(), eeprom.size()));
    if (result.test(ALL_OK)) {
      if (errors.test(HAS_WARNINGS)) {
        // TODO ShowEepromWarnings(this, CPN_STR_TTL_WARNING, errors.to_ulong());
      }
      board = eepromInterface->getBoard();
      return true;
    }
    else {
      errors |= result;
    }
  }

  setError(tr("Invalid binary EEPROM file %1").arg(filename));
  return false;
}
