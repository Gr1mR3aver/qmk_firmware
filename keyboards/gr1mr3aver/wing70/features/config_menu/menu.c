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
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <printf.h>

#include <quantum.h>
#include <qp.h>
#include <rgb_matrix.h>
#include "serial.h"
#include "menu.h"
#include "fonts/thintel15.h"
#include "keymap.h"


#include <process_keycode/process_unicode_common.h>

menu_state_t menu_state = {.dirty = false, .is_in_menu = false, .menu_stack = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, .selected_child = 0xFF};
uint8_t next_empty_menu = 2;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   Implementation of state sync between primary/secondary sides
// ----------------------------------------------------------------------

void rpc_update_menu_state(uint8_t initiator2target_buffer_size, const void* initiator2target_buffer, uint8_t target2initiator_buffer_size, void* target2initiator_buffer) {
    if (initiator2target_buffer_size == sizeof(menu_state_t)) {
        sprintf(target2initiator_buffer, "- updating menu state -\n");
        memcpy(&menu_state, initiator2target_buffer, sizeof(menu_state_t));
    } else {
        sprintf(target2initiator_buffer, "XXX invalid transaction size 0x%02x XXX\n", initiator2target_buffer_size);
    }
}

void rpc_sync_menu_state(void) {
    if (is_keyboard_master()) {
        // Keep track of the last state, so that we can tell if we need to propagate to secondary side
        static menu_state_t last_menu_state;
        static uint32_t          last_sync;
        bool                     needs_sync = false;

       // Check if the pong court state values are different
        if (memcmp(&menu_state, &last_menu_state, sizeof(menu_state_t))) {
            needs_sync = true;
            memcpy(&last_menu_state, &menu_state, sizeof(menu_state_t));
            menu_state.dirty = false;
        }

        // Send to secondary side periodically regardless of state change (period defined in config.h)
        if (timer_elapsed32(last_sync) > MAX_SYNC_INTERVAL_MS) {
            needs_sync = true;
        }

        // Perform the sync if requested
        if (needs_sync) {
            char result_string[RPC_S2M_BUFFER_SIZE];
                int result = transaction_rpc_exec(STATE_SYNC_USER_MENU, sizeof(menu_state_t), &menu_state, RPC_S2M_BUFFER_SIZE, result_string);
            if ( result) {
               last_sync = timer_read32();
            }
#ifdef CONSOLE_ENABLE
            else {
                dprintf("Failed to sync pong state with result 0x%04x\n", result);
            }
            dprint(result_string);
#endif
        }
    }

}

void set_menu_dirty_bit(bool is_dirty) {
    menu_state.dirty = is_dirty;
}

static bool menu_handler_unicode(menu_input_t input) {
    switch (input) {
        case menu_input_left:
            cycle_unicode_input_mode(-1);
            return false;
        case menu_input_right:
            cycle_unicode_input_mode(1);
            return false;
        default:
            return true;
    }
}

void display_handler_unicode(char *text_buffer, size_t buffer_len) {
    switch (get_unicode_input_mode()) {
        case UC_MAC:
            strncpy(text_buffer, "macOS", buffer_len - 1);
            return;
        case UC_LNX:
            strncpy(text_buffer, "Linux", buffer_len - 1);
            return;
        case UC_BSD:
            strncpy(text_buffer, "BSD", buffer_len - 1);
            return;
        case UC_WIN:
            strncpy(text_buffer, "Windows", buffer_len - 1);
            return;
        case UC_WINC:
            strncpy(text_buffer, "WinCompose", buffer_len - 1);
            return;
    }

    strncpy(text_buffer, "Unknown", buffer_len);
}

static bool menu_handler_rgbenabled(menu_input_t input) {
    switch (input) {
        case menu_input_left:
        case menu_input_right:
            rgb_matrix_toggle();
            return false;
        default:
            return true;
    }
}

void display_handler_rgbenabled(char *text_buffer, size_t buffer_len) { snprintf_(text_buffer, buffer_len - 1, "%s", rgb_matrix_is_enabled() ? "on" : "off"); }

static bool menu_handler_rgbmode(menu_input_t input) {
    switch (input) {
        case menu_input_left:
            rgb_matrix_step_reverse();
            return false;
        case menu_input_right:
            rgb_matrix_step();
            return false;
        default:
            return true;
    }
}

void display_handler_rgbmode(char *text_buffer, size_t buffer_len) { snprintf_(text_buffer, buffer_len - 1, "%d", (int)rgb_matrix_get_mode()); }

static bool menu_handler_rgbhue(menu_input_t input) {
    switch (input) {
        case menu_input_left:
            rgb_matrix_decrease_hue();
            return false;
        case menu_input_right:
            rgb_matrix_increase_hue();
            return false;
        default:
            return true;
    }
}

void display_handler_rgbhue(char *text_buffer, size_t buffer_len) { snprintf_(text_buffer, buffer_len - 1, "%d", (int)rgb_matrix_get_hue()); }

