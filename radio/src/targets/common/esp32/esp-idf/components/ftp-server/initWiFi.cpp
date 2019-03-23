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
extern "C" {
void wifi_init_sta(char *ssid, char *passwd);
void wifi_init_softap();
void ftpServerTask (void *pvParameters);
bool isWiFiConnected();
}

#define STA_ESP_WIFI_SSID   "ssid"
#define STA_ESP_WIFI_PASS   "passwd"
#define FTP_SERVER_TASK_CORE    0
#define FTP_SERVER_STACK_SIZE   0x1000
#define STA_CONNECT_TMO 5000

static const char *TAG = "initWiFi.cpp";
extern TaskHandle_t FtpTaskHandle;

void initWiFi(){
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    strcpy((char *)(g_eeGeneral.ssid), STA_ESP_WIFI_SSID);
    strcpy((char *)(g_eeGeneral.passwd), STA_ESP_WIFI_PASS);
    wifi_init_sta(g_eeGeneral.ssid,g_eeGeneral.passwd);
    vTaskDelay(STA_CONNECT_TMO / portTICK_PERIOD_MS);
    if(!isWiFiConnected()){
        ESP_LOGI(TAG, "Timeout connecting to '%s'",g_eeGeneral.ssid);
        wifi_init_softap();
    }
    BaseType_t ret_bt=xTaskCreatePinnedToCore(  ftpServerTask, "ftpServerTask", FTP_SERVER_STACK_SIZE, NULL, ESP_TASK_PRIO_MAX +7, &FtpTaskHandle, FTP_SERVER_TASK_CORE );
    configASSERT( FtpTaskHandle );
    
}