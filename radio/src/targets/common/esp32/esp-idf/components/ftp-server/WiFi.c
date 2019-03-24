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
#include "WiFi.h"
#define mp_hal_ticks_ms()  (esp_timer_get_time()/1000)

#define DEFAULT_ESP_WIFI_SSID      "OpenTXWiFi"
#define DEFAULT_ESP_WIFI_PASS      ""
#define MAX_STA_CONN    1



static const char *TAG = "WiFi.c";
static EventGroupHandle_t wifi_event_group;
tcpip_adapter_if_t tcpip_if[MAX_ACTIVE_INTERFACES];
char wifiStatus[STATUS_LEN]="idle";

static void setStatus(const char * format, ...){
    va_list arglist;
    va_start(arglist, format);
    xSemaphoreTake(wifi_mutex, portMAX_DELAY);
    vsnprintf(wifiStatus,sizeof(wifiStatus)-1, format, arglist);
    xSemaphoreGive(wifi_mutex);
    va_end(arglist);
    wifiStatus[STATUS_LEN-1]=0;
}


const int WIFI_CONNECTED_BIT = BIT0;

bool isWiFiConnected(){
    return xEventGroupGetBits(wifi_event_group) & WIFI_CONNECTED_BIT;
}

int network_get_active_interfaces()
{
    int n_if = 0;

    for (int i=0; i<MAX_ACTIVE_INTERFACES; i++) {
        tcpip_if[i] = TCPIP_ADAPTER_IF_MAX;
    }
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
        } else {
            ESP_LOGE(TAG,"esp_wifi_get_mode error: %x", ret);
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
    ESP_LOGI(TAG,"Starting FTP server task ...");
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
    ESP_LOGI(TAG, "FTP terminated!");
    vSemaphoreDelete(ftp_mutex);
    ftp_mutex = NULL;
}

static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    switch(event->event_id) {
    case SYSTEM_EVENT_STA_START:
        setStatus("connecting ...");
        esp_wifi_connect();
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        ESP_LOGI(TAG, "got ip:%s", ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
        setStatus("IP:%s",ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
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
        setStatus("reconnecting ...");
        esp_wifi_connect();
        xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED_BIT);
        break;
    default:
        break;
    }
    return ESP_OK;
}

void wifi_init_sta(char *ssid, char *passwd)
{
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = "",
            .password = ""
        },
    };
    strcpy((char *)wifi_config.sta.ssid, ssid);
    strcpy((char *)wifi_config.sta.password, passwd);
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
    ESP_ERROR_CHECK(esp_wifi_start() );
    ESP_LOGI(TAG, "wifi_init_sta finished.");
    ESP_LOGI(TAG, "connect to ap SSID:%s password:%s",
             ssid, passwd);
}

void wifi_init_softap()
{
    ESP_LOGI(TAG, "Initializing access point '%s'",DEFAULT_ESP_WIFI_SSID);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    wifi_config_t wifi_config = {
        .ap = {
            .ssid = DEFAULT_ESP_WIFI_SSID,
            .ssid_len = strlen(DEFAULT_ESP_WIFI_SSID),
            .password = DEFAULT_ESP_WIFI_PASS,
            .max_connection = MAX_STA_CONN,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK
        },
    };
    if (strlen(DEFAULT_ESP_WIFI_PASS) == 0) {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_LOGI(TAG, "wifi_init_softap finished.SSID:%s password:%s",
    DEFAULT_ESP_WIFI_SSID, DEFAULT_ESP_WIFI_PASS);
}

void init_wifi(){
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    tcpip_adapter_init();
    wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));
}


void stop_wifi()
{
	ESP_LOGI(TAG, "Stop FTP");
    ftp_terminate ();
    setStatus("idle");
}