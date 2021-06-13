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
#include "serial.h"
#include "keymap.h"
#include "encoder.c"
#include "features/display/display.h"
#ifdef PONG_ENABLE
#   include "games/pong/pong.h"
#endif
#ifdef CONFIG_MENU_ENABLE
#   include "features/config_menu/menu.h"
#endif

kb_system_state_t kb_system_state = {0};

uint32_t          last_slave_sync_time = 0;

// clang-format off

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [ALPHA] = LAYOUT_all(
        KC_ESC,         KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                            KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSLS,
        KC_TAB,         KC_Q,   KC_W,    KC_D,    KC_R,    KC_K,                            KC_Y,    KC_U,    KC_I,    KC_L,    KC_P,    LT(ACUTE,KC_QUOTE),
        LCTL(KC_LCTRL), KC_A,   KC_S,    KC_T,    KC_F,    KC_G,                            KC_H,    KC_N,    KC_E,    KC_O,    LT(UMLAUT,KC_SCLN), KC_ENT,
        KC_LSPO,        KC_Z,   KC_X,    KC_C,    KC_V,    KC_B,                            KC_J,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSPC,
                            KC_LALT, KC_LWIN,  LT(ADMIN,KC_SPC),  KC_HOME,            RGB_MOD,   LT(ADMIN,KC_SPC),  KC_DEL,  KC_BSPC,
                                                 LT(GRAVE,KC_GRAVE), KC_END,    KC_PGUP, KC_PGDN,

                                                     KC_MS_UP,                           KC_UP,
                                        KC_MS_LEFT, KC_MS_BTN1, KC_MS_RIGHT,    KC_LEFT, KC_ENT, KC_RIGHT,
                                                     KC_MS_DOWN,                        KC_DOWN
    ),
    [MEDIA] = LAYOUT_all(
        KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,                         KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,
        KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,                         KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,
        KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,                         KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,
        KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,                         KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,
                                    KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,             KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,
                                                                 KC_TRNS, KC_TRNS,    KC_TRNS, KC_TRNS,
    
                                                         KC_TRNS,                                   KC_TRNS,
                                                KC_TRNS, KC_TRNS, KC_TRNS,                 KC_TRNS, KC_TRNS, KC_TRNS,
                                                         KC_TRNS,                                   KC_TRNS
     ),
    [GRAVE] = LAYOUT_all(
        KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,                         KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,
        KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,                         KC_TRNS,   ACCENT_15, ACCENT_16, KC_TRNS, KC_TRNS,   KC_TRNS,
        KC_TRNS,   ACCENT_14, KC_TRNS,  KC_TRNS,   KC_TRNS,   KC_TRNS,                         KC_TRNS,   KC_TRNS, ACCENT_17, ACCENT_18, KC_TRNS,   KC_TRNS,
        KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,                         KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,
                                    KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,             KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,
                                                                 KC_TRNS, KC_TRNS,    KC_TRNS, KC_TRNS,
    
                                                         KC_TRNS,                                   KC_TRNS,
                                                KC_TRNS, KC_TRNS, KC_TRNS,                 KC_TRNS, KC_TRNS, KC_TRNS,
                                                         KC_TRNS,                                   KC_TRNS
    ),
    [ACUTE] = LAYOUT_all(
        KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,                         KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,
        KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,                         ACCENT_3, ACCENT_4, ACCENT_5, KC_TRNS, KC_TRNS,   KC_TRNS,
        KC_TRNS,   ACCENT_2, KC_TRNS,  KC_TRNS,   KC_TRNS,   KC_TRNS,                         KC_TRNS,   KC_TRNS, ACCENT_6, ACCENT_7, KC_TRNS,   KC_TRNS,
        KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,                         KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,
                                    KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,             KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,
                                                                 KC_TRNS, KC_TRNS,    KC_TRNS, KC_TRNS,
    
                                                         KC_TRNS,                                   KC_TRNS,
                                                KC_TRNS, KC_TRNS, KC_TRNS,                 KC_TRNS, KC_TRNS, KC_TRNS,
                                                         KC_TRNS,                                   KC_TRNS
    ),
    [UMLAUT] = LAYOUT_all(
        KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,                         KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,
        KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,                         ACCENT_9, ACCENT_10, ACCENT_11, KC_TRNS, KC_TRNS,   KC_TRNS,
        KC_TRNS,   ACCENT_8, KC_TRNS,  KC_TRNS,   KC_TRNS,   KC_TRNS,                         KC_TRNS,   KC_TRNS, ACCENT_12, ACCENT_13, KC_TRNS,   KC_TRNS,
        KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,                         KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,
                                    KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,             KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,
                                                                 KC_TRNS, KC_TRNS,    KC_TRNS, KC_TRNS,
    
                                                         KC_TRNS,                                   KC_TRNS,
                                                KC_TRNS, KC_TRNS, KC_TRNS,                 KC_TRNS, KC_TRNS, KC_TRNS,
                                                         KC_TRNS,                                   KC_TRNS
     ),
    [EXTRA] = LAYOUT_all(
        KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,                         KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,
        KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,                         KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,
        KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,                         KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,
        KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,                         KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,
                                    KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,             KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,
                                                                 KC_TRNS, KC_TRNS,    KC_TRNS, KC_TRNS,
    
                                                         KC_TRNS,                                   KC_TRNS,
                                                KC_TRNS, KC_TRNS, KC_TRNS,                 KC_TRNS, KC_TRNS, KC_TRNS,
                                                         KC_TRNS,                                   KC_TRNS
     ),
    [DEVEL] = LAYOUT_all(
        KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,                         KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,
        KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,                         KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,
        KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,                         KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,
        KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,                         KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,
                                    KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,             KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,
                                                                KC_TRNS, KC_TRNS,    KC_TRNS, KC_TRNS,
    
                                                         KC_TRNS,                                   KC_TRNS,
                                                KC_TRNS, KC_TRNS, KC_TRNS,                 KC_TRNS, KC_TRNS, KC_TRNS,
                                                         KC_TRNS,                                   KC_TRNS
     ),
    [SPECIAL] = LAYOUT_all(
        KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,                         KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,
        KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,                         KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,
        KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,                         KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,
        KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,                         KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,
                                    KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,             KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,
                                                                KC_TRNS, KC_TRNS,    KC_TRNS, KC_TRNS,
    
                                                         KC_TRNS,                                   KC_TRNS,
                                                KC_TRNS, KC_TRNS, KC_TRNS,                 KC_TRNS, KC_TRNS, KC_TRNS,
                                                         KC_TRNS,                                   KC_TRNS
    ),
    [ADMIN] = LAYOUT_all(
      SHOW_LOGO, CONFIG_MENU, SHOW_PONG, KC_TRNS,   KC_TRNS,   KC_TRNS,                         KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,
        KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,                         KC_TRNS,   KC_TRNS,   KC_TRNS,   LWIN(KC_L),   LALT(KC_PSCR),   KC_TRNS,
        KC_TRNS,   KC_TRNS,   KC_SYSTEM_SLEEP,   KC_TRNS,   KC_TRNS,   KC_TRNS,                         KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,
        KC_TRNS,   KC_UNDO,   KC_CUT,  KC_COPY,   KC_PASTE,   KC_TRNS,                         KC_TRNS,   CONFIG_MENU,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,
                                    KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,             KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,
                                                                KC_TRNS, KC_TRNS,    KC_TRNS, KC_TRNS,
    
                                                            KC_TRNS,                                   KC_TRNS,
                                                KC_TRNS, KC_TRNS, KC_TRNS,                 KC_TRNS, KC_TRNS, KC_TRNS,
                                                            KC_TRNS,                                   KC_TRNS
    )
};

