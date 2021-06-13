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

#include QMK_KEYBOARD_H
#ifdef CONFIG_MENU_ENABLE
#   include "features/config_menu/menu.h"
#endif
#ifdef PONG_ENABLE
#   include "games/pong/pong.h"
#endif

#define MEDIA_KEY_DELAY 2

void encoder_update_user_left(bool clockwise, uint8_t mods) {
    bool    is_ctrl  = mods & MOD_MASK_CTRL;
    bool    is_shift = mods & MOD_MASK_SHIFT;
    if (is_shift) {
        if (clockwise) {
            rgblight_increase_hue();
        } else {
            rgblight_decrease_hue();
        }
    } else if (is_ctrl) {
         if (clockwise) {
            rgblight_increase_val();
        } else {
            rgblight_decrease_val();
        }
    } else {
        if (clockwise) {
            tap_code16(KC_MS_WH_UP);
        } else {
            tap_code16(KC_MS_WH_DOWN);
        }
    }

}

void encoder_update_user_right(bool clockwise, uint8_t mods) {
    bool    is_ctrl  = mods & MOD_MASK_CTRL;
    bool    is_shift = mods & MOD_MASK_SHIFT;
    if (is_shift) {
            if (clockwise) {
                rgblight_increase_sat();
            } else {
                rgblight_decrease_sat();
            }
    } else if (is_ctrl) {
        if (clockwise) {
            rgblight_increase_speed();
        } else {
            rgblight_decrease_speed();
         }
    } else {
        uint16_t held_keycode_timer = timer_read();
        uint16_t mapped_code        = 0;
        if (clockwise) {
            mapped_code = KC_VOLD;
         } else {
            mapped_code = KC_VOLU;
        }
        register_code(mapped_code);
        while (timer_elapsed(held_keycode_timer) < MEDIA_KEY_DELAY)
            ; /* no-op */
        unregister_code(mapped_code);
    }

}

void encoder_update_user(int8_t index, bool clockwise) {
    uint8_t mods = get_mods() | get_oneshot_mods();
    switch (get_display_state()) {
#ifdef CONFIG_MENU_ENABLE
        case MENU_CONFIG:  // catch the case where we are in "config" mode
            process_encoder_menu(index, clockwise);
            break;
#endif
#ifdef PONG_ENABLE
        case PONG:
            process_encoder_pong(index, clockwise);
            break;
#endif
        default:
            if (index == 0) { /* First encoder */
                encoder_update_user_left(clockwise, mods);
            }
            else
            {
                encoder_update_user_right(clockwise, mods);
            }
            break;
    }

}
