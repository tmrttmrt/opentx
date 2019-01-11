#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2s.h"
#include "esp_log.h"
#define HASASSERT
#include "opentx.h"

static const char *TAG = "audio_driver.cpp";

void initAudio(){
    i2s_config_t i2s_config;
    i2s_config.mode =(i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN);
    i2s_config.sample_rate = 44100;
    i2s_config.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT; /* the DAC module will only take the 8bits from MSB */
    i2s_config.channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT;
    i2s_config.communication_format = I2S_COMM_FORMAT_I2S_MSB;
    i2s_config.intr_alloc_flags = 0; // default interrupt priority
    i2s_config.dma_buf_count = 8;
    i2s_config.dma_buf_len = 64;
    i2s_config.use_apll = false;

    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);   //install and start i2s driver
    
    i2s_pin_config_t pin_config = {
        .bck_io_num = -1,
        .ws_io_num = AUD_DAC_GPIO,
        .data_out_num = -1,
        .data_in_num = -1                                               //Not used
    };
    i2s_set_pin(I2S_NUM_0, &pin_config); //for internal DAC, this will enable both of the internal channels

    //You can call i2s_set_dac_mode to set built-in DAC output mode.
    i2s_set_dac_mode(I2S_DAC_CHANNEL_RIGHT_EN);

    //    i2s_set_sample_rates(i2s_num, 22050); //set sample rates

    //    i2s_driver_uninstall(i2s_num); //stop & destroy i2s driver
}
