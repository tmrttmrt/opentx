/*
* Copyright (C) OpenTX
*
* Based on code named
*   https://github.com/yanbe/esp32-ota-server
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

#ifndef _OTA_SERVER_H_
#define _OTA_SERVER_H__

#define OTA_LISTEN_PORT 8032
#define OTA_BUFF_SIZE 1024

enum OtaState {
    OTA_IDLE,
    OTA_WAITING,
    OTA_UPDATING,
    OTA_STOPPING
};

void ota_server_start();
void ota_server_stop();

#endif /* _OTA_SERVER_H_ */
