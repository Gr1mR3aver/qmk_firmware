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

#include QMK_KEYBOARD_H
#include "features/config_menu/menu.h"
#include "features/display/display.h"
#ifdef PONG_ENABLE
#   include "games/pong/pong.h"
#endif

enum {
    ALPHA,
    MEDIA,
    GRAVE,
    ACUTE,
    UMLAUT,
    EXTRA,
    DEVEL,
    SPECIAL,
    ADMIN,
    CONFIG
};

enum custom_keycodes {
    ACCENT_0 = SAFE_RANGE,
    ACCENT_1,
    ACCENT_2,
    ACCENT_3,
    ACCENT_4,
    ACCENT_5,
    ACCENT_6,
    ACCENT_7,
    ACCENT_8,
    ACCENT_9,
    ACCENT_10,
    ACCENT_11,
    ACCENT_12,
    ACCENT_13,
    ACCENT_14,
    ACCENT_15,
    ACCENT_16,
    ACCENT_17,
    ACCENT_18,
    CONFIG_MENU,
    SHOW_PONG,
    SHOW_LOGO
};

#pragma pack(push)
#pragma pack(1)

typedef struct _kb_system_state_t {
    unsigned lcd_power;
} kb_system_state_t;

#pragma pack(pop)


