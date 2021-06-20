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


#include QMK_KEYBOARD_H
#include <qp.h>
#include <math.h>
#include "serial.h"
#include "pong.h"
#include "wing70.h"


static bool is_left;

static segment_t a,b,c,d,e,f,g;
pong_state_t pong_state;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   Implementation of state sync between primary/secondary sides
// ----------------------------------------------------------------------

void pong_state_sync(void) {
    if (is_keyboard_master()) {
        // Keep track of the last state, so that we can tell if we need to propagate to secondary side
        static pong_state_t last_pong_state;
        static uint32_t          last_sync;
        bool                     needs_sync = false;

       // Check if the pong court state values are different
        if (memcmp(&pong_state, &last_pong_state, sizeof(pong_state_t))) {
            needs_sync = true;
            memcpy(&last_pong_state, &pong_state, sizeof(pong_state_t));
        }

        // Send to secondary side periodically regardless of state change (period defined in config.h)
        if (timer_elapsed32(last_sync) > MAX_SYNC_INTERVAL_MS) {
            needs_sync = true;
        }

        // Perform the sync if requested
        if (needs_sync) {
            char result_string[RPC_S2M_BUFFER_SIZE];
                int result = transaction_rpc_exec(STATE_SYNC_USER_PONG, sizeof(pong_state_t), &pong_state, RPC_S2M_BUFFER_SIZE, result_string);
            if ( result) {
               last_sync = timer_read32();
            }
#ifdef CONSOLE_ENABLED
            else {
                dprintf("Failed to sync pong state with result 0x%04x\n", result);
            }
            dprint(result_string);
#endif
        }
    }
}

