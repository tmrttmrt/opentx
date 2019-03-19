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

 
#ifndef _BOARD_ESP_WROOM_32_H_
#define _BOARD_ESP_WROOM_32_H_

#include "../common/esp32/board_esp32.h"

// Stubs
#define wdt_reset()
#define wdt_enable(x)
#define wdt_disable()
#define audioDefevent(a)

void tasksStart();
void boardInit(void);
void readKeysAndTrims();
uint16_t getBatteryVoltage();   // returns current battery voltage in 10mV steps

#define boardOff()  pwrOff()

// ADC driver
#define NUM_POTS                       2
#define NUM_SLIDERS                    0
#define NUM_XPOTS                      0
#define ADC_CHAN {ADC1_CHANNEL_0,ADC1_CHANNEL_3,ADC1_CHANNEL_6,ADC1_CHANNEL_7,ADC1_CHANNEL_4,ADC1_CHANNEL_5,(adc1_channel_t)ADC2_CHANNEL_8}
#define NUM_ADC2 1
#define ADC1_NAVG 20


#define TIMERS 2

// Switches driver
#define INP_ID2                 7
#define INP_ID1                 6
#define INP_AileDR              0
#define INP_ThrCt               3
#define INP_Gear                4
#define INP_RuddDR              2
#define INP_ElevDR              1
#define INP_Trainer             5

enum EnumSwitches
{
  SW_ID0,
  SW_ID1,
  SW_ID2,
  SW_THR,
  SW_RUD,
  SW_ELE,
  SW_AIL,
  SW_GEA,
  SW_TRN,
};
#define IS_3POS(sw)                    ((sw) == 0)
#define IS_TOGGLE(sw)                  ((sw) == SWSRC_TRN)
uint8_t keyState(uint8_t index);
uint8_t switchState(uint8_t index);

// LCD driver
#define LCD_W                          128
#define LCD_H                          64
#define LCD_DEPTH                      1
#define LCD_CONTRAST_MIN               10
#define LCD_CONTRAST_MAX               45
#define LCD_CONTRAST_DEFAULT           25
void lcdRefresh(void);
#define lcdRefreshWait()
void lcdSetRefVolt(unsigned char val);
void lcdSetContrast(void);
void lcdInit(void);
#define lcdOff()

// DBLKeys driver
#define KEYS_PRESSED()            0//(~PINL)

// Power driver (none)
#define pwrCheck()                 (e_power_on)
#define pwrOff()
#if defined(PWRMANAGE)
  #define UNEXPECTED_SHUTDOWN()   0//((mcusr & (1 << WDRF)) || g_eeGeneral.unexpectedShutdown)
#else
  #define UNEXPECTED_SHUTDOWN()   0//(mcusr & (1 << WDRF))
#endif

//PPM
//#define INP_E_PPM_IN              4
#define PPM_TX_GPIO               15

// Trims
#define NUM_TRIMS                 4
#define NUM_TRIMS_KEYS            (NUM_TRIMS * 2)

#define SLAVE_MODE()              0 //~PINH & (1<<INP_H_RF_Activated)
#define JACK_PPM_OUT()            0 //PORTB &= ~(1<<OUT_B_SIM_CTL)
#define JACK_PPM_IN()             0 //PORTB |= (1<<OUT_B_SIM_CTL)

// Backlight driver
#define BACKLIGHT_ENABLE()        backlightEnable()
#define BACKLIGHT_DISABLE()       backlightDisable()
#define BACKLIGHT_ON 1

// Rotary encoders driver
#define INP_J_ROT_ENC_1_PUSH      0
#define INP_J_ROT_ENC_2_PUSH      4
#define REA_DOWN()                rEncDown(INP_J_ROT_ENC_1_PUSH)
#define REB_DOWN()                rEncDown(INP_J_ROT_ENC_2_PUSH)
#define ROTENC_DOWN()             (REA_DOWN() || REB_DOWN())
//#define ROTENC_DIV2                 // rotary encoders resolution/2

#define IS_SHIFT_KEY(index)       (false)
#define IS_SHIFT_PRESSED()        (false)

// Keys driver
#define NUM_SWITCHES                   7
enum EnumKeys
{
  KEY_MENU,
  KEY_ENTER=KEY_MENU,
  KEY_EXIT,
  KEY_DOWN,
  KEY_MINUS = KEY_DOWN,
  KEY_UP,
  KEY_PLUS = KEY_UP,
  KEY_RIGHT,
  KEY_LEFT,
  
  TRM_BASE,
  TRM_LH_DWN = TRM_BASE,
  TRM_LH_UP,
  TRM_LV_DWN,
  TRM_LV_UP,
  TRM_RV_DWN,
  TRM_RV_UP,
  TRM_RH_DWN,
  TRM_RH_UP,
  TRM_LAST = TRM_RH_UP,

#if ROTARY_ENCODERS > 0 || defined(ROTARY_ENCODER_NAVIGATION)
  BTN_REa,
#endif
#if ROTARY_ENCODERS > 0
  BTN_REb,
#endif
  
  NUM_KEYS
};



enum CalibratedAnalogs {
  CALIBRATED_STICK1,
  CALIBRATED_STICK2,
  CALIBRATED_STICK3,
  CALIBRATED_STICK4,
  CALIBRATED_POT_FIRST,
  CALIBRATED_POT1 = CALIBRATED_POT_FIRST,
  CALIBRATED_POT2,
  CALIBRATED_POT_LAST = CALIBRATED_POT2,
  NUM_CALIBRATED_ANALOGS
};

// Buzzer driver
#define buzzerOn()                //PORTH |=  (1 << OUT_H_SpeakerBuzzer)
#define buzzerOff()               //PORTH &= ~(1 << OUT_H_SpeakerBuzzer)

// Speaker driver
#if defined(AUDIO)
#define AUD_DAC_GPIO   26
#define speakerOn()               //TCCR4A |=  (1 << COM4A0)
#define speakerOff()              //TCCR4A &= ~(1 << COM4A0)
#define VOLUME_LEVEL_MAX  255
#endif

// Battery driver
#define BATTERY_MIN                90  // 9V
#define BATTERY_MAX                120 // 12V
#define BATTERY_WARN               90  // 9V
#define BATT_SCALE                    150
#define BAT_AVG_SAMPLES       8

// Analogs driver
#define NUM_MOUSE_ANALOGS          0
#define NUM_DUMMY_ANAS             0
#endif