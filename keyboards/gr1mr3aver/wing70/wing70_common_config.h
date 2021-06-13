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

#include "config_common.h"


// USB Device parameters
#define VENDOR_ID 0x4752 // GR
#define PRODUCT_ID 0x5746 // 0x57 = W 0x46 = 70 (decimal)
#define MANUFACTURER "Gr1mR3aver"
#define DEVICE_VER 0x0001
#define PRODUCT "Wing70"
#define DESCRIPTION "Split 70-ish%% keyboard"

// 1000Hz poll rate
#define USB_POLLING_INTERVAL_MS 1

// Matrix
#ifdef SPLIT_KEYBOARD
#    define MATRIX_ROWS 12
#else
#    define MATRIX_ROWS 6
#endif
#define MATRIX_COLS 6

#ifdef RGB_MATRIX_ENABLE
#    define DRIVER_LED_TOTAL RGBLED_NUM
#    define RGB_MATRIX_SPLIT RGBLED_SPLIT
#endif  //  RGB_MATRIX_ENABLE
#define DRIVER_LED_TOTAL RGBLED_NUM
#define RGBLIGHT_ANIMATIONS
#define RGB_MATRIX_KEYPRESSES
#define RGB_MATRIX_FRAMEBUFFER_EFFECTS

// Display configuration
#define QUANTUM_PAINTER_COMPRESSED_CHUNK_SIZE 4096

#define DEBOUNCE 2
#define TAPPING_TERM 150

#ifndef ENCODER_RESOLUTION
#    define ENCODER_RESOLUTION 2
#endif  // ENCODER_RESOLUTION

// Debugging
#define DEBUG_MATRIX_SCAN_RATE

// Bootloader
#define BOOTMAGIC_LITE_ROW 0
#define BOOTMAGIC_LITE_COLUMN 5
#define BOOTMAGIC_LITE_ROW_RIGHT 6
#define BOOTMAGIC_LITE_COLUMN_RIGHT 5

// Diode direction
#define DIODE_DIRECTION ROW2COL

// Backlight config (to control LCD backlight)
#define BACKLIGHT_LEVELS 31

/* disable these deprecated features by default */
#define NO_ACTION_MACRO
#define NO_ACTION_FUNCTION

// Mouse keys config
#define MOUSEKEY_INTERVAL 16
#define MOUSEKEY_DELAY 0
#define MOUSEKEY_TIME_TO_MAX 75
#define MOUSEKEY_MAX_SPEED 5
#define MOUSEKEY_WHEEL_DELAY 0