void rpc_update_pong_state(uint8_t initiator2target_buffer_size, const void* initiator2target_buffer, uint8_t target2initiator_buffer_size, void* target2initiator_buffer) {
    if (initiator2target_buffer_size == sizeof(pong_state_t)) {
        sprintf(target2initiator_buffer, "- updating pong_state.court.-\n");
        memcpy(&pong_state, initiator2target_buffer, sizeof(pong_state_t));
    } else {
        sprintf(target2initiator_buffer, "XXX invalid transaction size 0x%02x XXX\n", initiator2target_buffer_size);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   Internal game functions
// ----------------------------------------------------------------------

void reset_court(void) {
    uint16_t x_start = rand() % 2 ? -2 : 2;
    uint16_t y_start = rand() % 2 ? -(rand() % 3) : rand() % 3;

    pong_state.court.ball.velocity.dx = x_start;
    pong_state.court.ball.velocity.dy = y_start;    pong_state.court.ball.position.x = 0;
    pong_state.court.ball.position.y = LCD_HEIGHT / 2;
    pong_state.court.ball.radius = BALL_RADIUS;

    pong_state.court.player1.paddle.size.width = PADDLE_WIDTH;
    pong_state.court.player1.paddle.size.height = PADDLE_HEIGHT;
    pong_state.court.player1.paddle.position.x = 0;
    pong_state.court.player1.paddle.position.y = (LCD_HEIGHT / 2) - (PADDLE_HEIGHT / 2);

    pong_state.court.player2.paddle.size.width = PADDLE_WIDTH;
    pong_state.court.player2.paddle.size.height = PADDLE_HEIGHT;
    pong_state.court.player2.paddle.position.x = (LCD_WIDTH - 1) - PADDLE_WIDTH;
    pong_state.court.player2.paddle.position.y = (LCD_HEIGHT / 2) - (PADDLE_HEIGHT / 2);

    pong_state.play_complete = false;
    pong_state.paddle_collision_count = 0;
}

void bounce_ball_x(pong_ball_t *ball, uint16_t min_x, uint16_t max_x) {
    bool bounced = false;
    bool isPlayerOne = true;

    if (ball->position.x + ball->radius > max_x) {
        // collision on right side
        bounced = true;
        isPlayerOne = false;
    } else if (ball->position.x - ball->radius < min_x) {
        //collision on left side
        bounced = true;
        isPlayerOne = true;
    }
    if (bounced) {
        pong_player_t *player = (isPlayerOne ? &pong_state.court.player1 : &pong_state.court.player2);
        uint16_t paddle_half, paddle_middle, paddle_range;
        double offset_fraction, radians, ball_speed;

        paddle_half = player->paddle.size.height/2;
        paddle_middle = player->paddle.position.y + paddle_half;
        paddle_range = paddle_half + ball->radius;
        ball_speed = sqrt((ball->velocity.dx^2) + (ball->velocity.dy^2));

        offset_fraction = (ball->position.y - paddle_middle) / paddle_range;

        radians = 0.25 * M_PI * (2 * offset_fraction);

        pong_state.paddle_collision_count++;
        if (pong_state.paddle_collision_count > PADDLE_HITS_ACCELERATE) {
            if (ball->velocity.dx > 0) ball->velocity.dx++;
            if (ball->velocity.dx < 0) ball->velocity.dx--;
            pong_state.paddle_collision_count = 0;
        }

        // invert x velocity
        ball->velocity.dx *= -1;
        // change y velocity according to the distance from the center of the paddle
        ball->velocity.dy = round(ball_speed * sin(radians));
    }
}

void bounce_ball_y(pong_ball_t *ball, uint16_t min_y, uint16_t max_y) {
    // invert the velocity (perfect bounce)
    // check the play area bounds and move back within the play area if needed
    bool bounced = false;
    if (ball->position.y + ball->radius > max_y || ball->position.y - ball->radius < min_y) {
        bounced = true;
    }
    if (bounced) ball->velocity.dy *= -1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   Game rendering functions
// ----------------------------------------------------------------------

void score_point(pong_player_t *player) {
    player->score += 1;
    reset_court();
}

inline void render_ball_shadow(pong_ball_t *ball, uint16_t shadow_x) {
    qp_rect(lcd, shadow_x, ball->position.y - ball->radius, shadow_x, ball->position.y + ball->radius, ball->color.hue, ball->color.sat, ball->color.val, true);
}

void render_ball(pong_ball_t *ball, pong_ball_t *last_ball, uint8_t hue, uint8_t sat, uint8_t val) {
    uint16_t x_offset = is_left ? LCD_WIDTH - 1 : 0;
    if (memcmp(ball, last_ball, sizeof(pong_ball_t))) {
        // if the ball has moved, overwrite the previous ball position
        qp_circle(lcd, last_ball->position.x + x_offset, last_ball->position.y, last_ball->radius, COURT_BG_COLOR, true);

        // make sure the new position is copied to the local instance
        memcpy(last_ball, ball, sizeof(pong_ball_t));
    }

    // render the current ball position
    qp_circle(lcd, ball->position.x + x_offset, ball->position.y, ball->radius, hue, sat, val, true);

    if (is_left && ball->position.x > 0 ) { // handle rendering the ball shadow on the left screen
        // render the ball's shadow
        render_ball_shadow(ball, x_offset);
        return;
    } else if (ball->position.x < 0 ) {
        // render the ball's shadow
        render_ball_shadow(ball, x_offset);
        return;
    }
}

void render_court(uint16_t net_x_position) {
        /// 1 - render the "net"
#ifdef NET_COLOR
    qp_rect(lcd, net_x_position, 0, net_x_position, LCD_HEIGHT -1, NET_COLOR, true);
#else
    qp_rect(lcd, net_x_position, 0, net_x_position, LCD_HEIGHT -1, rgb_matrix_get_hue(), rgb_matrix_get_sat(), rgb_matrix_get_val(), true);
#endif
    /// render top and bottom "lines"
    qp_rect(lcd, 0, 0, LCD_WIDTH-1, 0, COURT_FG_COLOR, true);
    qp_rect(lcd, 0 , LCD_HEIGHT-1, LCD_WIDTH-1, LCD_HEIGHT-1, COURT_FG_COLOR, true);
}

void render_paddle(pong_player_t *player, uint16_t x_offset) {
    static pong_paddle_t last_paddle;
    // check if the paddle has moved
    if (memcmp(&player->paddle.position, &last_paddle.position, sizeof(pong_position_t)))
    {
        // erase the previous paddle
        qp_rect(lcd, x_offset, last_paddle.position.y, x_offset + last_paddle.size.width, last_paddle.position.y + last_paddle.size.height, COURT_BG_COLOR, true);
        memcpy(&last_paddle, &player->paddle, sizeof(pong_paddle_t));
        // draw the new paddle
        qp_rect(lcd, x_offset, player->paddle.position.y, x_offset + player->paddle.size.width, player->paddle.position.y + player->paddle.size.height, player->paddle.color.hue, player->paddle.color.sat, player->paddle.color.val, true);
    }
}

void render_moving_ball(void) {

    static pong_ball_t last_ball = {
        .position = {
            .x = 0,
            .y = LCD_HEIGHT / 2
        },
        .radius = BALL_RADIUS,
        .velocity = {
            .dx = 0,
            .dy = 0
        }
    };

    // the primary side of the keyboard must check for collisions, bounce the ball
    //  and update the game state, which is then sync'd to the secondary side.
    //  secondary side is only responsible for rendering its half of the screen.
    if (is_keyboard_master()) {
        // bounce the ball off the top/bottom as needed
        bounce_ball_y(&pong_state.court.ball, 1, LCD_HEIGHT - 2);

        // detect if the ball is past the paddle surface
        if ( (pong_state.court.ball.position.x - (pong_state.court.player1.paddle.size.width + pong_state.court.ball.radius)) < - (LCD_WIDTH - 1) ) { // past player1's paddle
            if (pong_state.court.ball.position.y < pong_state.court.player1.paddle.position.y - pong_state.court.ball.radius ||
                pong_state.court.ball.position.y > pong_state.court.player1.paddle.position.y + pong_state.court.player1.paddle.size.height + pong_state.court.ball.radius) {
                // didn't hit the paddle - score for other player
                score_point(&pong_state.court.player2);
                return;
            } else {
                bounce_ball_x(&pong_state.court.ball, - (LCD_WIDTH - 1 - pong_state.court.player1.paddle.size.width), LCD_WIDTH - 1 - pong_state.court.player1.paddle.size.width);
            }
        } else if ( (pong_state.court.ball.position.x + (pong_state.court.player2.paddle.size.width +pong_state.court.ball.radius)) > (LCD_WIDTH - 1) ) { // past player2's paddle
            // detect if the ball missed the paddle
            if (pong_state.court.ball.position.y < pong_state.court.player2.paddle.position.y - pong_state.court.ball.radius ||
                pong_state.court.ball.position.y > pong_state.court.player2.paddle.position.y + pong_state.court.player2.paddle.size.height + pong_state.court.ball.radius ) {
                score_point(&pong_state.court.player1);
                return;
            } else {
                // if it did hit the paddle, bounce the ball
                bounce_ball_x(&pong_state.court.ball, - (LCD_WIDTH - 1 - pong_state.court.player2.paddle.size.width), LCD_WIDTH - 1 - pong_state.court.player2.paddle.size.width);
            }
        }
    }

    // move the ball using dx and dy
    pong_state.court.ball.position.x += pong_state.court.ball.velocity.dx;
    pong_state.court.ball.position.y += pong_state.court.ball.velocity.dy;

    // draw the new position
    render_ball(&pong_state.court.ball, &last_ball, BALL_COLOR);
}

void render_segment(segment_t *segment, uint16_t x_offset, uint16_t y_offset) {
    if (segment->is_visible) {
        qp_rect(lcd, segment->position.x + x_offset, segment->position.y + y_offset,  segment->position.x + x_offset + segment->size.width,  segment->position.y + segment->size.height + y_offset, segment->color.hue, segment->color.sat, segment->color.val, true);
    } else {
        qp_rect(lcd, segment->position.x + x_offset, segment->position.y + y_offset,  segment->position.x + x_offset + segment->size.width,  segment->position.y + segment->size.height + y_offset, COURT_BG_COLOR, true);
    }
}

bool is_vertical(segment_t *segment) {
    return segment->size.width == SEGMENT_WIDTH;
}

void set_object_color(pong_color_t *color, uint8_t hue, uint8_t sat, uint8_t val) {
    color->hue = hue;
    color->sat = sat;
    color->val = val;
}

void set_all_segments_color(uint8_t hue, uint8_t sat, uint8_t val) {
    set_object_color(&a.color, hue, sat, val);
    set_object_color(&b.color, hue, sat, val);
    set_object_color(&c.color, hue, sat, val);
    set_object_color(&d.color, hue, sat, val);
    set_object_color(&e.color, hue, sat, val);
    set_object_color(&f.color, hue, sat, val);
    set_object_color(&g.color, hue, sat, val);
}

void flip_orientation(segment_t *segment) {
    uint16_t tmp_val = segment->size.width;
    segment->size.width = segment->size.height;
    segment->size.height = tmp_val;
}

void reset_segments(void) {
    a.is_visible = false;
    b.is_visible = false;
    c.is_visible = false;
    d.is_visible = false;
    e.is_visible = false;
    f.is_visible = false;
    g.is_visible = false;
    set_all_segments_color(PADDLE_COLOR);
}

void render_all_segments(uint16_t offset_x, uint16_t offset_y) {
    render_segment(&a, offset_x, offset_y);
    render_segment(&b, offset_x, offset_y);
    render_segment(&c, offset_x, offset_y);
    render_segment(&d, offset_x, offset_y);
    render_segment(&e, offset_x, offset_y);
    render_segment(&f, offset_x, offset_y);
    render_segment(&g, offset_x, offset_y);
}

void render_digit(uint8_t digit, uint16_t position_x, uint16_t position_y) {
    switch(digit) {
        case 0:
            a.is_visible = true;
            b.is_visible = true;
            c.is_visible = true;
            d.is_visible = false;
            e.is_visible = true;
            f.is_visible = true;
            g.is_visible = true;
            break;
        case 1:
            a.is_visible = false;
            b.is_visible = false;
            c.is_visible = false;
            d.is_visible = false;
            e.is_visible = false;
            f.is_visible = true;
            g.is_visible = true;
            break;
        case 2:
            a.is_visible = false;
            b.is_visible = true;
            c.is_visible = true;
            d.is_visible = true;
            e.is_visible = true;
            f.is_visible = true;
            g.is_visible = false;
            break;
        case 3:
            a.is_visible = false;
            b.is_visible = false;
            c.is_visible = true;
            d.is_visible = true;
            e.is_visible = true;
            f.is_visible = true;
            g.is_visible = true;
            break;
        case 4:
            a.is_visible = true;
            b.is_visible = false;
            c.is_visible = false;
            d.is_visible = true;
            e.is_visible = false;
            f.is_visible = true;
            g.is_visible = true;
            break;
        case 5:
            a.is_visible = true;
            b.is_visible = false;
            c.is_visible = true;
            d.is_visible = true;
            e.is_visible = true;
            f.is_visible = false;
            g.is_visible = true;
            break;
        case 6:
            a.is_visible = true;
            b.is_visible = true;
            c.is_visible = false;
            d.is_visible = true;
            e.is_visible = true;
            f.is_visible = false;
            g.is_visible = true;
            break;
        case 7:
            a.is_visible = false;
            b.is_visible = false;
            c.is_visible = true;
            d.is_visible = false;
            e.is_visible = false;
            f.is_visible = true;
            g.is_visible = true;
            break;
        case 8:
            a.is_visible = true;
            b.is_visible = true;
            c.is_visible = true;
            d.is_visible = true;
            e.is_visible = true;
            f.is_visible = true;
            g.is_visible = true;
            break;
        case 9:
            a.is_visible = true;
            b.is_visible = false;
            c.is_visible = true;
            d.is_visible = true;
            e.is_visible = false;
            f.is_visible = true;
            g.is_visible = true;
            break;
    }
    render_all_segments(position_x, position_y);
}

void render_win(uint16_t offset_x, uint16_t offset_y) {
    // build the W
    reset_segments();
    set_all_segments_color(HSV_GREEN);
    // half the W
    a.is_visible = true;
    b.is_visible = true;
    e.is_visible = true;
    render_all_segments(offset_x, offset_y);
    reset_segments();
    set_all_segments_color(HSV_GREEN);
    // other half of W
    b.is_visible = true;
    e.is_visible = true;
    f.is_visible = true;
    g.is_visible = true;
    render_all_segments(offset_x + SEGMENT_LENGTH, offset_y);
    // render I
    reset_segments();
    set_all_segments_color(HSV_GREEN);
    a.is_visible = true;
    b.is_visible = true;
    render_all_segments(offset_x + (3*SEGMENT_LENGTH), offset_y);
    // render N
    reset_segments();
    set_all_segments_color(HSV_GREEN);    a.is_visible = true;
    b.is_visible = true;
    f.is_visible = true;
    g.is_visible = true;
    render_all_segments(offset_x + (4*SEGMENT_LENGTH), offset_y);
    // render diagonal on N
    for (int i = 0; i < SEGMENT_WIDTH; i++)
    {
        qp_line(lcd, offset_x + (4*SEGMENT_LENGTH) + i, offset_y, offset_x + (5*SEGMENT_LENGTH) + i, offset_y + (2*SEGMENT_LENGTH), a.color.hue, a.color.sat, a.color.val );
    }
}

void render_lose(uint16_t offset_x, uint16_t offset_y) {
    // render L
    reset_segments();
    set_all_segments_color(HSV_RED);
    a.is_visible = true;
    b.is_visible = true;
    e.is_visible = true;
    render_all_segments(offset_x, offset_y);
    // use O for 0
    render_digit(0, offset_x + (2*SEGMENT_LENGTH) , offset_y);
    // use 5 for S
    render_digit(5, offset_x + (4*SEGMENT_LENGTH), offset_y);
    // render E
    reset_segments();
    set_all_segments_color(HSV_RED);
    a.is_visible = true;
    b.is_visible = true;
    c.is_visible = true;
    d.is_visible = true;
    e.is_visible = true;
    render_all_segments(offset_x + (6*SEGMENT_LENGTH), offset_y);

}

bool render_win_lose_if_needed(void) {
    if (pong_state.court.player1.score >= MAX_SCORE) {
        if (is_left) {
            render_win((LCD_WIDTH - (5*SEGMENT_LENGTH))/2, (LCD_HEIGHT - 2*SEGMENT_LENGTH)/2);
        } else {
            render_lose((LCD_WIDTH - (7*SEGMENT_LENGTH))/2,(LCD_HEIGHT - 2*SEGMENT_LENGTH)/2);
        }
        return true;
    } else if (pong_state.court.player2.score >= MAX_SCORE) {
        if (is_left) {
            render_lose((LCD_WIDTH - (7*SEGMENT_LENGTH))/2,(LCD_HEIGHT - 2*SEGMENT_LENGTH)/2);
        } else {
            render_win((LCD_WIDTH - (5*SEGMENT_LENGTH))/2, (LCD_HEIGHT - 2*SEGMENT_LENGTH)/2);
        }
        return true;
    }
    return false;
}

void render_score(void) {
    uint16_t player1_x_offset = SEGMENT_LENGTH, y_offset = SEGMENT_LENGTH;
    uint16_t player2_x_offset = LCD_WIDTH - ((2*SEGMENT_LENGTH) + SEGMENT_WIDTH);

    if (render_win_lose_if_needed()) {
        pong_state.play_complete = true;
        return;
    }
    render_digit(pong_state.court.player1.score, player1_x_offset, y_offset);
    render_digit(pong_state.court.player2.score, player2_x_offset, y_offset);
}

void render_game_pong(void) {
    static uint32_t last_frame = 0;

    // check for conditions that mean we don't need to render
    if (pong_state.play_paused || timer_elapsed32(last_frame) < (1000/FPS_LIMIT)) {
        return;
    }

    if (pong_state.play_complete) {
        render_win_lose_if_needed();
        if (pong_state.needs_reset) pong_reset();
        return;
    }

    last_frame = timer_read32(); // limit frame rate
    pong_player_t *player;
    uint16_t net_x_position, paddle_area_x;

    if(is_left) {
        player = &pong_state.court.player1;
        paddle_area_x = 0;
        net_x_position = LCD_WIDTH - 1;
    } else {
        player = &pong_state.court.player2;
        paddle_area_x = (LCD_WIDTH - 1) - player->paddle.size.width;
        net_x_position = 0;
    }

    //render the screen
    /// 1 - render the court
    render_court(net_x_position);

    if (pong_state.play_starting ) {
        static uint32_t timer = 0;
        static int8_t seconds_remaining = 3;
        if(timer_elapsed32(timer) >= 1000) {
            render_digit(seconds_remaining, LCD_WIDTH / 2, LCD_HEIGHT / 2);
            seconds_remaining--;
            timer = timer_read32();
        }
        if (seconds_remaining < 0) {
            reset_segments();
            render_all_segments( LCD_WIDTH / 2, LCD_HEIGHT / 2);
            pong_state.play_starting = false;
            seconds_remaining = 3;
        }
    } else {
        /// 2 - render the ball
        render_moving_ball();
        /// 3 - render the paddle
        render_paddle(player, paddle_area_x);
        /// 4 - render the current score
        render_score();
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   External interface
// ----------------------------------------------------------------------

void move_paddle(pong_player_t *player, bool up) {
    // check the LCD bounds
    if (player->paddle.position.y > 0 && player->paddle.position.y + player->paddle.size.height < LCD_HEIGHT) {
         player->paddle.position.y += up ? -PADDLE_MOVEMENT_INCREMENT : PADDLE_MOVEMENT_INCREMENT;
    }
}

bool process_record_pong(uint16_t keycode, keyrecord_t *record) {
    pong_player_t *player =  record->event.key.col > 5 ? &pong_state.court.player2 : &pong_state.court.player1;

    if (record->event.pressed) {
        switch (keycode) {
            case KC_UP:
            case KC_MS_UP:
                move_paddle(player, true);
                return false;
            case KC_DOWN:
            case KC_MS_DOWN:
                move_paddle(player, false);
                return false;
            case KC_ENTER:
            case KC_RETURN:
            case KC_MS_BTN1:
                if (pong_state.play_complete) {
                    // if the game is over, reset the court
                    pong_state.needs_reset = true;
                } else {
                    // otherwise, toggle pause
                    pong_state.play_paused = !pong_state.play_paused;
                }
                return false;
            default:
                break;
        }
    }
    return true;
}

void process_encoder_pong(int8_t index, bool clockwise) {
    pong_player_t *player =  index == 0 ? &pong_state.court.player1 : &pong_state.court.player2;
    if (clockwise) {
        move_paddle(player, false);
    } else {
        move_paddle(player, true);
    }
}

void pong_reset(void) {

    // blank the screen
    qp_rect(lcd, 0,0, LCD_WIDTH -1, LCD_HEIGHT -1, HSV_BLACK, true);

    segment_t tmp_seg = {
        .size = {
            .width = SEGMENT_WIDTH,
            .height = SEGMENT_LENGTH
        },
        .position = {
            .x = 0,
            .y = 0
        },
        .is_visible = false
    };
    a=b=c=d=e=f=g = tmp_seg;

    uint8_t color[3] = { BALL_COLOR };
    pong_state.court.ball.color.hue = color[0];
    pong_state.court.ball.color.sat = color[1];
    pong_state.court.ball.color.val = color[2];

    uint8_t pcolor[3] = { PADDLE_COLOR };
    pong_state.court.player1.paddle.color.hue = pcolor[0];
    pong_state.court.player1.paddle.color.sat = pcolor[1];
    pong_state.court.player1.paddle.color.val = pcolor[2];
    pong_state.court.player2.paddle.color.hue = pcolor[0];
    pong_state.court.player2.paddle.color.sat = pcolor[1];
    pong_state.court.player2.paddle.color.val = pcolor[2];

    reset_segments();
    reset_court();

    // Initialize score display segments
    b.position.y = SEGMENT_LENGTH;
    if (is_vertical(&c)) flip_orientation(&c);
    d.position.y = SEGMENT_LENGTH;
    if (is_vertical(&d)) flip_orientation(&d);
    e.position.y = 2 * SEGMENT_LENGTH;
    if (is_vertical(&e)) flip_orientation(&e);
    f.position.x = SEGMENT_LENGTH;
    g.position.x = SEGMENT_LENGTH;
    g.position.y = SEGMENT_LENGTH;

    is_left = is_keyboard_left();
    pong_state.court.player1.score = 0;
    pong_state.court.player2.score = 0;
    pong_state.play_starting = true;
    pong_state.play_complete = false;
    pong_state.play_paused = false;
    pong_state.needs_reset = false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   Game initialization
// ----------------------------------------------------------------------

void pong_init(void) {
    pong_reset();
    transaction_register_rpc(STATE_SYNC_USER_PONG, rpc_update_pong_state);
}
