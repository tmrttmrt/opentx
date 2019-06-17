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
#define MAX_TX_RMT_ITEMS MAX_OUTPUT_CHANNELS+2

static const char *TAG = "pulses_esp32.cpp";

DRAM_ATTR uint8_t s_current_protocol[1] = { 255 };
rmt_isr_handle_t handle = 0;
TaskHandle_t xInitPulsesTaskId;
static portMUX_TYPE rmt_spinlock = portMUX_INITIALIZER_UNLOCKED;
QueueHandle_t xPulsesQueue;
static DRAM_ATTR rmt_item32_t rmtItems[MAX_TX_RMT_ITEMS];
volatile uint32_t testCount=0;
volatile uint32_t testTime=0;
static volatile bool ppmEnabled = false;
static volatile int intTrCount = 0;
volatile uint64_t lastMixTime;
volatile uint32_t ppmMixDly;

void extmoduleSendNextFrame()
{
  xQueueOverwrite( xPulsesQueue, rmtItems );
  lastMixTime = esp_timer_get_time();
}

void init_main_ppm(uint32_t out_enable){
  ESP_LOGI(TAG, "'init_main_ppm' called. out_enable:%d",out_enable);
  if(out_enable){
    rfPwrOn();
    setupPulsesPPMExternalModule();
    for(int i=0; i < MAX_TX_RMT_ITEMS; i++){
      RMTMEM.chan[PPM_OUT_RMT_CHANNEL_0].data32[i].val = rmtItems[i].val;
    }
    rmt_set_tx_thr_intr_en(PPM_OUT_RMT_CHANNEL_0, true, intTrCount);
    rmt_tx_start(PPM_OUT_RMT_CHANNEL_0,true);
    ppmEnabled = true;
    ESP_LOGI(TAG, "PPM started: intTrCount: %d", intTrCount);
  } else {
    ESP_LOGI(TAG, "PPM stopped: intTrCount: %d", intTrCount);
    ppmEnabled = false;
    rfPwrOff();
  }
}

void extmodulePpmStart()
{
  ESP_LOGI(TAG, "'extmodulePpmStart' called.");
  init_main_ppm(1);
}

void extmoduleStop()
{
  init_main_ppm(0);
}

void extmoduleSerialStart(uint32_t baudrate, uint32_t period_half_us, bool inverted)
{
  ESP_LOGI(TAG, "'extmoduleSerialStart' called. baudrate:%d",baudrate);
  if (baudrate == 125000) {
    // TODO init_main_ppm could take the period as parameter?
    init_main_ppm(0);
  }
  else {
    init_main_ppm(0);
    //    init_ssc(100);
  }
}

void disable_serial(uint32_t port)
{
  ESP_LOGI(TAG, "'disable_serial' called. port:%d",port);
  if (port == EXTERNAL_MODULE) {
    disable_ppm(EXTERNAL_MODULE);
  }
  else {
    // TODO
  }
}

template<class T>
void setupPulsesPPM(PpmPulsesData<T> * ppmPulsesData, uint8_t channelsStart, int8_t channelsCount, int8_t frameLength)
{
  int16_t PPM_range = g_model.extendedLimits ? (512*LIMIT_EXT_PERCENT/100) * 2 : 512 * 2; // range of 0.7 .. 1.7msec

  // Total frame length = 22.5msec
  // each pulse is 0.7..1.7ms long with a 0.3ms stop tail
  // The pulse ISR is 2mhz that's why everything is multiplied by 2

  uint8_t firstCh = channelsStart;
  uint8_t lastCh = min<uint8_t>(MAX_OUTPUT_CHANNELS, firstCh + 8 + channelsCount);
  int32_t rest = 22500u * 2;
  rest += int32_t(frameLength) * 1000;
  uint16_t pulseLevel;
  uint16_t idleLevel;
  uint16_t ppmDelay = GET_MODULE_PPM_DELAY(EXTERNAL_MODULE);
  if(GET_MODULE_PPM_POLARITY(EXTERNAL_MODULE)) {
    pulseLevel=1;
    idleLevel=0;
  } else {
    pulseLevel=0;
    idleLevel=1;
  }
  int j=0;
  rmt_item32_t* pd = rmtItems;
  for (uint32_t i=firstCh; i<lastCh; i++) {
    int16_t v = limit((int16_t)-PPM_range, channelOutputs[i], (int16_t)PPM_range) + 2*PPM_CH_CENTER(i);
    rest -= v;
    pd->duration0 = ppmDelay;
    pd->level0 = pulseLevel;
    pd->duration1 = (v - ppmDelay); // total pulse width includes stop phase
    pd->level1 = idleLevel;
    pd++;
    j+=1;
  }
  pd->duration0 = ppmDelay;
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
  intTrCount = j;
  pd++;
  pd->val = 0;
}

