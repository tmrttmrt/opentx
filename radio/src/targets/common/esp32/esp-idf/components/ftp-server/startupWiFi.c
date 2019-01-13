#include <string.h>
#include <sys/time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "ftp.h"
#include "network.h"

#define mp_hal_ticks_ms()  (esp_timer_get_time()/1000)

#define EXAMPLE_ESP_WIFI_SSID      "OpenTXWiFi"
#define EXAMPLE_ESP_WIFI_PASS      ""
#define EXAMPLE_MAX_STA_CONN       1
#define FTP_SERVER_TASK_CORE 0
#define FTP_SERVER_STACK_SIZE       0x1000

TaskHandle_t xFTPServerTaskHandle = NULL;
static const char *TAG = "startupWiFi.cpp";
static EventGroupHandle_t wifi_event_group;
tcpip_adapter_if_t tcpip_if[MAX_ACTIVE_INTERFACES];
int wifi_network_state = 0;

const int WIFI_CONNECTED_BIT = BIT0;

int network_get_active_interfaces()
{
    int n_if = 0;

    for (int i=0; i<MAX_ACTIVE_INTERFACES; i++) {
        tcpip_if[i] = TCPIP_ADAPTER_IF_MAX;
    }
    if (wifi_network_state) {
        wifi_mode_t mode;
        esp_err_t ret = esp_wifi_get_mode(&mode);
        if (ret == ESP_OK) {
            if (mode == WIFI_MODE_STA) {
                n_if = 1;
                tcpip_if[0] = TCPIP_ADAPTER_IF_STA;
            }
            else if (mode == WIFI_MODE_AP) {
                n_if = 1;
                tcpip_if[0] = TCPIP_ADAPTER_IF_AP;
            }
            else if (mode == WIFI_MODE_APSTA) {
                n_if = 2;
                tcpip_if[0] = TCPIP_ADAPTER_IF_STA;
                tcpip_if[1] = TCPIP_ADAPTER_IF_AP;
            }
        }
    }
    return n_if;
}

uint32_t network_hasip()
{
    tcpip_adapter_ip_info_t ip_info = {0};
    int n_if = network_get_active_interfaces();
    if (n_if) {
        for (int i=0; i<n_if; i++) {
            tcpip_adapter_get_ip_info(tcpip_if[i], &ip_info);
            if (ip_info.ip.addr > 0) {
                return ip_info.ip.addr;
            }
        }
    }
    return 0;
}

static bool _check_network()
{
    uint32_t ip = network_hasip();
    if (ip == 0) {
        return false;
    }
    return true;
}


void ftpServerTask (void *pvParameters)
{
    uint64_t elapsed, time_ms = mp_hal_ticks_ms();
    // Initialize ftp, create rx buffer and mutex
    if (!ftp_init()) {
        ESP_LOGE("[Ftp]", "Init Error");
        goto exit1;
    }

    while (!_check_network()) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        if (ftp_stop_requested()) goto exit;
    }

    // We have network connection, enable ftp
    ftp_enable();
    ESP_LOGI(TAG,"ftp server enabled.");
    time_ms = mp_hal_ticks_ms();
    while (1) {
        // Calculate time between two ftp_run() calls
        elapsed = mp_hal_ticks_ms() - time_ms;
        time_ms = mp_hal_ticks_ms();

        int res = ftp_run(elapsed);
        if (res < 0) {
            if (res == -1) {
                ESP_LOGD("[Ftp]", "\nRun Error");
            }
            // -2 is returned if Ftp stop was requested by user
            break;
        }

        vTaskDelay(1);

        // ---- Check if network is still available ----
        if (!_check_network()) {
            bool was_enabled = ftp_isenabled();
            ftp_disable();
            while (!_check_network()) {
                vTaskDelay(200 / portTICK_PERIOD_MS);
                if (ftp_stop_requested()) goto exit;
            }
            if (was_enabled) ftp_enable();
        }
        // ------------------------------------------
    }
exit:
    ftp_disable();
    ftp_deinit();
exit1:
    ESP_LOGI("[Ftp]", "\nTask terminated!");
    xFTPServerTaskHandle = NULL;
    vSemaphoreDelete(ftp_mutex);
    ftp_mutex = NULL;
    vTaskDelete(NULL);
}

static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    switch(event->event_id) {
    case SYSTEM_EVENT_STA_START:
        esp_wifi_connect();
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        ESP_LOGI(TAG, "got ip:%s",
        ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
        break;
    case SYSTEM_EVENT_AP_STACONNECTED:
        ESP_LOGI(TAG, "station:"MACSTR" join, AID=%d",
        MAC2STR(event->event_info.sta_connected.mac),
        event->event_info.sta_connected.aid);
        break;
    case SYSTEM_EVENT_AP_STADISCONNECTED:
        ESP_LOGI(TAG, "station:"MACSTR"leave, AID=%d",
        MAC2STR(event->event_info.sta_disconnected.mac),
        event->event_info.sta_disconnected.aid);
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        esp_wifi_connect();
        xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED_BIT);
        break;
    default:
        break;
    }
    return ESP_OK;
}

void wifi_init_softap()
{
    wifi_event_group = xEventGroupCreate();

    tcpip_adapter_init();
    ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    wifi_config_t wifi_config = {
        .ap = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .ssid_len = strlen(EXAMPLE_ESP_WIFI_SSID),
            .password = EXAMPLE_ESP_WIFI_PASS,
            .max_connection = EXAMPLE_MAX_STA_CONN,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK
        },
    };
    if (strlen(EXAMPLE_ESP_WIFI_PASS) == 0) {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    wifi_network_state=1;
    ESP_LOGI(TAG, "wifi_init_softap finished.SSID:%s password:%s",
    EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
}

void initWiFi(){
        //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    wifi_init_softap();
    BaseType_t ret_bt=xTaskCreatePinnedToCore(  ftpServerTask, "ftpServerTask", FTP_SERVER_STACK_SIZE, NULL, ESP_TASK_PRIO_MIN +2, &xFTPServerTaskHandle, FTP_SERVER_TASK_CORE );
    configASSERT( xFTPServerTaskHandle );
}