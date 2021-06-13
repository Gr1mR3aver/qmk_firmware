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

//#define MAX_MENUS 8

typedef enum _menu_flags_t {
    menu_flag_is_parent = (1 << 0),
    menu_flag_is_value  = (1 << 1),
} menu_flags_t;

typedef enum _menu_input_t {
    menu_input_exit,
    menu_input_back,
    menu_input_enter,
    menu_input_up,
    menu_input_down,
    menu_input_left,
    menu_input_right,
} menu_input_t;

typedef struct _menu_entry_t {
    menu_flags_t flags;
    const char * text;
    union {
        struct {
            struct _menu_entry_t *children;
            size_t                child_count;
        } parent;
        struct {
            bool (*menu_handler)(menu_input_t input);
            void (*display_handler)(char *text_buffer, size_t buffer_len);
        } child;
    };
} menu_entry_t;

typedef struct _menu_state_t {
    bool    dirty;
    bool    is_in_menu;
    uint8_t selected_child;
    uint8_t menu_stack[8];
} menu_state_t;

void post_init_menu(void);
bool process_record_menu(uint16_t keycode, keyrecord_t *record);
void process_encoder_menu(int8_t index, bool clockwise);
void render_menu(void);
void set_menu_dirty_bit(bool is_dirty);
void keyboard_post_init_menu(void);
void rpc_sync_menu_state(void);

//////////////////////////////////////////////////////
// Enable other components to add sub-menus

bool register_menu_entry(char* menu_name_buf, size_t menu_name_buf_len, menu_entry_t menu);

