#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_spiffs.h"
#include "esp_log.h"

static const char *TAG = "main.cpp";


void eepromReadBlock(uint8_t * buffer, size_t address, size_t size);
void eepromWriteBlock(uint8_t * buffer, size_t address, size_t size);
void eepromInit();
extern "C" {

    void initFS();
    
    void app_main()
    {
        uint8_t buff[64];
        initFS();
        eepromInit();
        eepromReadBlock(buff, 0, 64);
        eepromReadBlock(buff, 128, 64); 
        for (int i = 10; i >= 0; i--) {
            printf("Restarting in %d seconds...    \r", i);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        fflush(stdout);
//        ESP_ERROR_CHECK( esp_vfs_spiffs_unregister(NULL));
        esp_restart();
    }

}