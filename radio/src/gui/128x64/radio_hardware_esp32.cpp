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

#if defined(PCBESP_WROOM_32)

enum MenuRadioHardwareItems {
  ITEM_RADIO_HARDWARE_WIFI,
  ITEM_RADIO_HARDWARE_WIFI_NAME,
  ITEM_RADIO_HARDWARE_WIFI_PASSWD,
  ITEM_RADIO_HARDWARE_WIFI_TOGGLE,
  ITEM_RADIO_HARDWARE_MAX
};

#define WIFI_COL 9*FW

void menuRadioHardware(event_t event)
{
  MENU(STR_HARDWARE, menuTabGeneral, MENU_RADIO_HARDWARE, ITEM_RADIO_HARDWARE_MAX+1, {HEADER_LINE_COLUMNS LABEL(WiFi),0,0,0});

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
        lcdDrawTextAlignedLeft(y, "WiFi");
        break;
      case ITEM_RADIO_HARDWARE_WIFI_NAME:
        lcdDrawText(FW, y, "SSID:");
        char tmp[sizeof(g_eeGeneral.ssid)];
        str2zchar(tmp,g_eeGeneral.ssid,sizeof(g_eeGeneral.ssid));
        editName(WIFI_COL, y, tmp, sizeof(g_eeGeneral.ssid), event, attr);
        zchar2str(g_eeGeneral.ssid,tmp,sizeof(g_eeGeneral.ssid));
        break;
      case ITEM_RADIO_HARDWARE_WIFI_PASSWD:
        lcdDrawText(FW, y, "Passwd:");
        editName(WIFI_COL, y,  g_eeGeneral.passwd, sizeof(g_eeGeneral.passwd), event, attr);
        break;
      case ITEM_RADIO_HARDWARE_WIFI_TOGGLE:
        static uint8_t value=false;
        value=editCheckBoxIdt(value, FW, WIFI_COL, y, "Enabled:", attr, event);        
        break;
    }
  }
}
#endif 

