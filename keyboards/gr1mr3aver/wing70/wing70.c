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

#include <string.h>
#include <hal.h>
#include <print.h>

#ifdef ENABLE_ADC_USBPD_CHECK
#    include <analog.h>
#endif

#include "wing70.h"



bool config_menu_displayed = false;

//----------------------------------------------------------
// Initialisation

void keyboard_post_init_kb(void) {

    debug_enable = true;

    // try to synchronise the startup screens across halves
    if (is_keyboard_master()) wait_ms(400);

    // Turn off the LCD backlight and blank the screen
    backlight_disable();

    // Let the LCD get some power...
    wait_ms(100);

    // Initialise the LCD
    lcd = qp_st7789_make_device(LCD_CS_PIN, LCD_DC_PIN, LCD_RST_PIN, LCD_SPI_CLOCK_DIVISOR, true);

    qp_init(lcd, QP_ROTATION_180);
    qp_rect(lcd, 0,0,LCD_WIDTH-1,LCD_HEIGHT-1,HSV_BLACK, true);

    dprint("---- Power on LCD ----");
    // Turn on the LCD
    qp_power(lcd, true);


    for (uint16_t i = 0; i < LCD_HEIGHT; i++)
    {
        qp_rect(lcd, 0, i, LCD_WIDTH-1, i+2, i % 256, 255, 255, true);
    }

    // Turn on the LCD backlight
    backlight_enable();

//    extern void keyboard_post_init_user(void);
    keyboard_post_init_user();
}


//----------------------------------------------------------
// QMK overrides

void housekeeping_task_kb(void) {
//    extern void housekeeping_task_user(void);
    housekeeping_task_user();
}

void encoder_update_kb(uint8_t index, bool clockwise) {
    // Offload to the keymap instead.
    extern void encoder_update_user(uint8_t index, bool clockwise);
    encoder_update_user(index, clockwise);
}

void suspend_power_down_kb(void) {
    suspend_power_down_user();
}

void suspend_wakeup_init_kb(void) {
    suspend_wakeup_init_user();
}

#ifdef RGB_MATRIX_ENABLE
    // clang-format off
    #define RLO 30
    #define LLI(x) (x)
    #define LLP(x,y) {(x),(y)}
    #define RLI(x) (RLO+(x))
    #define RLP(x,y) {(224-(x)),((y))}
    led_config_t g_led_config = {
        {
            // Key Matrix to LED Index
            { LLI(0),  LLI(1),  LLI(2),  LLI(3),  LLI(4),  LLI(5) },
            { LLI(11),  LLI(10),  LLI(9),  LLI(7),  LLI(7), LLI(6) },
            { LLI(12), LLI(13), LLI(14), LLI(15), LLI(16), LLI(17) },
            { LLI(23), LLI(22), LLI(21), LLI(20), LLI(19), LLI(18) },
            { LLI(24), LLI(25), LLI(26), LLI(27), LLI(28), LLI(29) },
            { NO_LED,  NO_LED,  NO_LED,  NO_LED,  NO_LED,  NO_LED },
            { RLI(0),  RLI(1),  RLI(2),  RLI(3),  RLI(4),  RLI(5) },
            { RLI(11),  RLI(10),  RLI(9),  RLI(7),  RLI(7), RLI(6) },
            { RLI(12), RLI(13), RLI(14), RLI(15), RLI(16), RLI(17) },
            { RLI(23), RLI(22), RLI(21), RLI(20), RLI(19), RLI(18) },
            { RLI(24), RLI(25), RLI(26), RLI(27), RLI(28), RLI(29) },
            { NO_LED,  NO_LED,  NO_LED,  NO_LED,  NO_LED,  NO_LED },        },
        {
            // LED Index to Physical Position
            // Matrix left
            LLP(  80,	   7), 	LLP(  64,	   5), 	LLP(  48,	   0), 	LLP(  32,	   4), 	LLP(  16,	   8), 	LLP(   0,	   8),
            LLP(   0,	  19), 	LLP(  16,	  19), 	LLP(  32,	  15), 	LLP(  48,	  11), 	LLP(  64,	  16), 	LLP(  80,	  18),
            LLP(  80,	  29), 	LLP(  64,	  27), 	LLP(  48,	  21), 	LLP(  32,	  26), 	LLP(  16,	  30), 	LLP(   0,	  30),
            LLP(   0,	  41), 	LLP(  16,	  41), 	LLP(  32,	  37), 	LLP(  48,	  32), 	LLP(  64,	  38), 	LLP(  80,	  40),
            LLP( 104,	  53),  LLP( 111,	  60), 	LLP(  96,	  64), 	LLP(  88,	  55), 	LLP(  64,	  50), 	LLP(  47,	  48),
            // Matrix right
            RLP(  80,	   7), 	RLP(  64,	   5), 	RLP(  48,	   0), 	RLP(  32,	   4), 	RLP(  16,	   8), 	RLP(   0,	   8),
            RLP(   0,	  19), 	RLP(  16,	  19), 	RLP(  32,	  15), 	RLP(  48,	  11), 	RLP(  64,	  16), 	RLP(  80,	  18),
            RLP(  80,	  29), 	RLP(  64,	  27), 	RLP(  48,	  21), 	RLP(  32,	  26), 	RLP(  16,	  30), 	RLP(   0,	  30),
            RLP(   0,	  41), 	RLP(  16,	  41), 	RLP(  32,	  37), 	RLP(  48,	  32), 	RLP(  64,	  38), 	RLP(  80,	  40),
            RLP( 104,	  53),  RLP( 111,	  60), 	RLP(  96,	  64), 	RLP(  88,	  55), 	RLP(  64,	  50), 	RLP(  47,	  48),

        },
        {
            // LED Index to Flag
            // Matrix left
            LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
            LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
            LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
            LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
            LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
            // Matrix right
            LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
            LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
            LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
            LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
            LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
        }
    };
// clang-format on
#endif  // RGB_MATRIX_ENABLE
