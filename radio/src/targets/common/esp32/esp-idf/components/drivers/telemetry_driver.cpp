/*
 * Copyright (C) OpenTX
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
 
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "esp_log.h"
#define HASASSERT
#include "opentx.h"

#define TXD_PIN (GPIO_NUM_17)
#define RXD_PIN (GPIO_NUM_16)
static const int BUF_SIZE = 128;
static const char *TAG = "telemetry_driver.cpp";

uint8_t telemetryGetByte(uint8_t * byte)
{
    int rxBytes = uart_read_bytes(UART_NUM_1, byte, 1, 0);
    if(rxBytes==1) {
        return 1;
    } else {
        return 0;
    }
}

void telemetryPortInit(uint32_t baudrate, uint8_t mode)
{
    ESP_LOGI(TAG,"telemetryPortInit");
    uart_config_t uart_config;
    memset(&uart_config, 0, sizeof(uart_config));

    uart_config.baud_rate = baudrate;
    if (mode & TELEMETRY_SERIAL_8E2) {
      ESP_LOGI(TAG,"%d,8,2",baudrate);
      uart_config.data_bits = UART_DATA_8_BITS;
      uart_config.parity = UART_PARITY_EVEN;
      uart_config.stop_bits = UART_STOP_BITS_2;
    }
    else {
      ESP_LOGI(TAG,"%d,8,1",baudrate);
      uart_config.data_bits = UART_DATA_8_BITS;
      uart_config.parity = UART_PARITY_DISABLE;
      uart_config.stop_bits = UART_STOP_BITS_1;
    }
    uart_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    // We won't use a buffer for sending data.
    uart_driver_install(UART_NUM_1, BUF_SIZE * 2, BUF_SIZE * 2, 0, NULL, 0);
}

void telemetryTransmitBuffer(uint8_t * data, uint8_t len)
{
    int txBytes = uart_write_bytes(UART_NUM_1,(char *) data, (size_t) len);
}
