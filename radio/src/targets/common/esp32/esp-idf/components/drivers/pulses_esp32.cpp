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

#include "driver/rmt.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#define HASASSERT
#include "opentx.h"

#define PPM_OUT_RMT_CHANNEL_0 RMT_CHANNEL_0
#define SETUP_PULSES_DURATION 1000 // 500us

static const char *TAG = "pulses_esp32.cpp";
extern uint8_t s_pulses_paused;
DRAM_ATTR uint8_t s_current_protocol[1] = { 255 };
rmt_isr_handle_t handle = 0;
static portMUX_TYPE rmt_spinlock = portMUX_INITIALIZER_UNLOCKED;
QueueHandle_t xPulsesQueue;
SemaphoreHandle_t xPPMSem = NULL;
DRAM_ATTR int16_t locChannelOutputs[MAX_OUTPUT_CHANNELS] = {0};
uint32_t testCount=0;
uint8_t ppmPol;


void setExternalModulePolarity()
{
  ppmPol = GET_MODULE_PPM_POLARITY(EXTERNAL_MODULE);
}

void disable_main_ppm()
{

}

void disable_second_ppm()
{

}

void extmoduleSendNextFrame()
{

}

void disable_ppm(uint32_t port)
{
  if (port == EXTERNAL_MODULE) {
    disable_main_ppm();
  }
  else {
    disable_second_ppm();
  }
}

void init_main_ppm(uint32_t period, uint32_t out_enable){
}

void init_second_ppm(uint32_t period)
{
}

void init_ppm(uint32_t port)
{
  if (port == EXTERNAL_MODULE) {
    init_main_ppm(3000, 1);
  }
  else {
    init_second_ppm(3000);
  }
}


void extmoduleSerialStart(uint32_t baudrate, uint32_t period_half_us, bool inverted)
{
  if (baudrate == 125000) {
    // TODO init_main_ppm could take the period as parameter?
    init_main_ppm(2500 * 2, 0);

  }
  else {
    init_main_ppm(3500 * 2, 0);
//    init_ssc(100);
  }
}


void disable_serial(uint32_t port)
{
  if (port == EXTERNAL_MODULE) {
    disable_ppm(EXTERNAL_MODULE);
  }
  else {
    // TODO
  }
}

template<class T>
void IRAM_ATTR setupPulsesPPM(PpmPulsesData<T> * ppmPulsesData, uint8_t channelsStart, int8_t channelsCount, int8_t frameLength)
{
  int16_t PPM_range = g_model.extendedLimits ? (512*LIMIT_EXT_PERCENT/100) * 2 : 512 * 2; // range of 0.7 .. 1.7msec

  // Total frame length = 22.5msec
  // each pulse is 0.7..1.7ms long with a 0.3ms stop tail
  // The pulse ISR is 2mhz that's why everything is multiplied by 2

  uint8_t firstCh = channelsStart;
  uint8_t lastCh = min<uint8_t>(MAX_OUTPUT_CHANNELS, firstCh + 8 + channelsCount);
  int32_t rest = 22500u * 2;
  rest += int32_t(frameLength) * 1000;
    //The pulse tick is 2mhz that's why everything is multiplied by 2
  uint8_t p = 8 + (channelsCount * 2); //Channels *2
  uint16_t q = (/*g_model.ppmDelay*50+*/300)*2; // Stoplen *2

    uint16_t pulseLevel;
    uint16_t idleLevel;
    if(ppmPol) {
        pulseLevel=1;
        idleLevel=0;
    } else {
        pulseLevel=0;
        idleLevel=1;
    }
    portENTER_CRITICAL(&rmt_spinlock);
    int j=0;
    volatile rmt_item32_t* pd = RMTMEM.chan[PPM_OUT_RMT_CHANNEL_0].data32;
  for (uint32_t i=firstCh; i<lastCh; i++) {
        int16_t v = limit((int16_t)-PPM_range, locChannelOutputs[i], (int16_t)PPM_range) + 2*PPM_CH_CENTER(i);
        rest -= v;
        pd->duration0 = q;
        pd->level0 = pulseLevel;
        pd->duration1 = (v - q); // total pulse width includes stop phase
        pd->level1 = idleLevel;
        pd++;
        j+=2;
    }
    pd->duration0 = q;
    pd->level0 = pulseLevel;
    if (rest > 0x7FFF) rest = 0x7FFF; /* prevents overflows */
    if (rest < 9000)  rest = 9000;
    pd->duration1 = (rest - SETUP_PULSES_DURATION);
    pd->level1 = idleLevel;
    pd++;
    j++;
    pd->duration0 = SETUP_PULSES_DURATION;
    pd->level0 = idleLevel;
    pd->duration1 = 0;
    pd->level1 = idleLevel;
    RMT.tx_lim_ch[PPM_OUT_RMT_CHANNEL_0].limit = j; //Send interrupt SETUP_PULSES_DURATION before the end of the PPM packet
    RMT.int_ena.val |= BIT(PPM_OUT_RMT_CHANNEL_0 + 24);
    testCount++;
    portEXIT_CRITICAL(&rmt_spinlock);
}

