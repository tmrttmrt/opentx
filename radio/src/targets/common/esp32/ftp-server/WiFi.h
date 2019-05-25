/*
 * Copyright (C) OpenTX
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

#ifndef _WIFI_H_
#define _WIFI_H_

#define STATUS_LEN 32
#define mp_hal_ticks_ms()  (esp_timer_get_time()/1000)
#define REBOOT_PATH "/flash/reboot"

enum WifiState {
    WIFI_IDLE = BIT0,
    WIFI_STARTING = BIT1,
    WIFI_CONNECTING = BIT2,
    WIFI_CONNECTED = BIT3,
    WIFI_STOPPING = BIT4
};

#if defined(__cplusplus)
extern "C" {
#endif
void wifi_init_sta(char *ssid, char *passwd);
void wifi_init_softap();
bool stop_wifi();
void init_wifi();
uint32_t network_hasip();
void ftpServerTask (void *pvParameters);
extern volatile enum WifiState wifiState;
extern volatile uint32_t expireTimer_ms;
#if defined(__cplusplus)
}
#endif

extern TaskHandle_t wifiTaskHandle;
extern SemaphoreHandle_t wifi_mutex;

#endif //_WIFI_H_