/* Copyright © 2021 Paul Cotter (@gr1mr3aver)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "wing70_common_config.h"

// Matrix
#define MATRIX_ROW_PINS \
    { A10, A15, B3, B4, B5, B9 }
#define MATRIX_COL_PINS \
    { A9, A8, B15, B14, B13, B12 }
// Encoders
#define ENCODERS_PAD_A \
    { A3 }
#define ENCODERS_PAD_B \
    { B10 }

// Split configuration
#define SERIAL_USART_DRIVER SD1
#define SERIAL_USART_TX_PAL_MODE 7
#define SOFT_SERIAL_PIN B6
#define SERIAL_USART_SPEED 921600
#define SPLIT_HAND_PIN B2
#define SPLIT_USB_DETECT
#define SPLIT_USB_TIMEOUT 1000
#define SPLIT_TRANSPORT_MIRROR
#define SPLIT_MODS_ENABLE

#define SPLIT_TRANSPORT_MIRROR
#define SPLIT_LAYER_STATE_ENABLE
#define SPLIT_LED_STATE_ENABLE
#define SPLIT_WPM_ENABLE

//I2C configuration
// USE DEFAULTS

// EEPROM Settings
#define EXTERNAL_EEPROM_SPI_SLAVE_SELECT_PIN A4
#define EXTERNAL_EEPROM_BYTE_COUNT 32768
#define EXTERNAL_EEPROM_PAGE_SIZE 1
#define EXTERNAL_EEPROM_SPI_CLOCK_DIVISOR 4

// SPI Configuration
#define SPI_DRIVER SPID1
#define SPI_SCK_PIN A5
#define SPI_SCK_PAL_MODE 5// check datasheet
#define SPI_MOSI_PIN A7
#define SPI_MOSI_PAL_MODE 5 // check datasheet
#define SPI_MISO_PIN A6
#define SPI_MISO_PAL_MODE 5// check datasheet

// LCD Configuration
#define LCD_RST_PIN B1
#define LCD_CS_PIN A2
#define LCD_DC_PIN A0
#ifndef LCD_ACTIVITY_TIMEOUT
#    define LCD_ACTIVITY_TIMEOUT 10 // seconds of keyboard inactivity before powering down the screens
#endif
#define LCD_SPI_CLOCK_DIVISOR 2

// Backlight driver (to control LCD backlight)
#define BACKLIGHT_PIN B8
#define BACKLIGHT_PWM_DRIVER PWMD4
#define BACKLIGHT_PWM_CHANNEL 3
#define BACKLIGHT_PAL_MODE 2

// RGB configuration
#ifdef SPLIT_KEYBOARD
#    define RGBLED_NUM 60
#    define RGBLED_SPLIT { 30,30 }
#else
#    define RGBLED_NUM 60
#endif

#define WS2812_EXTERNAL_PULLUP
#define RGB_DI_PIN B0
#define WS2812_PWM_DRIVER PWMD3
#define WS2812_PWM_CHANNEL 3
#define WS2812_PWM_PAL_MODE 2
#define WS2812_DMA_STREAM STM32_DMA1_STREAM2
#define WS2812_DMA_CHANNEL 5
#define WS2812_TRST_US 80


