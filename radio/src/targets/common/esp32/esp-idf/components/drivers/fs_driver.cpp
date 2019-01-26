#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "esp_vfs.h"
#include "esp_vfs_fat.h"
#include "esp_system.h"
#include "esp_spiffs.h"
#include "board_esp32.h"

// Handle of the wear levelling library instance
static wl_handle_t s_wl_handle = WL_INVALID_HANDLE;
static const char *TAG = "fs_driver";
// Mount path for the partition
const char *base_path = "/flash";
const char *sd_base_path = "/sdcard";
sdmmc_card_t* card;

#define PIN_NUM_MISO GPIO_NUM_5
#define PIN_NUM_MOSI GPIO_NUM_16
#define PIN_NUM_CLK  GPIO_NUM_19
#define PIN_NUM_CS   GPIO_NUM_4

void initFS(){
    ESP_LOGI(TAG, "Mounting FAT filesystem ...");
    // To mount device we need name of device partition, define base_path
    // and allow format partition in case if it is new one and was not formated before
    esp_vfs_fat_mount_config_t mount_config;
    memset(&mount_config, 0, sizeof(mount_config));
    mount_config.max_files = 5;
    mount_config.format_if_mount_failed = true;
    mount_config.allocation_unit_size = CONFIG_WL_SECTOR_SIZE;
    esp_err_t err = esp_vfs_fat_spiflash_mount(base_path, "storage", &mount_config, &s_wl_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to mount FATFS (%s)", esp_err_to_name(err));
        return;
    }
}


void mountSDCard(){
    ESP_LOGI(TAG, "Initializing SD card SPI peripheral");

    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    host.slot=VSPI_HOST;
    sdspi_slot_config_t slot_config = SDSPI_SLOT_CONFIG_DEFAULT();
    slot_config.gpio_miso = PIN_NUM_MISO;
    slot_config.gpio_mosi = PIN_NUM_MOSI;
    slot_config.gpio_sck  = PIN_NUM_CLK;
    slot_config.gpio_cs   = PIN_NUM_CS;
    // This initializes the slot without card detect (CD) and write protect (WP) signals.
    // Modify slot_config.gpio_cd and slot_config.gpio_wp if your board has these signals.

       // Options for mounting the filesystem.
    // If format_if_mount_failed is set to true, SD card will be partitioned and
    // formatted in case when mounting fails.
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };

    // Use settings defined above to initialize SD card and mount FAT filesystem.
    // Note: esp_vfs_fat_sdmmc_mount is an all-in-one convenience function.
    // Please check its source code and implement error recovery when developing
    // production applications.
    esp_err_t ret = esp_vfs_fat_sdmmc_mount("/sdcard", &host, &slot_config, &mount_config, &card);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount filesystem. "
                "If you want the card to be formatted, set format_if_mount_failed = true.");
        } else {
            ESP_LOGE(TAG, "Failed to initialize the card (%s). "
                "Make sure SD card lines have pull-up resistors in place.", esp_err_to_name(ret));
        }
        return;
    }
}