/*
void IRAM_ATTR setupPulses()
{
    uint8_t required_protocol = g_model.protocol;
    BaseType_t xHigherPriorityTaskWoken;

    xQueueReceiveFromISR( xPulsesQueue, locChannelOutputs, &xHigherPriorityTaskWoken );

    if (s_pulses_paused) {
        required_protocol = PROTO_NONE;
    }


    if (s_current_protocol[0] != required_protocol) {

#if defined(DSM2_SERIAL) && defined(TELEMETRY_FRSKY)
        if (s_current_protocol[0] == 255 || IS_DSM2_PROTOCOL(s_current_protocol[0])) {
            telemetryInit();
        }
#endif
        s_current_protocol[0] = required_protocol;


        switch (required_protocol) {
#if defined(DSM2_PPM) // For DSM2=SERIAL, the default: case is executed, below
        case PROTO_DSM2_LP45:
        case PROTO_DSM2_DSM2:
        case PROTO_DSM2_DSMX:
            break;
#endif // defined(DSM2_PPM)

#if defined(PXX)
        case PROTO_PXX:
            break;
#endif

        case PROTO_PPMSIM:
            setupPulsesPPM();
            break;

#if defined(DSM2_SERIAL) && defined(TELEMETRY_FRSKY)
        case PROTO_DSM2_LP45:
        case PROTO_DSM2_DSM2:
        case PROTO_DSM2_DSMX:
            DSM2_Init();
            // no break
#endif
        default: // PPM and DSM2=SERIAL modes
            break;
        }
    }

    switch(required_protocol) {
#if defined(PXX)
    case PROTO_PXX:
        // schedule next Mixer calculations
        SCHEDULE_MIXER_END(20*16);
        setupPulsesPXX();
        break;
#endif

#if defined(DSM2)
    case PROTO_DSM2_LP45:
    case PROTO_DSM2_DSM2:
    case PROTO_DSM2_DSMX:
        // schedule next Mixer calculations
        SCHEDULE_MIXER_END(22*16);
        setupPulsesDSM2(); // Different versions for DSM2=SERIAL vs. DSM2=PPM
        break;
#endif

#if defined(IRPROTOS)
    case PROTO_PICZ:
        setupPulsesPiccoZ(g_model.ppmNCH);
        // TODO BSS stbyLevel = 0; //start with 1
        break;
#endif
    case PROTO_NONE:
        break;
    default: // standard PPM protocol
        setupPulsesPPM();
        xSemaphoreGiveFromISR( xPPMSem, &xHigherPriorityTaskWoken );
        break;
    }
    if ( xHigherPriorityTaskWoken == pdTRUE) {
        portYIELD_FROM_ISR();
    }
}


static void IRAM_ATTR rmt_driver_isr_PPM(void* arg)
{
    uint32_t intr_st = RMT.int_st.val;
    if(intr_st & BIT(PPM_OUT_RMT_CHANNEL_0+24)) {
        setupPulses();
        RMT.int_clr.val = intr_st & (BIT(PPM_OUT_RMT_CHANNEL_0) |
                                     BIT(PPM_OUT_RMT_CHANNEL_0+1)| BIT(PPM_OUT_RMT_CHANNEL_0+2)| BIT(PPM_OUT_RMT_CHANNEL_0+24));
    }
}

void resumePulses()
{
    ESP_LOGI(TAG, "Resume pulses called. xPulsesQueue:%x",(uint32_t)xPulsesQueue);
    if(xPulsesQueue) {
        s_pulses_paused = false;
        setupPulses();
    }
}

void initPulses()
{
    rmt_config_t config;

    xPulsesQueue=xQueueCreate( 1, sizeof( channelOutputs ) );
    if(NULL == xPulsesQueue){
        ESP_LOGE(TAG, "Failed to create queue: xPulsesQueue!");
    } else {
        ESP_LOGI(TAG, "xPulsesQueue created");
    }
    memset(&config, 0, sizeof(config));
    config.rmt_mode = RMT_MODE_TX;
    config.channel = PPM_OUT_RMT_CHANNEL_0;
    config.gpio_num = (gpio_num_t)PPM_TX_GPIO;
    config.mem_block_num = 1;
    config.tx_config.loop_en = 1;
    config.tx_config.carrier_en = 0;
    config.clk_div = 40; //2MHz tick

    ESP_ERROR_CHECK(rmt_config(&config));
    if(ESP_OK != rmt_isr_register(rmt_driver_isr_PPM, NULL, ESP_INTR_FLAG_IRAM, &handle )){
        ESP_LOGE(TAG, "Failed to register rmt (PPM) interrupt");
    }
    s_pulses_paused = false;
    setupPulses();
    if(ESP_OK != rmt_tx_start(PPM_OUT_RMT_CHANNEL_0,true)){
        ESP_LOGE(TAG, "Failed to start PPM pulses!");
    }
}

void sendToPulses()
{
    xQueueOverwrite( xPulsesQueue, channelOutputs );
}

*/


