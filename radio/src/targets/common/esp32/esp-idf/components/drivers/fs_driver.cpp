/*
 * Copyright (C) OpenTX
 *
 * Based on code named
 *   th9x - http://code.google.com/p/th9x
 *   er9x - http://code.google.com/p/er9x
 *   gruvin9x - http://code.google.com/p/gruvin9x
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_vfs.h"
#include "esp_vfs_fat.h"
#include "esp_system.h"
#include "esp_spiffs.h"
#include "board_esp32.h"
#include "driver/sdspi_host.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_defs.h"
#define HASASSERT
#include "opentx.h"

// Handle of the wear levelling library instance
static wl_handle_t s_wl_handle = WL_INVALID_HANDLE;
static const char *TAG = "fs_driver";
// Mount path for the partition
const char *base_path = "/flash";
const char *sd_base_path = SD_PATH;
sdmmc_card_t* card = NULL;

#define PIN_NUM_MISO GPIO_NUM_19
#define PIN_NUM_MOSI GPIO_NUM_5
#define PIN_NUM_CLK  GPIO_NUM_18
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

#if defined(SDCARD)

void mountSDCard(){
    ESP_LOGI(TAG, "Initializing SD card SPI peripheral");

    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    host.slot=VSPI_HOST;

    sdspi_slot_config_t slot_config = SDSPI_SLOT_CONFIG_DEFAULT();
    slot_config.gpio_miso = PIN_NUM_MISO;
    slot_config.gpio_mosi = PIN_NUM_MOSI;
    slot_config.gpio_sck  = PIN_NUM_CLK;
    slot_config.gpio_cs   = PIN_NUM_CS;
    slot_config.dma_channel=2;
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };
    gpio_set_pull_mode(PIN_NUM_MISO, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(PIN_NUM_MOSI, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(PIN_NUM_CLK, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(PIN_NUM_CS, GPIO_PULLUP_ONLY);

    esp_err_t ret = esp_vfs_fat_sdmmc_mount("/sdcard", &host, &slot_config, &mount_config, &card);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount filesystem.");
        } else {
            ESP_LOGE(TAG, "Failed to initialize the card (%s).", esp_err_to_name(ret));
        }
        return;
    }
    sdmmc_card_print_info(stdout, card);
}


void sdInit(void){
}

uint32_t sdMounted(void){
    if(card) return true;
    return false;
}

uint32_t sdIsHC(void){
    if(!card) return false;
    return card->ocr & SD_OCR_SDHC_CAP;
}

uint32_t sdGetNoSectors(){
    FATFS *fs;
    DWORD fre_clust;

    /* Get volume information and free clusters of sdcard */
    auto res = f_getfree(ROOT_PATH, &fre_clust, &fs);
    if (res) {
        return 0;
    }
    return (fs->n_fatent - 2) * fs->csize;
}


uint32_t sdGetSize()
{
    return sdGetNoSectors() *FF_SS_SDCARD;
}

uint32_t sdGetFreeSectors(){
    FATFS *fs;
    DWORD fre_clust;

    /* Get volume information and free clusters of sdcard */
    auto res = f_getfree(ROOT_PATH, &fre_clust, &fs);
    if (res) {
        return 0;
    }
    return fre_clust * fs->csize;
}

uint32_t sdGetSpeed()
{
    if(!card) return false;
    return card->csd.tr_speed;
}

void sdMountPoll()
{
  static uint8_t mountTimer;
  if (mountTimer-- == 0) {
    mountTimer = 100;
    if (!sdMounted()) {
        //to do
    }
  }
}

#endif

