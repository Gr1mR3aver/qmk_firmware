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
#include <stdint.h>
#include "wing70.h"

typedef enum _display_state {
    INIT,
    LOGO,
#ifdef PONG_ENABLE
    PONG,
#endif
#ifdef SPACE_INVADERS_ENABLE
    SPACE_INVADERS,
#endif
#ifdef CATERPILLAR_ENABLE
    CATERPILLAR,
#endif
    MENU_CONFIG,
    WPM,
    MOD_STATE
} display_state;

typedef struct _render_loc_t {
    uint16_t x, y;
} render_loc_t;

typedef struct _lcd_state_t {
    display_state current_state;
    display_state previous_state;
    bool transition_complete;
} lcd_state_t;

void revert_display_state(void);
bool update_display_state(lcd_state_t *lcd_state);
void set_display_state(display_state new_state);
display_state get_display_state(void);
void keyboard_post_init_display(void);
void housekeeping_task_render_display(void);
void rpc_sync_display_state(void);
