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
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#define HASASSERT
#include "opentx.h"

static const char *TAG = "main_esp32.cpp";

void checkBattery();
uint8_t checkTrim(event_t event);

void perMain()
{
#if defined(SIMU)
  doMixerCalculations();
#endif
#if defined(LCD_ST7920)
  uint8_t lcdstate=0;
  #define IS_LCD_REFRESH_ALLOWED()       (0==lcdstate)
#else
  #define IS_LCD_REFRESH_ALLOWED()       (1)
#endif


#if defined(MODULE_ALWAYS_SEND_PULSES)
  if (startupWarningState < STARTUP_WARNING_DONE) {
    // don't do menu's until throttle and switch warnings are handled
    return;
  }
#endif

/*
  if (eepromIsTransferComplete()) {
    if (theFile.isWriting())
      theFile.nextWriteStep();
    else if (TIME_TO_WRITE())
      storageCheck(false);
  }
*/

if (TIME_TO_WRITE()){
      storageCheck(false);
}

#if defined(SDCARD)
//  sdMountPoll();
  logsWrite();
#endif

  event_t evt = getEvent();
//  ESP_LOGI(TAG,"getEvent(): %d",evt);
  evt = checkTrim(evt);

  if (evt && (g_eeGeneral.backlightMode & e_backlight_mode_keys)) backlightOn(); // on keypress turn the light on
  doLoopCommonActions();

#if defined(TELEMETRY_FRSKY) || defined(TELEMETRY_MAVLINK)
  telemetryWakeup();
#endif

#if defined(NAVIGATION_STICKS)
  uint8_t sticks_evt = getSticksNavigationEvent();
  if (sticks_evt) evt = sticks_evt;
#endif

#if defined(GUI)
  const char * warn = warningText;
  bool popupMenuActive = (popupMenuNoItems > 0);

  if (IS_LCD_REFRESH_ALLOWED()) { 
    lcdClear();
    if (menuEvent) {
      menuVerticalPosition = menuEvent == EVT_ENTRY_UP ? menuVerticalPositions[menuLevel] : 0;
      menuHorizontalPosition = 0;
      evt = menuEvent;
      menuEvent = 0;
    }
    menuHandlers[menuLevel]((warn || popupMenuActive) ? 0 : evt);

    if (warn) DISPLAY_WARNING(evt);
#if defined(NAVIGATION_MENUS)
    if (popupMenuActive) {
      const char * result = runPopupMenu(evt);
      if (result) {
        popupMenuHandler(result);
      }
    }
#endif
    drawStatusLine();
  }

#if defined(LCD_KS108)
  lcdRefreshSide();
#elif defined(LCD_ST7920)
  lcdstate = lcdRefresh_ST7920(0);
#else
  lcdRefresh();
#endif

#endif // defined(GUI)

/*  if (SLAVE_MODE()) {
    JACK_PPM_OUT();
  }
  else {
    JACK_PPM_IN();
  }
*/
  checkBattery();
}

uint16_t getBatteryVoltage()
{
  int32_t instant_vbat = anaIn(TX_VOLTAGE); // using filtered ADC value on purpose
  instant_vbat = (instant_vbat * BATT_SCALE * (128 + g_eeGeneral.txVoltageCalibration) ) / 26214;
  instant_vbat += 20; // add 0.2V because of the diode TODO check if this is needed, but removal will beak existing calibrations!!!
  return (uint16_t)instant_vbat;
}


void checkBattery()
{
  static uint32_t batSum;
  static uint8_t sampleCount;
  // filter battery voltage by averaging it
  if (g_vbat100mV == 0) {
    g_vbat100mV = (getBatteryVoltage() + 5) / 10;
    batSum = 0;
    sampleCount = 0;
  }
  else {
    batSum += getBatteryVoltage();
    // TRACE("checkBattery(): sampled = %d", getBatteryVoltage());
    if (++sampleCount >= BAT_AVG_SAMPLES) {
      g_vbat100mV = (batSum + BAT_AVG_SAMPLES * 5 ) / (BAT_AVG_SAMPLES * 10);
      batSum = 0;
      sampleCount = 0;
      // TRACE("checkBattery(): g_vbat100mV = %d", g_vbat100mV);
    }
  }
}

