#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2s.h"
#include "esp_log.h"
#define HASASSERT
#include "opentx.h"

static const char *TAG = "audio_driver.cpp";

void initAudio(){
    ESP_LOGI(TAG,"initAudio()");
    i2s_config_t i2s_config;
    memset(&i2s_config, 0, sizeof(i2s_config));
    i2s_config.mode =(i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN);
    i2s_config.sample_rate = AUDIO_SAMPLE_RATE;
    i2s_config.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT; /* the DAC module will only take the 8bits from MSB */
    i2s_config.channel_format = I2S_CHANNEL_FMT_ONLY_LEFT;
    i2s_config.communication_format = I2S_COMM_FORMAT_I2S_MSB;
    i2s_config.intr_alloc_flags = 0; // default interrupt priority
    i2s_config.dma_buf_count = 8;
    i2s_config.dma_buf_len = 64;
    i2s_config.use_apll = false;

    //You can call i2s_set_dac_mode to set built-in DAC output mode.
    i2s_set_dac_mode(I2S_DAC_CHANNEL_LEFT_EN);

}

void setSampleRate(uint32_t frequency){
    ESP_LOGI(TAG,"setSampleRate: %d",frequency);
//    i2s_set_sample_rates(I2S_NUM_0, frequency);
}

void audioPlayTask(void * pdata){
    const AudioBuffer * nextBuffer = 0;
    ESP_LOGI(TAG,"Starting audioPlayTask.");

    while(true){
        
        if (nextBuffer) {
            size_t bytes_written=0;
//            i2s_write(I2S_NUM_0, (const void*) nextBuffer->data, nextBuffer->size * sizeof(audio_data_t), &bytes_written, portMAX_DELAY);
            audioQueue.buffersFifo.freeNextFilledBuffer();
            nextBuffer = audioQueue.buffersFifo.getNextFilledBuffer();
        } 
        if (nextBuffer == 0) {
            vTaskDelay(1);//1 tick 
            nextBuffer = audioQueue.buffersFifo.getNextFilledBuffer();
        }
    }
}