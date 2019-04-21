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
