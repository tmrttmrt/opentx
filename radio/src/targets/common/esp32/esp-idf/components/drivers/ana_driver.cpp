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

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_log.h"
#define HASASSERT
#include "opentx.h"

adc1_channel_t analogPorts[]={ADC1_CHANNEL_0,ADC1_CHANNEL_3,ADC1_CHANNEL_6,ADC1_CHANNEL_7,ADC1_CHANNEL_4,ADC1_CHANNEL_5,(adc1_channel_t) ADC2_CHANNEL_8};
static_assert(sizeof(analogPorts)/sizeof(adc1_channel_t)==NUM_ANALOGS,"Analog pins assignment issue");

static const char *TAG = "ana_driver.cpp";

void IRAM_ATTR getADC(){
    int     read_raw;
    
    int channel=0;
    for(;channel<7;channel++){
        s_anaFilt[channel]=adc1_get_raw(analogPorts[channel]);
    }
    for(;channel<NUM_ANALOGS;channel++){
        adc2_get_raw((adc2_channel_t)analogPorts[channel], ADC_WIDTH_12Bit, &read_raw);
        s_anaFilt[channel]=read_raw;
    }
    
}

void initADC(){
    adc1_config_width(ADC_WIDTH_BIT_12);
    
    int channel=0;
    for(;channel<7;channel++){
        adc1_config_channel_atten(analogPorts[channel], ADC_ATTEN_DB_11);
    }
    for(;channel<NUM_ANALOGS;channel++){
        adc2_config_channel_atten((adc2_channel_t)analogPorts[channel], ADC_ATTEN_DB_11);
    }
    
}