// clang-format on



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   Implementation of state sync between primary/secondary sides
// ----------------------------------------------------------------------

void rpc_sync_kb_state(void) {
    if (is_keyboard_master()) {
        // Keep track of the last state, so that we can tell if we need to propagate to secondary side
        static kb_system_state_t last_kb_state;
        static uint32_t          last_sync;
        bool                     needs_sync = false;

       // Check if the keyboard state values are different
        if (memcmp(&kb_system_state, &last_kb_state, sizeof(kb_system_state_t))) {
            needs_sync = true;
            memcpy(&last_kb_state, &kb_system_state, sizeof(kb_system_state_t));
        }

        // Send to secondary side periodically regardless of state change (period defined in config.h)
        if (timer_elapsed32(last_sync) > MAX_SYNC_INTERVAL_MS) {
            needs_sync = true;
        }
        // Perform the sync if requested
        if (needs_sync) {
            char result_string[RPC_S2M_BUFFER_SIZE];
                dprint("--- syncing data ---\n");
                int result = transaction_rpc_exec(STATE_SYNC_USER, sizeof(kb_system_state_t), &kb_system_state, RPC_S2M_BUFFER_SIZE, result_string);
            if ( result) {
               last_sync = timer_read32();
            } else {
                dprintf("Failed to sync with result 0x%04x\n", result);
            }
            dprint(result_string);
        }
    }
}

