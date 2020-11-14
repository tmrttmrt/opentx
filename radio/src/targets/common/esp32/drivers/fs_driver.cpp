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
#include "board_esp32.h"
#include "driver/sdspi_host.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_defs.h"
#include "diskio_impl.h"
#include "diskio_sdmmc.h"
#define HASASSERT
#include "opentx.h"

// Handle of the wear levelling library instance
static wl_handle_t s_wl_handle = WL_INVALID_HANDLE;
static const char *TAG = "fs_driver";
// Mount path for the partition
const char *base_path = "/flash";
const char *sd_base_path = SD_PATH;
sdmmc_card_t* card = NULL;

void initFS()
{
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

void mountSDCard()
{
    ESP_LOGI(TAG, "Initializing SD card SPI peripheral");

    gpio_set_pull_mode(SD_PIN_NUM_MISO, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(SD_PIN_NUM_MOSI, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(SD_PIN_NUM_CLK, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(SD_PIN_NUM_CS, GPIO_PULLUP_ONLY);

    esp_err_t ret = gpio_install_isr_service(0);
    ESP_ERROR_CHECK(ret);

    spi_bus_config_t bus_config;
    memset(&bus_config, 0, sizeof(bus_config));
    bus_config.miso_io_num=SD_PIN_NUM_MISO;
    bus_config.mosi_io_num=SD_PIN_NUM_MOSI;
    bus_config.sclk_io_num=SD_PIN_NUM_CLK;
    bus_config.quadwp_io_num=-1;
    bus_config.quadhd_io_num=-1;
    
    ret = spi_bus_initialize(VSPI_HOST, &bus_config, 2);
    ESP_ERROR_CHECK(ret);

    ret = sdspi_host_init();
    ESP_ERROR_CHECK(ret);  
    
    sdspi_dev_handle_t sdspi_handle;
    sdspi_device_config_t dev_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    dev_config.host_id = VSPI_HOST,
    dev_config.gpio_cs = SD_PIN_NUM_CS,
    
    ret = sdspi_host_init_device(&dev_config, &sdspi_handle);
    ESP_ERROR_CHECK(ret);

    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    host.slot = sdspi_handle;
    
    BYTE pdrv = FF_DRV_NOT_USED;
    if (ff_diskio_get_drive(&pdrv) != ESP_OK || pdrv == FF_DRV_NOT_USED) {
        ESP_LOGE(TAG, "the maximum count of volumes is already mounted");
        return;
    }
    
    card = (sdmmc_card_t*)malloc(sizeof(sdmmc_card_t));
    if (card == NULL) {
        ESP_LOGE(TAG, "could not locate new sdmmc_card_t");
        return;
    }
    
    ret = sdmmc_card_init(&host, card);
    if (ret != ESP_OK) {
        free(card);
        card=NULL;
        ESP_LOGW(TAG, "Failed to init SD card (%s)", esp_err_to_name(ret));
        return;
    }
    
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };

    FATFS* fs = NULL;
    ff_diskio_register_sdmmc(pdrv, card);
    ESP_LOGD(TAG, "using pdrv=%i", pdrv);
    char drv[3] = {(char)('0' + pdrv), ':', 0};
    
    ret = esp_vfs_fat_register("/sdcard", drv, mount_config.max_files, &fs);
    if (ret == ESP_ERR_INVALID_STATE) {
        // it's okay, already registered with VFS
    } else if (ret != ESP_OK) {
        ESP_LOGD(TAG, "esp_vfs_fat_register failed 0x(%x)", ret);
        return;
    }
    
    FRESULT res = f_mount(fs, drv, 1);
    if (res != FR_OK) {
        ESP_LOGW(TAG, "failed to mount card (%d)", res);
        return;
    }
    sdmmc_card_print_info(stdout, card);
}


void sdInit(void)
{
}

uint32_t sdMounted(void)
{
    if(card) return true;
    return false;
}

uint32_t sdIsHC(void)
{
    if(!card) return false;
    return card->ocr & SD_OCR_SDHC_CAP;
}

uint32_t sdGetNoSectors()
{
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

uint32_t sdGetFreeSectors()
{
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

