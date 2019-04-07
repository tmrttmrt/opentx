#include "esp_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "nvs_flash.h"
#define HASASSERT
#include "opentx.h"
#include "WiFi.h"
extern "C" {
#include "ftp.h"
}

#define FTP_SERVER_TASK_CORE    0
#define FTP_SERVER_STACK_SIZE   0x1000
#define STA_CONNECT_TMO 10000

static const char *TAG = "initWiFi.cpp";
TaskHandle_t wifiTaskHandle = NULL;
static char ssid[sizeof(g_eeGeneral.ssid)+1] = "OpenTX";
static char passwd[sizeof(g_eeGeneral.passwd)+1] = "";
char ftp_pass[FTP_USER_PASS_LEN_MAX > sizeof(g_eeGeneral.ftppass) ? FTP_USER_PASS_LEN_MAX +1 : sizeof(g_eeGeneral.ftppass)+1] = "opentx";
SemaphoreHandle_t wifi_mutex = NULL;


void initWiFi()
{
    wifi_mutex = xSemaphoreCreateMutex();
    init_wifi();
}

void wifiTask(void *pvParameters)
{
    wifi_init_sta(ssid,passwd);
    vTaskDelay(STA_CONNECT_TMO / portTICK_PERIOD_MS);
    if(wifiState!=WIFI_CONNECTED) {
        ESP_LOGI(TAG, "Timeout connecting to '%s'",ssid);
        wifi_init_softap();
    }
    ftpServerTask(pvParameters);
    ESP_LOGI(TAG, "Stopping WiFi ...");
    ESP_ERROR_CHECK(esp_wifi_stop());
    vTaskDelay(100/ portTICK_PERIOD_MS);
    ESP_LOGI(TAG, "Deinit WiFi ...");
    ESP_ERROR_CHECK(esp_wifi_deinit());
    wifiTaskHandle = NULL;
    wifiState=WIFI_IDLE;
    vTaskDelete(NULL);
}


void startWiFi( char *ssid_zchar, char *passwd_zchar, char* ftppass_zchar)
{
    if(!(wifiState & WIFI_IDLE)) return;
    wifiState = WIFI_STARTING;
    if(NULL != ssid_zchar) {
        zchar2str(ssid,ssid_zchar,sizeof(g_eeGeneral.ssid));
        zchar2str(passwd,passwd_zchar,sizeof(g_eeGeneral.passwd));
        zchar2str(ftp_pass,ftppass_zchar,sizeof(g_eeGeneral.ftppass));
        ESP_LOGI(TAG,"ssid: '%s'",ssid);
        ESP_LOGI(TAG,"passwd: '%s'",passwd);
        ESP_LOGI(TAG,"ftppasswd: '%s'",ftp_pass);
    }
    BaseType_t ret_bt=xTaskCreatePinnedToCore(  wifiTask, "wifiTask", FTP_SERVER_STACK_SIZE, NULL, ESP_TASK_PRIO_MAX +7, &wifiTaskHandle, FTP_SERVER_TASK_CORE );
    configASSERT( wifiTaskHandle );
}


void stopWiFi()
{
    if(wifiState & (WIFI_STOPPING | WIFI_IDLE)) return;
    if(stop_wifi()) {
        wifiState = WIFI_STOPPING;
    }
}

const char* getWiFiStatus()
{
    static char stat[STATUS_LEN]="Idle";
    switch(wifiState) {
    case WIFI_IDLE:
        return "Idle";
        break;
    case WIFI_STARTING:
        return "Starting...";
        break;
    case WIFI_CONNECTING:
        return "Connecting...";
        break;
    case WIFI_CONNECTED:
        ip4_addr_t tmp;
        tmp.addr=network_hasip();
        strcpy(stat,"IP: ");
        strcat(stat,ip4addr_ntoa(&tmp));
        return stat;
        break;
    case WIFI_STOPPING:
        return "Stopping...";
        break;
    }
    return stat;
}

bool isWiFiStarted(uint32_t expire)
{
    expireTimer_ms=mp_hal_ticks_ms()+expire;
    return !(wifiState & WIFI_IDLE) ;
}