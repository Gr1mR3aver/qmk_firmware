#pragma once

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

// clang-format off

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [ALPHA] = LAYOUT_all(
        KC_ESC,         KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                            KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSLS,
        KC_TAB,         KC_Q,   KC_W,    KC_D,    KC_R,    KC_K,                            KC_Y,    KC_U,    KC_I,    KC_L,    KC_P,    LT(ACUTE,KC_QUOTE),
        LCTL(KC_LCTRL), KC_A,   KC_S,    KC_T,    KC_F,    KC_G,                            KC_H,    KC_N,    KC_E,    KC_O,    LT(UMLAUT,KC_SCLN), KC_ENT,
        KC_LSPO,        KC_Z,   KC_X,    KC_C,    KC_V,    KC_B,                            KC_J,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSPC,
                            KC_LALT, KC_LWIN,  LT(ADMIN,KC_SPC),  KC_HOME,            KC_MINUS,   LT(ADMIN,KC_SPC),  LT(DEVEL,KC_DEL),  KC_BSPC,
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
        KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,                         KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_LBRACKET,   KC_RBRACKET,   KC_TRNS,
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
