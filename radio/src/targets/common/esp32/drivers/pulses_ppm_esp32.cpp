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

#include "soc/periph_defs.h"
#include "driver/rmt.h"
#include "driver/periph_ctrl.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#define HASASSERT
#include "opentx.h"

#define PPM_OUT_RMT_CHANNEL_0 RMT_CHANNEL_0
#define SETUP_PULSES_DURATION 1000 // 500us
#define MAX_TX_RMT_ITEMS MAX_OUTPUT_CHANNELS+2


#define RMT_SOUCCE_CLK_APB (APB_CLK_FREQ) /*!< RMT source clock is APB_CLK */
#define RMT_SOURCE_CLK_REF (1 * 1000000)  /*!< not used yet */
#define RMT_SOURCE_CLK(select) ((select == RMT_BASECLK_REF) ? (RMT_SOURCE_CLK_REF) : (RMT_SOUCCE_CLK_APB)) /*! RMT source clock frequency */ 

static const char *TAG = "pulses_esp32.cpp";

DRAM_ATTR uint8_t s_current_protocol[1] = { 255 };
static rmt_isr_handle_t handle = 0;
TaskHandle_t xInitPulsesTaskId;
static portMUX_TYPE rmt_spinlock = portMUX_INITIALIZER_UNLOCKED;
static QueueHandle_t xPulsesQueue;
static DRAM_ATTR rmt_item32_t rmtItems[MAX_TX_RMT_ITEMS];
volatile uint32_t testCount=0;
volatile uint32_t testTime=0;
static volatile bool ppmEnabled = false;
static volatile int intTrCount = 0;
volatile uint64_t lastMixTime;
volatile uint32_t ppmMixDly;

esp_err_t rmt_config_otx(const rmt_config_t* rmt_param)
{
    rmt_mode_t mode = rmt_param->rmt_mode;
    rmt_channel_t channel = rmt_param->channel;
    gpio_num_t gpio_num = rmt_param->gpio_num;
    uint8_t mem_cnt = rmt_param->mem_block_num;
    int clk_div = rmt_param->clk_div;
    uint32_t carrier_freq_hz = rmt_param->tx_config.carrier_freq_hz;
    bool carrier_en = rmt_param->tx_config.carrier_en;

    periph_module_enable(PERIPH_RMT_MODULE);

    RMT.conf_ch[channel].conf0.div_cnt = clk_div;
    /*Visit data use memory not FIFO*/
    RMT.apb_conf.fifo_mask = RMT_DATA_MODE_MEM;
    /*Reset tx/rx memory index */
    portENTER_CRITICAL(&rmt_spinlock);
    RMT.conf_ch[channel].conf1.mem_rd_rst = 1;
    RMT.conf_ch[channel].conf1.mem_wr_rst = 1;
    portEXIT_CRITICAL(&rmt_spinlock);

    if(mode == RMT_MODE_TX) {
        uint32_t rmt_source_clk_hz = 0;
        uint16_t carrier_duty_percent = rmt_param->tx_config.carrier_duty_percent;
        uint8_t carrier_level = rmt_param->tx_config.carrier_level;
        uint8_t idle_level = rmt_param->tx_config.idle_level;

        portENTER_CRITICAL(&rmt_spinlock);
        RMT.conf_ch[channel].conf1.tx_conti_mode = rmt_param->tx_config.loop_en;
        /*Memory set block number*/
        RMT.conf_ch[channel].conf0.mem_size = mem_cnt;
        RMT.conf_ch[channel].conf1.mem_owner = RMT_MEM_OWNER_TX;
        /*We use APB clock in this version, which is 80Mhz, later we will release system reference clock*/
        RMT.conf_ch[channel].conf1.ref_always_on = RMT_BASECLK_APB;
        rmt_source_clk_hz = RMT_SOURCE_CLK(RMT_BASECLK_APB);
        /*Set idle level */
        RMT.conf_ch[channel].conf1.idle_out_en = rmt_param->tx_config.idle_output_en;
        RMT.conf_ch[channel].conf1.idle_out_lv = idle_level;
        /*Set carrier*/
        RMT.conf_ch[channel].conf0.carrier_en = carrier_en;
        if (carrier_en) {
            uint32_t duty_div, duty_h, duty_l;
            duty_div = rmt_source_clk_hz / carrier_freq_hz;
            duty_h = duty_div * carrier_duty_percent / 100;
            duty_l = duty_div - duty_h;
            RMT.conf_ch[channel].conf0.carrier_out_lv = carrier_level;
            RMT.carrier_duty_ch[channel].high = duty_h;
            RMT.carrier_duty_ch[channel].low = duty_l;
        } else {
            RMT.conf_ch[channel].conf0.carrier_out_lv = 0;
            RMT.carrier_duty_ch[channel].high = 0;
            RMT.carrier_duty_ch[channel].low = 0;
        }
        portEXIT_CRITICAL(&rmt_spinlock);

        ESP_LOGD(TAG, "Rmt Tx Channel %u|Gpio %u|Sclk_Hz %u|Div %u|Carrier_Hz %u|Duty %u",
                 channel, gpio_num, rmt_source_clk_hz, clk_div, carrier_freq_hz, carrier_duty_percent);

    }
    else if(RMT_MODE_RX == mode) {
        uint8_t filter_cnt = rmt_param->rx_config.filter_ticks_thresh;
        uint16_t threshold = rmt_param->rx_config.idle_threshold;

        portENTER_CRITICAL(&rmt_spinlock);
        /*clock init*/
        RMT.conf_ch[channel].conf1.ref_always_on = RMT_BASECLK_APB;
        uint32_t rmt_source_clk_hz = RMT_SOURCE_CLK(RMT_BASECLK_APB);
        /*memory set block number and owner*/
        RMT.conf_ch[channel].conf0.mem_size = mem_cnt;
        RMT.conf_ch[channel].conf1.mem_owner = RMT_MEM_OWNER_RX;
        /*Set idle threshold*/
        RMT.conf_ch[channel].conf0.idle_thres = threshold;
        /* Set RX filter */
        RMT.conf_ch[channel].conf1.rx_filter_thres = filter_cnt;
        RMT.conf_ch[channel].conf1.rx_filter_en = rmt_param->rx_config.filter_en;
        portEXIT_CRITICAL(&rmt_spinlock);

        ESP_LOGD(TAG, "Rmt Rx Channel %u|Gpio %u|Sclk_Hz %u|Div %u|Thresold %u|Filter %u",
            channel, gpio_num, rmt_source_clk_hz, clk_div, threshold, filter_cnt);
    }
    rmt_set_pin(channel, mode, gpio_num);
    return ESP_OK;
}


