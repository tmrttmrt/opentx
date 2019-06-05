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

#include "opentx.h"

#if defined(PCBESP_WROOM_32) || defined(PCBESP_HELTEC_32)

#define HW_SETTINGS_COLUMN2            (30 + 5*FW)

enum MenuRadioHardwareItems {
  ITEM_RADIO_HARDWARE_WIFI,
  ITEM_RADIO_HARDWARE_WIFI_NAME,
  ITEM_RADIO_HARDWARE_WIFI_PASSWD,
  ITEM_RADIO_HARDWARE_WIFI_STATUS,
  ITEM_RADIO_HARDWARE_FTP_PASSWD,
  ITEM_RADIO_HARDWARE_LABEL_STICKS,
  ITEM_RADIO_HARDWARE_BATTERY_CALIB,
  ITEM_RADIO_HARDWARE_DEBUG,
  ITEM_RADIO_HARDWARE_MAX
};

#define WIFI_COL (int)(5.8*FW)

void menuRadioHardware(event_t event)
{
  MENU(STR_HARDWARE, menuTabGeneral, MENU_RADIO_HARDWARE, ITEM_RADIO_HARDWARE_MAX+1, {HEADER_LINE_COLUMNS 0,0,0,(uint8_t)-1,0,0,1});

  uint8_t sub = menuVerticalPosition - HEADER_LINE;

  for (uint8_t i=0; i<LCD_LINES-1; i++) {
    coord_t y = MENU_HEADER_HEIGHT + 1 + i*FH;
    uint8_t k = i+menuVerticalOffset;
    for (int j=0; j<=k; j++) {
      if (mstate_tab[j+HEADER_LINE] == HIDDEN_ROW)
        k++;
    }
    uint8_t blink = ((s_editMode>0) ? BLINK|INVERS : INVERS);
    uint8_t attr = (sub == k ? blink : 0);

    switch(k) {
      case ITEM_RADIO_HARDWARE_WIFI:
        {
            uint8_t val = isWiFiStarted();
            drawCheckBox(WIFI_COL, y, val, attr);
            drawFieldLabel(WIFI_COL, y, "WiFi");
            if (attr & (~RIGHT)) val = checkIncDec(event, val, 0, 1, 0);
            if((bool)val != (bool)isWiFiStarted()){ 
                if(val){
                    startWiFi(g_eeGeneral.ssid, g_eeGeneral.passwd, g_eeGeneral.ftppass);
                } else {
                    stopWiFi();
                }   
            }
        }
        break;
      case ITEM_RADIO_HARDWARE_WIFI_NAME:
        lcdDrawText(FW/2, y, "SSID:");
        editName(WIFI_COL, y, g_eeGeneral.ssid, sizeof(g_eeGeneral.ssid), event, attr);
        break;
      case ITEM_RADIO_HARDWARE_WIFI_PASSWD:
        lcdDrawText(FW/2, y, "Pass:");
        editNameMask(WIFI_COL, y,  g_eeGeneral.passwd, sizeof(g_eeGeneral.passwd), true, event, attr);
        break;
      case ITEM_RADIO_HARDWARE_WIFI_STATUS:
        lcdDrawText(FW/2, y, "Stat:");
        lcdDrawText(WIFI_COL, y, (char *)getWiFiStatus());
        break;
      case ITEM_RADIO_HARDWARE_FTP_PASSWD:
        lcdDrawText(FW/2, y, "FTP P.:");
        editNameMask(WIFI_COL, y,  g_eeGeneral.ftppass, sizeof(g_eeGeneral.ftppass), true, event, attr);
        break;
      case ITEM_RADIO_HARDWARE_LABEL_STICKS:
        lcdDrawTextAlignedLeft(y, STR_STICKS);
        lcdDrawText(HW_SETTINGS_COLUMN2, y, BUTTON(TR_CALIBRATION), attr);
        if (attr && event == EVT_KEY_FIRST(KEY_ENTER)) {
          pushMenu(menuRadioCalibration);
        }
        break;
      case ITEM_RADIO_HARDWARE_BATTERY_CALIB:
        lcdDrawTextAlignedLeft( y, STR_BATT_CALIB);
        {
          uint32_t batCalV = getBatteryVoltage()/10;
          putsVolts(HW_SETTINGS_COLUMN2, y, batCalV, attr);
        }
        if (attr) {
          CHECK_INCDEC_GENVAR(event, g_eeGeneral.txVoltageCalibration, -127, 127);
        }
        break;
      case ITEM_RADIO_HARDWARE_DEBUG:
        lcdDrawTextAlignedLeft(y, STR_DEBUG);
        lcdDrawText(HW_SETTINGS_COLUMN2, y, STR_ANALOGS_BTN, menuHorizontalPosition == 0 ? attr : 0);
        lcdDrawText(lcdLastRightPos + 2, y, STR_KEYS_BTN, menuHorizontalPosition == 1 ? attr : 0);
        if (attr && event == EVT_KEY_FIRST(KEY_ENTER)) {
          if (menuHorizontalPosition == 0)
            pushMenu(menuRadioDiagAnalogs);
          else
            pushMenu(menuRadioDiagKeys);
        }
        break;
    }
  }
}
#endif 

