/*
 * Copyright (C) OpenTX
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

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2s.h"
#include "esp_log.h"
#define HASASSERT
#include "opentx.h"

static const char *TAG = "audio_driver.cpp";


void initAudio()
{
    ESP_LOGI(TAG,"initAudio()");
    i2s_config_t i2s_config;
    memset(&i2s_config, 0, sizeof(i2s_config));
    i2s_config.mode =(i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN);
    i2s_config.sample_rate = 2*AUDIO_SAMPLE_RATE;//Upsampling due to strange i2s behaviour
    i2s_config.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT; /* the DAC module will only take the 8bits from MSB */
    i2s_config.channel_format = I2S_CHANNEL_FMT_ALL_LEFT;
    i2s_config.communication_format = I2S_COMM_FORMAT_I2S_MSB;
    i2s_config.intr_alloc_flags = 0; // default interrupt priority
    i2s_config.dma_buf_count = 4;
    i2s_config.dma_buf_len = AUDIO_BUFFER_SIZE* sizeof(audio_data_t);
    i2s_config.use_apll = false;

    esp_err_t err = i2s_driver_install(I2S_NUM_0,&i2s_config,0,NULL);
    if (err != ESP_OK) {
        // handle other errors
        ESP_LOGE(TAG,"%s",esp_err_to_name(err));
    }
    //You can call i2s_set_dac_mode to set built-in DAC output mode.
    i2s_set_dac_mode(I2S_DAC_CHANNEL_LEFT_EN);
}

void setSampleRate(uint32_t frequency)
{
    ESP_LOGI(TAG,"setSampleRate: %d",frequency);
    i2s_set_clk(I2S_NUM_0, 2*frequency, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);
}

