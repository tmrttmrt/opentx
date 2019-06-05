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
#include "driver/i2c.h"
#include "soc/gpio_struct.h"
#include "driver/gpio.h"
#include "esp_log.h"
#define HASASSERT
#include "opentx.h"

#define I2C_MASTER_NUM             I2C_NUM_1        /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ         1000000          /*!< I2C master clock frequency */

#define OLED_ADDRESS    0x3C

#define ACK_CHECK_EN                       0x1              /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS                      0x0              /*!< I2C master will not check ack from slave */
#define ACK_VAL                            0x0              /*!< I2C ack value */
#define NACK_VAL                           0x1              /*!< I2C nack value */

static const char *TAG = "lcd_driver_i2c.cpp";


void i2c_master_init()
{
	i2c_config_t conf;
  memset(&conf, 0, sizeof(conf));
	conf.mode = I2C_MODE_MASTER;
	conf.sda_io_num = I2C_DISPLAY_SDA_GPIO;
	conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
	conf.scl_io_num = I2C_DISPLAY_SCL_GPIO;
	conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
	conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
	i2c_param_config(I2C_MASTER_NUM, &conf);
	ESP_ERROR_CHECK_WITHOUT_ABORT( i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0) );
}

void _i2cWrite(uint8_t reg, uint8_t *data, uint32_t size)
{
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, OLED_ADDRESS << 1 | I2C_MASTER_WRITE, ACK_CHECK_EN);
	i2c_master_write_byte(cmd, reg, ACK_CHECK_EN);
	i2c_master_write(cmd, data, size, ACK_CHECK_EN);
	i2c_master_stop(cmd);
	//i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
	i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 0);
	i2c_cmd_link_delete(cmd);
}

void _sendCommand(uint8_t *data, uint32_t size)
{
	_i2cWrite(0x00, data, size);
}

void _sendData(uint8_t *data, uint32_t size)
{
	_i2cWrite(0x40, data, size);
}

void _set_display_on_off(bool state)
{
	uint8_t command[1] = { (uint8_t)(0xae | (uint8_t)state) };
	_sendCommand(command, sizeof(command));
}

void _set_multiplex_ratio(uint8_t ratio)
{
	uint8_t command[2] = { 0xa8, ratio };
	_sendCommand(command, sizeof(command));
}

void _set_display_offset(uint8_t offset)
{
	uint8_t command[2] = { 0xd3, offset };
	_sendCommand(command, sizeof(command));
}

void _set_display_start_line(uint8_t line)
{
	uint8_t command[1] = { (uint8_t)(0x40 | line) };
	_sendCommand(command, sizeof(command));
}

void _set_segment_remap(uint8_t remap)
{
	uint8_t command[1] = { (uint8_t)(0xa0 | remap) };
	_sendCommand(command, sizeof(command));
}

void _set_com_scan_direction(uint8_t dir)
{
	uint8_t command[1] = { (uint8_t)(0xc0 | dir << 3) };
	_sendCommand(command, sizeof(command));
}

void _set_com_pins_configuration(uint8_t conf, uint8_t remap)
{
	uint8_t command[2] = { 0xda, (uint8_t)(0x02 | (conf << 4) | (remap << 5)) };
	_sendCommand(command, sizeof(command));
}

void _set_contrast_control(uint8_t contrast)
{
	uint8_t command[2] = { 0x81, contrast };
	_sendCommand(command, sizeof(command));
}

void _entire_display_on(uint8_t status)
{
	uint8_t command[1] = { (uint8_t)(0xa4 | status) };
	_sendCommand(command, sizeof(command));
}

void _set_inverse_display(uint8_t inverse)
{
	uint8_t command[1] = { (uint8_t)(0xa6 | inverse) };
	_sendCommand(command, sizeof(command));
}

void _set_display_clock(uint8_t divider, uint8_t freq)
{
	uint8_t command[2] = { 0xd5, (uint8_t)(freq << 4 | divider) };
	_sendCommand(command, sizeof(command));
}

void _charge_pump_settings(uint8_t on)
{
	uint8_t command[2] = { 0x8d, (uint8_t)(0x10 | (on << 2)) };
	_sendCommand(command, sizeof(command));
}

void _set_memory_addressing_mode(uint8_t mode)
{
	uint8_t command[2] = { 0x20, mode };
	_sendCommand(command, sizeof(command));
}

void _set_column_address(uint8_t start, uint8_t end)
{
	uint8_t command[3] = { 0x21, start, end };
	_sendCommand(command, sizeof(command));
}

void _set_page_address(uint8_t start, uint8_t end)
{
	uint8_t command[3] = { 0x22, start, end };
	_sendCommand(command, sizeof(command));
}

void oledInitialize(void)
{
	/* Send initialization */
	_set_display_on_off(false);
	_set_multiplex_ratio(63);
	_set_display_offset(0);
	_set_display_start_line(0);

	_set_segment_remap(1);

	_set_com_pins_configuration(1, 0);
	_set_com_scan_direction(1);

	_set_contrast_control(0x7f);
	_entire_display_on(0);
	_set_inverse_display(0);
	_set_display_clock(0, 15);
	_charge_pump_settings(1);
	_set_display_on_off(true);

	_set_memory_addressing_mode(0);
	_set_column_address(0, 127);
	_set_page_address(0, 7);
}

void lcdSetRefVolt(uint8_t val)
{
}

void lcdRefresh()
{
  uint8_t * p = displayBuf;
  for (uint8_t y=0; y < 8; y++) {
      _set_column_address(0, 127);
      _set_page_address(y, 7);
      _sendData(p,LCD_W);
      p+=LCD_W;
  }
}

void lcdInit()
{
    ESP_LOGI(TAG,"lcdInit()");
    i2c_master_init();
    gpio_set_direction(I2C_DISPLAY_RESET_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_level(I2C_DISPLAY_RESET_GPIO, 0);
    vTaskDelay(100 / portTICK_RATE_MS);
    gpio_set_level(I2C_DISPLAY_RESET_GPIO, 1);
    vTaskDelay(100 / portTICK_RATE_MS);
    oledInitialize();
}