static bool menu_handler_rgbsat(menu_input_t input) {
    switch (input) {
        case menu_input_left:
            rgb_matrix_decrease_sat();
            return false;
        case menu_input_right:
            rgb_matrix_increase_sat();
            return false;
        default:
            return true;
    }
}

void display_handler_rgbsat(char *text_buffer, size_t buffer_len) { snprintf_(text_buffer, buffer_len - 1, "%d", (int)rgb_matrix_get_sat()); }

static bool menu_handler_rgbval(menu_input_t input) {
    switch (input) {
        case menu_input_left:
            rgb_matrix_decrease_val();
            return false;
        case menu_input_right:
            rgb_matrix_increase_val();
            return false;
        default:
            return true;
    }
}

void display_handler_rgbval(char *text_buffer, size_t buffer_len) { snprintf_(text_buffer, buffer_len - 1, "%d", (int)rgb_matrix_get_val()); }

static bool menu_handler_rgbspeed(menu_input_t input) {
    switch (input) {
        case menu_input_left:
            rgb_matrix_decrease_speed();
            return false;
        case menu_input_right:
            rgb_matrix_increase_speed();
            return false;
        default:
            return true;
    }
}

void display_handler_rgbspeed(char *text_buffer, size_t buffer_len) { snprintf_(text_buffer, buffer_len - 1, "%d", (int)rgb_matrix_get_speed()); }

menu_entry_t rgb_entries[] = {
    {
        .flags                 = menu_flag_is_value,
        .text                  = "RGB enabled",
        .child.menu_handler    = menu_handler_rgbenabled,
        .child.display_handler = display_handler_rgbenabled,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "RGB mode",
        .child.menu_handler    = menu_handler_rgbmode,
        .child.display_handler = display_handler_rgbmode,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "RGB hue",
        .child.menu_handler    = menu_handler_rgbhue,
        .child.display_handler = display_handler_rgbhue,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "RGB saturation",
        .child.menu_handler    = menu_handler_rgbsat,
        .child.display_handler = display_handler_rgbsat,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "RGB value",
        .child.menu_handler    = menu_handler_rgbval,
        .child.display_handler = display_handler_rgbval,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "RGB speed",
        .child.menu_handler    = menu_handler_rgbspeed,
        .child.display_handler = display_handler_rgbspeed,
    },
};

menu_entry_t root_entries[] = {
    {.flags = menu_flag_is_value, .text = "Unicode mode", .child.menu_handler = menu_handler_unicode, .child.display_handler = display_handler_unicode},
    {
        .flags              = menu_flag_is_parent,
        .text               = "RGB settings",
        .parent.children    = rgb_entries,
        .parent.child_count = sizeof(rgb_entries) / sizeof(rgb_entries[0]),
    },
};

menu_entry_t root = {
    .flags              = menu_flag_is_parent,
    .text               = "Configuration",
    .parent.children    = root_entries,
    .parent.child_count = sizeof(root_entries) / sizeof(root_entries[0]),
};


menu_entry_t *get_current_menu(void) {
    if (menu_state.selected_child == 0xFF) {
        return NULL;
    }

    menu_entry_t *entry = &root;
    for (int i = 0; i < sizeof(menu_state.menu_stack); ++i) {
        if (menu_state.menu_stack[i] == 0xFF) {
            return entry;
        }
        entry = &entry->parent.children[menu_state.menu_stack[i]];
    }

    return entry;
}

menu_entry_t *get_selected_menu_item(void) { return &(get_current_menu()->parent.children[menu_state.selected_child]); }

bool menu_handle_input(menu_input_t input) {
    menu_entry_t *menu     = get_current_menu();
    menu_entry_t *selected = get_selected_menu_item();
    menu_state.dirty = true;

    switch (input) {
        case menu_input_exit:
            menu_state.is_in_menu = false;
            memset(menu_state.menu_stack, 0xFF, sizeof(menu_state.menu_stack));
            menu_state.selected_child = 0xFF;
            return false;
        case menu_input_back:
            // Iterate backwards through the stack and remove the last entry
            for (int i = 0; i < sizeof(menu_state.menu_stack); ++i) {
                if (menu_state.menu_stack[sizeof(menu_state.menu_stack) - 1 - i] != 0xFF) {
                    menu_state.selected_child                               = menu_state.menu_stack[sizeof(menu_state.menu_stack) - 1 - i];
                    menu_state.menu_stack[sizeof(menu_state.menu_stack) - 1 - i] = 0xFF;
                    break;
                }

                // If we've dropped out of the last entry in the stack, exit the menu
                if (i == sizeof(menu_state.menu_stack) - 1) {
                    menu_state.is_in_menu     = false;
                    menu_state.selected_child = 0xFF;
                }
            }
            return false;
        case menu_input_enter:
            // Only attempt to enter the next menu if we're a parent object
            if (selected->flags & menu_flag_is_parent) {
                // Iterate forwards through the stack and add the selected entry
                for (int i = 0; i < sizeof(menu_state.menu_stack); ++i) {
                    if (menu_state.menu_stack[i] == 0xFF) {
                        menu_state.menu_stack[i]  = menu_state.selected_child;
                        menu_state.selected_child = 0;
                        break;
                    }
                }
            }
            return false;
        case menu_input_up:
            menu_state.selected_child = (menu_state.selected_child + menu->parent.child_count - 1) % menu->parent.child_count;
            return false;
        case menu_input_down:
            menu_state.selected_child = (menu_state.selected_child + menu->parent.child_count + 1) % menu->parent.child_count;
            return false;
        case menu_input_left:
        case menu_input_right:
            if (selected->flags & menu_flag_is_value) {
                return selected->child.menu_handler(input);
            }
            return false;
        default:
            menu_state.dirty = false;
            return false;
    }
}

