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

#include "board_esp32.h"

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
#define STORAGE_NUM_POTS               2
#define NUM_SLIDERS                    0
#define STORAGE_NUM_SLIDERS            0
#define NUM_XPOTS                      0
#define STORAGE_NUM_MOUSE_ANALOGS      0
#define ADC_CHAN {ADC1_CHANNEL_0,ADC1_CHANNEL_3,ADC1_CHANNEL_6,ADC1_CHANNEL_7,ADC1_CHANNEL_4,ADC1_CHANNEL_5,ADC1_CHANNEL_2}
#define NUM_ADC2 0
#define ADC1_NAVG 20


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
#define LCD_PIN_NUM_MISO -1

//#define LCD_PIN_NUM_MOSI GPIO_NUM_12
//#define LCD_PIN_NUM_CLK  GPIO_NUM_14
//#define LCD_PIN_NUM_CS   GPIO_NUM_13
//#define LCD_PIN_NUM_DC   GPIO_NUM_27
//#define LCD_PIN_NUM_RST  GPIO_NUM_0
//#define LCD_PIN_NUM_BCKL GPIO_NUM_2

#define I2C_DISPLAY_SCL_GPIO          GPIO_NUM_15
#define I2C_DISPLAY_SDA_GPIO          GPIO_NUM_4
#define I2C_DISPLAY_RESET_GPIO        GPIO_NUM_16

void lcdRefresh(void);
#define lcdRefreshWait()
void lcdSetRefVolt(unsigned char val);
void lcdSetContrast(void);
void lcdInit(void);
#define backlightEnable()
#define backlightDisable()
#define isBacklightEnabled() true
#define lcdOff()
#define LCD_LOCK()
#define LCD_UNLOCK()

//SDCARD driver
#define SD_PIN_NUM_MISO GPIO_NUM_19
#define SD_PIN_NUM_MOSI GPIO_NUM_5
#define SD_PIN_NUM_CLK  GPIO_NUM_18
#define SD_PIN_NUM_CS   GPIO_NUM_14

// DBLKeys driver
#define KEYS_PRESSED()            0

// Power driver (none)
#define pwrCheck()                 (e_power_on)
#define pwrOff()
#if defined(PWRMANAGE)
  #define UNEXPECTED_SHUTDOWN()   0//((mcusr & (1 << WDRF)) || g_eeGeneral.unexpectedShutdown)
#else
  #define UNEXPECTED_SHUTDOWN()   0//(mcusr & (1 << WDRF))
#endif


// Pulses driver
void init_no_pulses(uint32_t port);
void init_ppm(uint32_t port);
void disable_ppm(uint32_t port);
void init_pxx1_pulses(uint32_t port);
void disable_pxx1_pulses(uint32_t port);
void disable_serial(uint32_t port);
void init_module_timer( uint32_t module_index, uint32_t period, uint8_t state);
void disable_module_timer( uint32_t module_index);
void extmoduleSerialStart(uint32_t baudrate, uint32_t period_half_us, bool inverted);
void extmoduleSendNextFrame();
void rfPwrOn();
void rfPwrOff();

//#define INP_E_PPM_IN              4
#define PPM_TX_GPIO             GPIO_NUM_12
#define RF_POWER_BIT            5

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
//#define INP_J_ROT_ENC_1_PUSH      0
//#define ROTARY_ENCODER_NAVIGATION

#define IS_SHIFT_KEY(index)       (false)
#define IS_SHIFT_PRESSED()        (false)

// Keys driver
#define NUM_SWITCHES                   7
//MCP23017
#define MCP23017_ADDR_KEYS  0x20
#define I2C_KEYS_SDA_GPIO GPIO_NUM_23
#define I2C_KEYS_SCL_GPIO GPIO_NUM_22

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
  
  KEY_COUNT,
  KEY_MAX = KEY_COUNT - 1,  

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

#define IS_POT(x)                      ((x)>=POT_FIRST && (x)<=POT_LAST)
#define STICKS_PWM_ENABLED()          (false)


// Speaker driver
#if defined(AUDIO)
#define AUD_DAC_GPIO   26
#define VOLUME_LEVEL_MAX  23
#define VOLUME_LEVEL_DEF  12
#endif

// Battery driver
#define BATTERY_MIN                90  // 9V
#define BATTERY_MAX                120 // 12V
#define BATTERY_WARN               90  // 9V

// Analogs driver
#define NUM_MOUSE_ANALOGS          0
#define NUM_DUMMY_ANAS             0

// Telemetry driver
#define TELE_TXD_GPIO (GPIO_NUM_17)
#define TELE_RXD_GPIO (GPIO_NUM_13)

  
#endif

