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
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_log.h"
#define HASASSERT
#include "opentx.h"

adc1_channel_t analogPorts[]=ADC_CHAN;

static_assert(sizeof(analogPorts)/sizeof(adc1_channel_t)==NUM_ANALOGS,"Analog pins assignment issue");

static const char *TAG = "ana_driver.cpp";

void getADC(){
    int     read_raw;
    int     i;
    uint32_t sum;
    int channel=0;
    
    for(;channel<(NUM_ANALOGS-NUM_ADC2);channel++){
        sum=0;
        for(i=0;i<ADC1_NAVG;i++){
            sum += adc1_get_raw(analogPorts[channel]);
        }
        sum/=ADC1_NAVG;
        s_anaFilt[channel]=sum;
//        if(1==channel)
//            espLogI("%d: %d\n",channel,sum);
    }

    for(;channel<NUM_ANALOGS;channel++){
        esp_err_t err=adc2_get_raw((adc2_channel_t)analogPorts[channel], ADC_WIDTH_12Bit, &read_raw);
        if ( err == ESP_OK ) {
            s_anaFilt[channel]=read_raw;
        } else {
            s_anaFilt[channel]=0;
        }
    }
    
}

void initADC(){
    ESP_LOGI(TAG,"Configuring ADCs ...");
    adc1_config_width(ADC_WIDTH_BIT_12);
    
    int channel=0;
    for(;channel<(NUM_ANALOGS-NUM_ADC2);channel++){
        adc1_config_channel_atten(analogPorts[channel], ADC_ATTEN_DB_11);
    }
    for(;channel<NUM_ANALOGS;channel++){
        adc2_config_channel_atten((adc2_channel_t)analogPorts[channel], ADC_ATTEN_DB_11);
    }
    
}