void setupPulsesPPMExternalModule()
{
  setupPulsesPPM(&extmodulePulsesData.ppm, g_model.moduleData[EXTERNAL_MODULE].channelsStart, g_model.moduleData[EXTERNAL_MODULE].channelsCount, g_model.moduleData[EXTERNAL_MODULE].ppm.frameLength);
}

static void IRAM_ATTR rmt_driver_isr_PPM(void* arg)
{ 
  BaseType_t mustYield=false;
  uint32_t intr_st = RMT.int_st.val;
  if(intr_st & BIT(PPM_OUT_RMT_CHANNEL_0 + 24)) {
    portENTER_CRITICAL_ISR(&rmt_spinlock);
    if(ppmEnabled){
      ppmMixDly = esp_timer_get_time() - lastMixTime;
      static rmt_item32_t cRmtItems[MAX_TX_RMT_ITEMS];
      if (pdTRUE == xQueueReceiveFromISR( xPulsesQueue, cRmtItems, &mustYield)){
        for(int i=0; i < MAX_TX_RMT_ITEMS; i++){
          RMTMEM.chan[PPM_OUT_RMT_CHANNEL_0].data32[i].val = rmtItems[i].val;
        }
        RMT.tx_lim_ch[PPM_OUT_RMT_CHANNEL_0].limit = intTrCount; //Send interrupt SETUP_PULSES_DURATION before the end of the PPM packet
        testCount++;
      }
      RMT.int_ena.val |= BIT(PPM_OUT_RMT_CHANNEL_0 + 24);
    } 
    else {
      RMTMEM.chan[PPM_OUT_RMT_CHANNEL_0].data32[0].val = 0;
      RMT.conf_ch[PPM_OUT_RMT_CHANNEL_0].conf1.tx_start = 0;
      RMT.conf_ch[PPM_OUT_RMT_CHANNEL_0].conf1.mem_rd_rst = 1;
      RMT.conf_ch[PPM_OUT_RMT_CHANNEL_0].conf1.mem_rd_rst = 0;
      intTrCount = 1;
    }
    portEXIT_CRITICAL_ISR(&rmt_spinlock);
  }
  RMT.int_clr.val = intr_st;
  if (mustYield) portYIELD_FROM_ISR();
}

void initPulses()
{
  rmt_config_t config;
  memset(&config, 0, sizeof(config));

  xPulsesQueue=xQueueCreate( 1, sizeof(rmtItems) );
  if(NULL == xPulsesQueue){
    ESP_LOGE(TAG, "Failed to create queue: xPulsesQueue!");
  } else {
    ESP_LOGI(TAG, "xPulsesQueue created");
  }

  config.rmt_mode = RMT_MODE_TX;
  config.channel = PPM_OUT_RMT_CHANNEL_0;
  config.gpio_num = PPM_TX_GPIO;
  config.mem_block_num = 1;
  config.tx_config.loop_en = 1;
  config.tx_config.carrier_en = 0;
  config.clk_div = 40; //2MHz tick

  ESP_ERROR_CHECK(rmt_config(&config));
  if(ESP_OK != rmt_isr_register(rmt_driver_isr_PPM, NULL, ESP_INTR_FLAG_IRAM, &handle )){
    ESP_LOGE(TAG, "Failed to register rmt (PPM) interrupt");
  }
  rmt_tx_stop(PPM_OUT_RMT_CHANNEL_0);
  ESP_LOGI(TAG, "PPM initialized.");
}


