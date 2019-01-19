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
#include "freertos/task.h"
#define HASASSERT
#include "opentx.h"

#define PPM_OUT_RMT_CHANNEL_0 RMT_CHANNEL_0
#define SETUP_PULSES_DURATION 1000 // 500us

uint8_t s_pulses_paused = 0;
uint8_t s_current_protocol[1] = { 255 };
rmt_isr_handle_t handle;
static portMUX_TYPE rmt_spinlock = portMUX_INITIALIZER_UNLOCKED;
extern portMUX_TYPE mixerMux; 
volatile uint32_t nextMixerEndTime = 0;
#define SCHEDULE_MIXER_END(delay) nextMixerEndTime = esp_timer_get_time()/1000 + (delay)

void IRAM_ATTR setupPulsesPPM(uint8_t proto)
{
  // Total frame length is a fixed 22.5msec (more than 9 channels is non-standard and requires this to be extended.)
  // Each channel's pulse is 0.7 to 1.7ms long, with a 0.3ms stop tail, making each compelte cycle 1 to 2ms.

  int16_t PPM_range = g_model.extendedLimits ? 640*2 : 512*2;   //range of 0.7..1.7msec

  //The pulse tick is 2mhz that's why everything is multiplied by 2
  uint8_t p = (proto == PROTO_PPM16 ? 16 : 8) + (g_model.ppmNCH * 2); //Channels *2
  uint16_t q = (g_model.ppmDelay*50+300)*2; // Stoplen *2
  int32_t rest = 22500u*2 - q;
  rest += (int32_t(g_model.ppmFrameLength))*1000;

  uint16_t pulseLevel;
  uint16_t idleLevel;
  if(g_model.pulsePol){
      pulseLevel=1;
      idleLevel=0;
  } else {
      pulseLevel=0;
      idleLevel=1;
  }
  vTaskEnterCritical(&mixerMux);
  portENTER_CRITICAL(&rmt_spinlock);
  int j=0;
  volatile rmt_item32_t* pd = RMTMEM.chan[PPM_OUT_RMT_CHANNEL_0].data32;
  for ( uint8_t i=(proto==PROTO_PPM16) ? p-8 : 0; i<p; i++) {
    int16_t v = limit((int16_t)-PPM_range, channelOutputs[i], (int16_t)PPM_range) + 2*PPM_CH_CENTER(i);
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
  portEXIT_CRITICAL(&rmt_spinlock);
  vTaskExitCritical(&mixerMux);
  rmt_set_tx_thr_intr_en(PPM_OUT_RMT_CHANNEL_0, true, j); //Send interrupt SETUP_PULSES_DURATION before the end of the PPM packet
}


void IRAM_ATTR setupPulses()
{
  uint8_t required_protocol = g_model.protocol;

  if (s_pulses_paused) {
    required_protocol = PROTO_NONE;
  }


  if (s_current_protocol[0] != required_protocol) {

#if defined(DSM2_SERIAL) && defined(TELEMETRY_FRSKY)
    if (s_current_protocol[0] == 255 || IS_DSM2_PROTOCOL(s_current_protocol[0])) {
      telemetryInit();
    }
#endif
    rmt_tx_start(PPM_OUT_RMT_CHANNEL_0,true);
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

      case PROTO_PPM16: // Not currently implemented
        setupPulsesPPM(PROTO_PPM16);
        break;

      case PROTO_PPMSIM:
        setupPulsesPPM(PROTO_PPMSIM);
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

    default: // standard PPM protocol
      SCHEDULE_MIXER_END(g_model.ppmFrameLength-SETUP_PULSES_DURATION/2000);//ms
      setupPulsesPPM(PROTO_PPM);
      break;
  }
}


static void IRAM_ATTR rmt_driver_isr_PPM(void* arg)
{
    uint32_t intr_st = RMT.int_st.val;
    if(intr_st & BIT(PPM_OUT_RMT_CHANNEL_0+24)){
       setupPulses();
       RMT.int_clr.val = intr_st & (BIT(PPM_OUT_RMT_CHANNEL_0) | 
            BIT(PPM_OUT_RMT_CHANNEL_0+1)| BIT(PPM_OUT_RMT_CHANNEL_0+2)| BIT(PPM_OUT_RMT_CHANNEL_0+24));
    }
}



void startPulses()
{
    rmt_config_t config;
    config.rmt_mode = RMT_MODE_TX;
    config.channel = PPM_OUT_RMT_CHANNEL_0;
    config.gpio_num = (gpio_num_t)PPM_TX_GPIO;
    config.mem_block_num = 1;
    config.tx_config.loop_en = 1;
    // enable the carrier to be able to hear the Morse sound
    // if the RMT_TX_GPIO is connected to a speaker
    config.tx_config.carrier_en = 0;
    //config.tx_config.idle_output_en = 0;
    //config.tx_config.idle_level = 0;
    //config.tx_config.carrier_duty_percent = 50;
    // set audible career frequency of 611 Hz
    // actually 611 Hz is the minimum, that can be set
    // with current implementation of the RMT API
    //config.tx_config.carrier_freq_hz = 611;
    //config.tx_config.carrier_level = 1;
    // set the maximum clock divider to be able to output
    // RMT pulses in range of about one hundred milliseconds
    config.clk_div = 40; //2MHz tick

    ESP_ERROR_CHECK(rmt_config(&config));
    rmt_isr_register(rmt_driver_isr_PPM, NULL, ESP_INTR_FLAG_IRAM, &handle );
    setupPulses();
    rmt_tx_start(PPM_OUT_RMT_CHANNEL_0,true);
}