void rpc_update_kb_state(uint8_t initiator2target_buffer_size, const void* initiator2target_buffer, uint8_t target2initiator_buffer_size, void* target2initiator_buffer) {
    if (initiator2target_buffer_size == sizeof(kb_system_state_t)) {
        sprintf(target2initiator_buffer, "syncing state structure size: 0x%02X\n", sizeof(kb_system_state_t));
        memcpy(&kb_system_state, initiator2target_buffer, sizeof(kb_system_state_t));
    } else {
        sprintf(target2initiator_buffer, "invalid transaction size 0x%02x\n", initiator2target_buffer_size);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   Initialization
// ----------------------------------------------------------------------

void keyboard_post_init_state(void) {
     // Register keyboard state sync split transaction
    transaction_register_rpc(STATE_SYNC_USER, rpc_update_kb_state);
    memset(&kb_system_state, 0, sizeof(kb_system_state));
}

void keyboard_post_init_user(void) {
#ifdef PONG_ENABLE
    pong_init();
#endif
#ifdef CONFIG_MENU_ENABLE
    keyboard_post_init_menu();
#endif
    keyboard_post_init_display();
    keyboard_post_init_state();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   Housekeeping routines
// ----------------------------------------------------------------------


void housekeeping_task_user(void) {
    // Turn on/off the LCD
    static bool lcd_on = true;

    if (lcd_on != (bool)kb_system_state.lcd_power) {
        lcd_on = (bool)kb_system_state.lcd_power;
        qp_power(lcd, lcd_on);
    }

    if (is_keyboard_master())
    {
        kb_system_state.lcd_power = (last_input_activity_elapsed() < LCD_ACTIVITY_TIMEOUT*1000) ? 1 : 0;
        rpc_sync_kb_state();
        rpc_sync_display_state();
#ifdef CONFIG_MENU_ENABLE
        rpc_sync_menu_state();
#endif
#ifdef PONG_ENABLE
        pong_state_sync();
#endif

        // Match the backlight to the LCD state
        if (is_backlight_enabled() != lcd_on) {
            if (lcd_on)
                backlight_enable();
            else
                backlight_disable();
        }
    }

    housekeeping_task_render_display();

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   Key processing
// ----------------------------------------------------------------------

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  // If console is enabled, it will print the matrix position and status of each key pressed
#ifdef CONSOLE_ENABLE
    uprintf("KL: keycode: 0x%04X, col: %u, row: %u, pressed: %04x, time: %u, interrupt: %04x, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
#endif
    if (record->event.pressed) {
        switch (keycode) {
            case ACCENT_0:
                SEND_STRING(SS_LGUI(SS_LSFT(SS_TAP(X_S))));
                break;
            case ACCENT_1:
                SEND_STRING(SS_LGUI(SS_LSFT(SS_TAP(X_S))));
                break;
            case ACCENT_2:
                SEND_STRING(SS_LALT(SS_TAP(X_KP_1) SS_TAP(X_KP_6) SS_TAP(X_KP_0) ));
                break;
            case ACCENT_3:
                SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_2) SS_TAP(X_KP_5) SS_TAP(X_KP_3) ));
                break;
            case ACCENT_4:
                SEND_STRING(SS_LALT(SS_TAP(X_KP_1) SS_TAP(X_KP_6) SS_TAP(X_KP_3) ));
                break;
            case ACCENT_5:
                SEND_STRING(SS_LALT(SS_TAP(X_KP_1) SS_TAP(X_KP_6) SS_TAP(X_KP_1) ));
                break;
            case ACCENT_6:
                SEND_STRING(SS_LALT(SS_TAP(X_KP_1) SS_TAP(X_KP_3) SS_TAP(X_KP_0) ));
                break;
            case ACCENT_7:
                SEND_STRING(SS_LALT(SS_TAP(X_KP_1) SS_TAP(X_KP_6) SS_TAP(X_KP_2) ));
                break;
            case ACCENT_8:
                SEND_STRING(SS_LALT(SS_TAP(X_KP_1) SS_TAP(X_KP_3) SS_TAP(X_KP_2) ));
                break;
            case ACCENT_9:
                SEND_STRING(SS_LALT(SS_TAP(X_KP_1) SS_TAP(X_KP_5) SS_TAP(X_KP_2) ));
                break;
            case ACCENT_10:
                SEND_STRING(SS_LALT(SS_TAP(X_KP_1) SS_TAP(X_KP_2) SS_TAP(X_KP_9) ));
                break;
            case ACCENT_11:
                SEND_STRING(SS_LALT(SS_TAP(X_KP_1) SS_TAP(X_KP_3) SS_TAP(X_KP_9) ));
                break;
            case ACCENT_12:
                SEND_STRING(SS_LALT(SS_TAP(X_KP_1) SS_TAP(X_KP_3) SS_TAP(X_KP_7) ));
                break;
            case ACCENT_13:
                SEND_STRING(SS_LALT(SS_TAP(X_KP_1) SS_TAP(X_KP_4) SS_TAP(X_KP_8) ));
                break;
            case ACCENT_14:
                SEND_STRING(SS_LALT(SS_TAP(X_KP_1) SS_TAP(X_KP_3) SS_TAP(X_KP_3) ));
                break;
            case ACCENT_15:
                SEND_STRING(SS_LALT(SS_TAP(X_KP_1) SS_TAP(X_KP_5) SS_TAP(X_KP_1) ));
                break;
            case ACCENT_16:
                SEND_STRING(SS_LALT(SS_TAP(X_KP_1) SS_TAP(X_KP_4) SS_TAP(X_KP_1) ));
                break;
            case ACCENT_17:
                SEND_STRING(SS_LALT(SS_TAP(X_KP_1) SS_TAP(X_KP_3) SS_TAP(X_KP_8) ));
                break;
            case ACCENT_18:
                SEND_STRING(SS_LALT(SS_TAP(X_KP_1) SS_TAP(X_KP_4) SS_TAP(X_KP_9) ));
                break;
#ifdef CONFIG_MENU_ENABLE
            case CONFIG_MENU:
                 // if we are currently in the menu, then unset the menu state and exit
                if (get_display_state() == MENU_CONFIG) {
                    revert_display_state();
                    set_menu_dirty_bit(true);
                } else {
                    set_display_state(MENU_CONFIG);
                    return process_record_menu(keycode, record);
                }
                return false;
#endif // CONFIG_MENU_ENABLE
#ifdef PONG_ENABLE
            case SHOW_PONG:
                // toggle PONG on and off
                if (get_display_state() == PONG) {
                    revert_display_state();
                } else {
                    set_display_state(PONG);
                }
                return false;
#endif // PONG_ENABLE
            case SHOW_LOGO:
                // Show the logo
                set_display_state(LOGO);
                return false;
            default:
#ifdef CONFIG_MENU_ENABLE
                if (get_display_state() == MENU_CONFIG) return process_record_menu(keycode, record);
#endif
#ifdef PONG_ENABLE
                if (get_display_state() == PONG) return process_record_pong(keycode, record);
#endif
                break;
        }
    }
    return true;
}

