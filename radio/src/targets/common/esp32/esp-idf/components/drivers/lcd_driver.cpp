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
#include "esp_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "soc/gpio_struct.h"
#include "driver/gpio.h"
#include "esp_log.h"
#define HASASSERT
#include "opentx.h"

static const char *TAG = "lcd_driver.cpp";
spi_device_handle_t spi;

void backlightEnable()
{
    gpio_set_level(LCD_PIN_NUM_BCKL, BACKLIGHT_ON?1:0);
}

void backlightDisable()
{
    gpio_set_level(LCD_PIN_NUM_BCKL, BACKLIGHT_ON?0:1);
}

bool isBacklightEnabled()
{
    return gpio_get_level (LCD_PIN_NUM_BCKL)==BACKLIGHT_ON?true:false;
}

void lcd_spi_pre_transfer_callback(spi_transaction_t *t)
{
    int dc=(int)t->user;
    gpio_set_level(LCD_PIN_NUM_DC, dc);
}

void lcdSendCtl(const uint8_t cmd)
{
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));       //Zero out the transaction
    t.length=8;                     //Command is 8 bits
    t.tx_buffer=&cmd;               //The data is the cmd itself
    t.user=(void*)0;                //D/C needs to be set to 0
    ret=spi_device_transmit(spi, &t);  //Transmit!
    assert(ret==ESP_OK);            //Should have had no issues.
}

const static uint8_t lcdInitSequence[] = {
    //ST7565 eq. : KS0713, SED1565, S6B1713, SPLC501C, NT7532 /34 /38, TL03245
#if defined(LCD_ST7565R)
    0xE2, //Initialize the internal functions
    0xAE, //DON = 0: display OFF
    0xA0, //ADC = 0: normal direction (SEG132->SEG1)
    0xA6, //REV = 0: non-reverse display
    0xA4, //EON = 0: normal display. non-entire
    0xA2, //Select LCD bias
    0xC8, //SHL = 1: reverse direction (COM64->COM1)
    0x2F, //Control power circuit operation VC=VR=VF=1
    0x25, //Select int resistance ratio R2 R1 R0 =5
    0x81, //Set reference voltage Mode
    0x22, //24 SV5 SV4 SV3 SV2 SV1 SV0 = 0x18
    0xAF, //DON = 1: display ON
    0x60  //Set the display start line to zero
#elif defined(LCD_ERC12864FSF)
    0xE2, //Initialize the internal functions
    0xAE, //DON = 0: display OFF
    0xA1, //ADC = 1: reverse direction (SEG132->SEG1)
    0xA6, //REV = 0: non-reverse display
    0xA4, //EON = 0: normal display. non-entire
    0xA3, //Select LCD bias
    0xC0, //SHL = 0: normal direction (COM1->COM64)
    0x2F, //Control power circuit operation VC=VR=VF=1
    0x27, //Select int resistance ratio R2 R1 R0
    0x81, //Set reference voltage Mode
    0x2D, //24 SV5 SV4 SV3 SV2 SV1 SV0
    0xAF  //DON = 1: display ON
#else    //ST7565P (default 9x LCD)
    0xE2, //Initialize the internal functions
    0xAE, //DON = 0: display OFF
    0xA1, //ADC = 1: reverse direction(SEG132->SEG1)
    0xA6, //REV = 0: non-reverse display
    0xA4, //EON = 0: normal display. non-entire
    0xA2, //Select LCD bias=0
    0xC0, //SHL = 0: normal direction (COM1->COM64)
    0x2F, //Control power circuit operation VC=VR=VF=1
    0x25, //Select int resistance ratio R2 R1 R0 =5
    0x81, //Set reference voltage Mode
    0x22, //24 SV5 SV4 SV3 SV2 SV1 SV0 = 0x18
    0xAF  //DON = 1: display ON
#endif
};

void dispInit()
{
    LCD_LOCK();
    gpio_set_level(LCD_PIN_NUM_RST, 0);
    vTaskDelay(100 / portTICK_RATE_MS);
    gpio_set_level(LCD_PIN_NUM_RST, 1);
    vTaskDelay(100 / portTICK_RATE_MS);

    for (uint8_t i=0; i<DIM(lcdInitSequence); i++) {
        lcdSendCtl(lcdInitSequence[i]) ;
    }
#if defined(LCD_ERC12864FSF)
    g_eeGeneral.contrast = 0x2D;
#else
    g_eeGeneral.contrast = 0x22;
#endif
    LCD_UNLOCK();
}

void lcdSetRefVolt(uint8_t val)
{
    LCD_LOCK();
    lcdSendCtl(0x81);
    lcdSendCtl(val);
    LCD_UNLOCK();
}

void lcdSendData( const uint8_t *data, int len)
{
    esp_err_t ret;
    spi_transaction_t t;
    if (len==0) return;             //no need to send anything
    memset(&t, 0, sizeof(t));       //Zero out the transaction
    t.length=len*8;                 //Len is in bytes, transaction length is in bits.
    t.tx_buffer=data;               //Data
    t.user=(void*)1;                //D/C needs to be set to 1
    ret=spi_device_transmit(spi, &t);  //Transmit!
    assert(ret==ESP_OK);            //Should have had no issues.
}

void lcdInit()
{
    ESP_LOGI(TAG,"lcdInit()");
    esp_err_t ret;
    spi_bus_config_t buscfg;
    memset(&buscfg, 0, sizeof(buscfg));
    buscfg.miso_io_num=LCD_PIN_NUM_MISO;
    buscfg.mosi_io_num=LCD_PIN_NUM_MOSI;
    buscfg.sclk_io_num=LCD_PIN_NUM_CLK;
    buscfg.quadwp_io_num=-1;
    buscfg.quadhd_io_num=-1;
    buscfg.max_transfer_sz=LCD_W;
    spi_device_interface_config_t devcfg;
    memset(&devcfg, 0, sizeof(devcfg));
    devcfg.clock_speed_hz=10*1000*1000;           //Clock out at 10 MHz
    devcfg.mode=0;                                //SPI mode 0
    devcfg.spics_io_num=LCD_PIN_NUM_CS;               //CS pin
    devcfg.queue_size=7;                          //We want to be able to queue 7 transactions at a time
    devcfg.pre_cb=lcd_spi_pre_transfer_callback;  //Specify pre-transfer callback to handle D/C line

    //Initialize the SPI bus
    ret=spi_bus_initialize(HSPI_HOST, &buscfg, 1);
    ESP_ERROR_CHECK(ret);
    //Attach the LCD to the SPI bus
    ret=spi_bus_add_device(HSPI_HOST, &devcfg, &spi);
    ESP_ERROR_CHECK(ret);
    //Initialize the LCD

    //Initialize non-SPI GPIOs
    gpio_set_direction(LCD_PIN_NUM_DC, GPIO_MODE_OUTPUT);
    gpio_set_direction(LCD_PIN_NUM_RST, GPIO_MODE_OUTPUT);
    gpio_set_direction(LCD_PIN_NUM_BCKL, GPIO_MODE_OUTPUT);

    dispInit();
}


void lcdRefresh()
{
    LCD_LOCK();
    uint8_t * p = displayBuf;
    for (uint8_t y=0; y < 8; y++) {
#if defined(LCD_ST7565R)
        lcdSendCtl(0x01);
#else
        lcdSendCtl(0x04);
#endif
        lcdSendCtl(0x10); // Column addr 0
        lcdSendCtl( y | 0xB0); //Page addr y
        lcdSendData(p,LCD_W);
        p+=LCD_W;
    }
    LCD_UNLOCK();
}