esp_err_t rmt_tx_stop_otx(rmt_channel_t channel)
{
    portENTER_CRITICAL(&rmt_spinlock);
    RMTMEM.chan[channel].data32[0].val = 0;
    RMT.conf_ch[channel].conf1.tx_start = 0;
    RMT.conf_ch[channel].conf1.mem_rd_rst = 1;
    RMT.conf_ch[channel].conf1.mem_rd_rst = 0;
    portEXIT_CRITICAL(&rmt_spinlock);
    return ESP_OK;
}

esp_err_t rmt_tx_start_otx(rmt_channel_t channel, bool tx_idx_rst)
{
    portENTER_CRITICAL(&rmt_spinlock);
    if(tx_idx_rst) {
        RMT.conf_ch[channel].conf1.mem_rd_rst = 1;
    }
    RMT.conf_ch[channel].conf1.mem_owner = RMT_MEM_OWNER_TX;
    RMT.conf_ch[channel].conf1.tx_start = 1;
    portEXIT_CRITICAL(&rmt_spinlock);
    return ESP_OK;
}

inline void rmt_set_intr_enable_mask_otx(uint32_t mask)
{
    portENTER_CRITICAL(&rmt_spinlock);
    RMT.int_ena.val |= mask;
    portEXIT_CRITICAL(&rmt_spinlock);
}

inline void rmt_clr_intr_enable_mask_otx(uint32_t mask)
{
    portENTER_CRITICAL(&rmt_spinlock);
    RMT.int_ena.val &= (~mask);
    portEXIT_CRITICAL(&rmt_spinlock);
}

esp_err_t rmt_set_tx_thr_intr_en_otx(rmt_channel_t channel, bool en, uint16_t evt_thresh)
{
    if(en) {
        portENTER_CRITICAL(&rmt_spinlock);
        RMT.tx_lim_ch[channel].limit = evt_thresh;
        portEXIT_CRITICAL(&rmt_spinlock);
        RMT.apb_conf.mem_tx_wrap_en = true;
        rmt_set_intr_enable_mask_otx(BIT(channel + 24));
    } else {
        rmt_clr_intr_enable_mask_otx(BIT(channel + 24));

    }
    return ESP_OK;
}

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
    rmt_set_tx_thr_intr_en_otx(PPM_OUT_RMT_CHANNEL_0, true, intTrCount);
    rmt_tx_start_otx(PPM_OUT_RMT_CHANNEL_0,true);
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
/*
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
*/
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

  ESP_ERROR_CHECK(rmt_config_otx(&config));
  if(ESP_OK != rmt_isr_register(rmt_driver_isr_PPM, NULL, ESP_INTR_FLAG_IRAM, &handle )){
    ESP_LOGE(TAG, "Failed to register rmt (PPM) interrupt");
  }
  ESP_LOGI(TAG, "PPM initialized.");
  rmt_tx_stop_otx(PPM_OUT_RMT_CHANNEL_0);
}


