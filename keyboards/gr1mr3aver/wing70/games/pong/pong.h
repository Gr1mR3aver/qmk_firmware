/* Copyright © 2021 Paul Cotter (@gr1mr3aver)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#ifdef CONFIG_MENU_ENABLE
#   include "features/config_menu/menu.h"
#endif

// Game settings
#define FPS_LIMIT 60
#define BALL_RADIUS 6
#define PADDLE_WIDTH 4
#define PADDLE_HEIGHT 40
#define MAX_SCORE 10
#define PADDLE_MOVEMENT_INCREMENT 6
#define PADDLE_HITS_ACCELERATE 10

/////////////////////
/// Color settings
#define BALL_COLOR HSV_WHITE
#define PADDLE_COLOR HSV_WHITE
#define COURT_FG_COLOR HSV_WHITE
#define COURT_BG_COLOR HSV_BLACK
// By default the net will use the rgb matrix color. Uncomment the following line and
//  specify the color if you want the net to use a specfic color.
// #define NET_COLOR HSV_RED


// score rendering
#define SEGMENT_WIDTH 2
#define SEGMENT_LENGTH 20
#define SCORE_COLOR HSV_CYAN
#define SCORE_LEADER_COLOR HSV_RED


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   UI element typedefs
// ----------------------------------------------------------------------

typedef struct _pong_position_t {
    int16_t x, y;
} pong_position_t;

typedef struct _pong_velocity_t {
    int16_t dx, dy;
} pong_velocity_t;

typedef struct _pong_dimensions_t {
    uint8_t width, height;
} pong_dimensions_t;

typedef struct _pong_color_t {
    uint8_t hue, sat, val;
} pong_color_t;

typedef struct _pong_ball_t {
    pong_color_t color;
    pong_position_t position;
    uint8_t radius;
    pong_velocity_t velocity;
} pong_ball_t;

typedef struct _pong_paddle_t {
    pong_color_t color;
    pong_position_t position;
    pong_dimensions_t size;
} pong_paddle_t;

typedef struct _pong_player_t {
    pong_paddle_t paddle;
    uint16_t score;
} pong_player_t;

typedef struct _pong_court_t {
    pong_player_t player1, player2;
    pong_ball_t ball;
} pong_court_t;

typedef struct _pong_state_t {
    pong_court_t court;
    bool play_paused, play_starting, play_complete, needs_reset;
    uint32_t paddle_collision_count;
} pong_state_t;

// score display segment
typedef struct _segment_t {
    pong_color_t color;
    pong_position_t position;
    pong_dimensions_t size;
    bool is_visible;
} segment_t;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   External interface
// ----------------------------------------------------------------------

void pong_init(void);
void pong_reset(void);
void render_game_pong(void);
bool process_record_pong(uint16_t keycode, keyrecord_t *record);
void process_encoder_pong(int8_t index, bool clockwise);
void pong_state_sync(void);