bool process_record_menu(uint16_t keycode, keyrecord_t *record) {

    if (keycode == CONFIG_MENU && record->event.pressed && !menu_state.is_in_menu) {
        menu_state.is_in_menu     = true;
        menu_state.selected_child = 0;
        return false;
    }

    if (menu_state.is_in_menu) {
        if (record->event.pressed) {
            switch (keycode) {
                case CONFIG_MENU:
                    return menu_handle_input(menu_input_exit);
                case KC_ESC:
                case KC_GESC:
                    return menu_handle_input(menu_input_back);
                case KC_SPACE:
                case KC_ENTER:
                case KC_RETURN:
                case KC_SFTENT:
                case KC_MS_BTN1:
                    return menu_handle_input(menu_input_enter);
                case KC_UP:
                case KC_MS_UP:
                    return menu_handle_input(menu_input_up);
                case KC_DOWN:
                case KC_MS_DOWN:
                    return menu_handle_input(menu_input_down);
                case KC_LEFT:
                case KC_MS_WH_DOWN:
                case KC_MS_LEFT:
                    return menu_handle_input(menu_input_left);
                case KC_RIGHT:
                case KC_MS_WH_UP:
                case KC_MS_RIGHT:
                    return menu_handle_input(menu_input_right);
                default:
                    break;
            }
        }
    }
    return true;
}

void process_encoder_menu(int8_t index, bool clockwise) {
    //  navigate menu options
    if (clockwise) {
        menu_handle_input(menu_input_right);
    } else {
        menu_handle_input(menu_input_left);
    }
}

bool needs_refresh(void) {
    static menu_state_t last_state;

    if (menu_state.dirty == true) {
        return true;
    }

    if (memcmp(&last_state, &menu_state, sizeof(menu_state_t))) { // if the state has changed
        memcpy(&last_state, &menu_state, sizeof(menu_state_t));
        return true;
    }

    return false;
}

void render_menu(void) {
    if (!needs_refresh()) return;
    if (menu_state.is_in_menu) {
        qp_rect(lcd, 0, 0, LCD_WIDTH-1, LCD_HEIGHT-1, 0, 0, 0, true);

        uint8_t       hue      = rgb_matrix_get_hue();
        menu_entry_t *menu     = get_current_menu();
        menu_entry_t *selected = get_selected_menu_item();

        int y = 80;

        qp_rect(lcd, 0, y, LCD_WIDTH-1, y + 3, hue, 255, 255, true);
        y += 8;

        qp_drawtext(lcd, 8, y, font_thintel15, menu->text);
        y += font_thintel15->glyph_height + 4;

        qp_rect(lcd, 0, y, LCD_WIDTH-1, y + 3, hue, 255, 255, true);
        y += 8;
        for (int i = 0; i < menu->parent.child_count; ++i) {
            menu_entry_t *child = &menu->parent.children[i];
            uint16_t      x;
            if (child == selected) {
                x = qp_drawtext_recolor(lcd, 8, y, font_thintel15, child->text, HSV_GREEN, 85, 255, 0);
            } else {
                x = qp_drawtext_recolor(lcd, 8, y, font_thintel15, child->text, HSV_RED, 0, 255, 0);
            }
            if (child->flags & menu_flag_is_parent) {
                qp_drawtext(lcd, 8 + x, y, font_thintel15, "  >");
            }
            if (child->flags & menu_flag_is_value) {
                char buf[32] = {0};
                child->child.display_handler(buf, sizeof(buf));
                qp_drawtext(lcd, 8 + x, y, font_thintel15, buf);
            }
            y += font_thintel15->glyph_height + 4;
            qp_rect(lcd, 0, y, LCD_WIDTH-1, y, hue, 255, 255, true);
            y += 5;
        }
        menu_state.dirty = false;
    }
}

void keyboard_post_init_menu(void) {
    transaction_register_rpc(STATE_SYNC_USER_MENU, rpc_update_menu_state);
}
