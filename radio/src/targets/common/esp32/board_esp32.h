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

#ifndef _BOARD_ESP32_H_
#define _BOARD_ESP32_H_
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task.h"
#include "esp_attr.h"
#include "esp_now.h"
#if defined(SDCARD)
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include "fs_wrappers.h"
#endif

#define RTOS_WAIT_TICKS(a) vTaskDelay(a);
#define RTOS_WAIT_MS(a) vTaskDelay((a)/portTICK_PERIOD_MS);
#define RTOS_TASK_HANDLE TaskHandle_t
#define RTOS_MUTEX_HANDLE SemaphoreHandle_t
#define RTOS_DEFINE_STACK(stack, size) 
#define TASK_FUNCTION(f) void f(void * pdata)
#define RTOS_LOCK_MUTEX(m) xSemaphoreTake(m, portMAX_DELAY);
#define RTOS_UNLOCK_MUTEX(m) xSemaphoreGive(m)
#define RTOS_MS_PER_TICK portTICK_PERIOD_MS
#define TASK_RETURN()
#define RTOS_INIT() rtosInit()
#define RTOS_START()
#define RTOS_CREATE_TASK(taskId, task, taskName, taskStack, stackSize, taskPrio) \
  xTaskCreatePinnedToCore( task, taskName , stackSize, NULL, taskPrio.prio, &taskId, taskPrio.core );\
  configASSERT( taskId );
#define RTOS_CREATE_MUTEX(mh) mh = xSemaphoreCreateMutex();
#define MENUS_STACK_SIZE       0x1D00
#define MIXER_STACK_SIZE       0xA00
#define AUDIO_STACK_SIZE       0x900
#define PER10MS_STACK_SIZE     0x500
#define ENC_STACK_SIZE         0x900
#define PPM_STACK_SIZE         0x800
#define MIXER_TASK_PRIO mixerTaskPrio
#define MENUS_TASK_PRIO menuTaskPrio
#define AUDIO_TASK_PRIO audioTaskPrio
#define MENU_TASK_CORE 0
#define MIXER_TASK_CORE 1
#define PULSES_TASK_CORE 1
#define AUDIO_TASK_CORE 0
#define PER10MS_TASK_CORE 0
#define ENC_TASK_CORE 0
#define MIXER_TIME_MS 15

typedef struct TaskPrio
{
  UBaseType_t prio;
  BaseType_t core;
} TaskPrio;

extern TaskPrio mixerTaskPrio;
extern TaskPrio menuTaskPrio;
extern TaskPrio audioTaskPrio;

#define usbPlugged() false
#define checkTrainerSettings()
#define eepromIsWriting() (false)
#define eepromWriteProcess()
#define isForcePowerOffRequested() false
#define resetForcePowerOffRequest()

void telemetryPortInit(uint32_t baudrate, uint8_t mode);
void rtosInit();

static inline uint32_t RTOS_GET_MS(void) {
  return (uint32_t)(esp_timer_get_time()/1000);
}

#define RTOS_GET_TIME(a) xTaskGetTickCount ()

void espLogI(const char * format, ...);
void espLogPut(const char * format, ...);

#define eeFlush()
#define DISPLAY_PROGRESS_BAR(x)

size_t fsLoadModelData(char *mpath, uint8_t *buff, size_t size, uint8_t &version);
size_t fsLoadModelData(uint8_t index, uint8_t *buff, size_t size);
size_t fsWriteModelData(uint8_t index, uint8_t *buff, size_t size);
bool eeLoadGeneral();
void eeDeleteModel(uint8_t index);
bool eeCopyModel(char * dpath, uint8_t src);
bool eeCopyModel(uint8_t dst, char *spath);
bool eeCopyModel(uint8_t dst, uint8_t src);
void eeSwapModels(uint8_t id1, uint8_t id2);
uint16_t eeLoadModelData(uint8_t index);
void eeLoadModelName(uint8_t id, char *name);
uint16_t eeModelSize(uint8_t index);


#if defined(SDCARD)
#define SD_IS_HC()                      (sdIsHC())
#define SD_GET_SPEED()                 (sdGetSpeed())
#define _MAX_LFN      CONFIG_FATFS_MAX_LFN
#define SD_PATH "/sdcard"
#define sdPoll10ms()
#define SD_CARD_PRESENT() sdMounted()

void sdInit(void);
uint32_t sdIsHC(void);
uint32_t sdGetSpeed(void);
uint32_t sdMounted(void);
void sdMountPoll();
void sdDone();
void checkSDVersion();
const char * eeBackupModel(uint8_t i_fileSrc);
const char * eeRestoreModel(uint8_t i_fileDst, char *model_name);
const char * eeBackupAll();
#endif

uint16_t getTmr1MHz();
void timer10msInit();
void initADC();
void initKeys();
void eepromInit();
void initAudio();
void initFS();
void initPulses();
void setSampleRate(uint32_t frequency);
void audioPlayTask(void * pdata);
void encoderTask(void * pdata);
uint8_t telemetryGetByte(uint8_t * byte);

void telemetryTransmitBuffer(uint8_t * data, uint8_t len);
uint16_t audioStackAvailable();
uint16_t per10msStackAvailable();
uint16_t mixerStackAvailable();
uint16_t menusStackAvailable();
uint16_t encStackAvailable();
bool rEncDown(uint8_t mask);
void mountSDCard();

void backlightEnable();
void backlightDisable();
bool isBacklightEnabled();

void initWiFi();
void startWiFi( char *ssid_zchar, char *passwd_zchar, char* ftppass_zchar);
void stopWiFi();
const char* getWiFiStatus();
bool isWiFiStarted(uint32_t expire=500);

// Internal Module
#define HARDWARE_INTERNAL_MODULE
void intmoduleSendNextFrame();
void startWiFiESPNow();
void stopWiFiESPNow();
void init_espnow();
void disable_espnow();
void pause_espnow();
void resume_espnow();
void init_bind_espnow();
void stop_bind_espnow();
bool is_binding_espnow();

enum Analogs {
  STICK1,
  STICK2,
  STICK3,
  STICK4,
  POT_FIRST,
  POT1 = POT_FIRST,
  POT2,
  POT_LAST = POT2,
  TX_VOLTAGE,
  NUM_ANALOGS
};

#endif