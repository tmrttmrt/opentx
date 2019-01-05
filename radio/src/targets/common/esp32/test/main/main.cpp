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
}

}