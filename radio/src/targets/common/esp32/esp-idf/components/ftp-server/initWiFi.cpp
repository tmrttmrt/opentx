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

#define FTP_SERVER_TASK_CORE    0
#define FTP_SERVER_STACK_SIZE   0x1000
#define STA_CONNECT_TMO 10000

static const char *TAG = "initWiFi.cpp";
TaskHandle_t wifiTaskHandle = NULL;
static char ssid[sizeof(g_eeGeneral.ssid)];
static char passwd[sizeof(g_eeGeneral.passwd)];
SemaphoreHandle_t wifi_mutex = NULL;


void initWiFi(){
    wifi_mutex = xSemaphoreCreateMutex();
    init_wifi();
}

void wifiTask(void *pvParameters){
    wifi_init_sta(ssid,passwd);
    vTaskDelay(STA_CONNECT_TMO / portTICK_PERIOD_MS);
    if(wifiState!=WIFI_CONNECTED){
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


void startWiFi( char *ssid_zchar, char *passwd_zchar){
    if(!(wifiState & WIFI_IDLE)) return;
    wifiState = WIFI_STARTING;
    zchar2str(ssid,ssid_zchar,sizeof(g_eeGeneral.ssid));
    zchar2str(passwd,passwd_zchar,sizeof(g_eeGeneral.passwd));
    ESP_LOGI(TAG,"ssid: '%s'",ssid);
    ESP_LOGI(TAG,"passwd: '%s'",passwd);
    BaseType_t ret_bt=xTaskCreatePinnedToCore(  wifiTask, "wifiTask", FTP_SERVER_STACK_SIZE, NULL, ESP_TASK_PRIO_MAX +7, &wifiTaskHandle, FTP_SERVER_TASK_CORE );
    configASSERT( wifiTaskHandle );
}


void stopWiFi(){
    if(wifiState & (WIFI_STOPPING | WIFI_IDLE)) return;
    if(stop_wifi()){
        wifiState = WIFI_STOPPING;
    }
}

char* getWiFiStatus(){
    static char buffer[sizeof(wifiStatus)];
    if(xSemaphoreTake(wifi_mutex, 100/ portTICK_PERIOD_MS) == pdTRUE){
        strcpy(buffer,wifiStatus);
        xSemaphoreGive(wifi_mutex);
    }
    return buffer;
}

bool isWiFiStarted(){
    return !(wifiState & WIFI_IDLE) ;
}