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


#include "serial.h"
#ifdef CONFIG_MENU_ENABLE
#   include "features/config_menu/menu.h"
#endif
#ifdef PONG_ENABLE
#   include "games/pong/pong.h"
#endif
#include "graphics/gif/gif.h"
#include "graphics/logo_color.h"
#include "fonts/thintel15.h"
#include "display.h"

#include <stdio.h>
#include <print.h>

GIFIMAGE logo_gif;
bool logo_loaded = false;
bool more_frames = true;
lcd_state_t lcd_state = { .current_state = INIT, .previous_state = INIT, .transition_complete = false };

render_loc_t render_loc;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   Implementation of state sync between primary/secondary sides
// ----------------------------------------------------------------------

void rpc_sync_display_state(void) {
    if (is_keyboard_master()) {
        // Keep track of the last state, so that we can tell if we need to propagate to secondary side
        static lcd_state_t      last_lcd_state;
        static uint32_t         last_sync;
        bool                    needs_sync = false;

       // Check if the pong court state values are different
        if (memcmp(&lcd_state, &last_lcd_state, sizeof(lcd_state_t))) {
            needs_sync = true;
            memcpy(&last_lcd_state, &lcd_state, sizeof(lcd_state_t));
        }

        // Send to secondary side periodically regardless of state change (period defined in config.h)
        if (timer_elapsed32(last_sync) > MAX_SYNC_INTERVAL_MS) {
            needs_sync = true;
        }

        // Perform the sync if requested
        if (needs_sync) {
            char result_string[RPC_S2M_BUFFER_SIZE];
                int result = transaction_rpc_exec(STATE_SYNC_USER_DISPLAY, sizeof(lcd_state_t), &lcd_state, RPC_S2M_BUFFER_SIZE, result_string);
            if ( result) {
               last_sync = timer_read32();

            }
#ifdef CONSOLE_ENABLE
            else {
               dprintf("Failed to sync display state with result 0x%04x\n", result);
            }
            dprint(result_string);
#endif
        }
    }
}

void rpc_update_display_state(uint8_t initiator2target_buffer_size, const void* initiator2target_buffer, uint8_t target2initiator_buffer_size, void* target2initiator_buffer) {
    if (initiator2target_buffer_size == sizeof(lcd_state_t)) {
        sprintf(target2initiator_buffer, "- updating lcd_state -\n");
        memcpy(&lcd_state, initiator2target_buffer, sizeof(lcd_state_t));
    } else {
        sprintf(target2initiator_buffer, "XXX invalid transaction size 0x%02x XXX\n", initiator2target_buffer_size);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   State manipulation functions
// ----------------------------------------------------------------------

void set_display_state(display_state new_state) {
    lcd_state.previous_state = lcd_state.current_state;
    lcd_state.current_state = new_state;
    lcd_state.transition_complete = false;
}

display_state get_display_state(void) {
    return lcd_state.current_state;
}

void revert_display_state(void) {
    display_state tmp_state = lcd_state.current_state;
    lcd_state.current_state = lcd_state.previous_state;
    lcd_state.previous_state = tmp_state;
    lcd_state.transition_complete = false;
}

void reset_display_state(void) {
    lcd_state.previous_state = lcd_state.current_state;
    lcd_state.current_state = INIT;
    lcd_state.transition_complete = false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   Menu rendering functions
// ----------------------------------------------------------------------

void display_blank(void) {
    dprint("---- Blanking the screen ----");
    for (uint16_t j = LCD_WIDTH; j > 0; j--)
    {
        qp_rect(lcd, j-1, 0, j-1, LCD_HEIGHT - 1, HSV_BLACK, true);
    }
}

void draw_gif_frame(GIFDRAW* pDraw) {
        debug_enable = true;
    if (pDraw->ucHasTransparency) {
        uint16_t pixels[pDraw->iWidth];
        uint16_t pixelCount = 0;
        for(int i = 0; i < pDraw->iWidth; i++){
            if (pDraw->pPixels[i] == pDraw->ucTransparent)
            {
                // open a viewport that is only as large as the collected pixels so far, starting at the last non-transparent pixel
                if (pixelCount > 0) {
                    qp_viewport(lcd, render_loc.x + pDraw->iX + (i - pixelCount), render_loc.y + pDraw->iY + pDraw->y, render_loc.x + pDraw->iX + i, render_loc.y + pDraw->iY + pDraw->y);
                    qp_pixdata(lcd, pixels, pixelCount*2);
                    pixelCount = 0;
                }
            } else {
                // not a transparent pixel, append it to the pixels array
                pixels[pixelCount] = pDraw->pPalette[pDraw->pPixels[i]];
                pixelCount++;
            }
        }
        // render any last pixels.
        if (pixelCount > 0) {
            qp_viewport(lcd, render_loc.x + pDraw->iX + (pDraw->iWidth - pixelCount), render_loc.y + pDraw->iY + pDraw->y, render_loc.x + pDraw->iX + pDraw->iWidth, render_loc.y + pDraw->iY + pDraw->y);
            pixelCount = 0;
            qp_pixdata(lcd, pixels, pixelCount);
        }
    } else {
        uint16_t pixels[pDraw->iWidth];
        for(int i = 0; i < pDraw->iWidth; i++){
            pixels[i] = pDraw->pPalette[pDraw->pPixels[i]];
        }
        qp_viewport(lcd, render_loc.x + pDraw->iX, render_loc.y + pDraw->iY + pDraw->y, render_loc.x + pDraw->iX + pDraw->iWidth, render_loc.y + pDraw->iY + pDraw->y);
        qp_pixdata(lcd, pixels, pDraw->iWidth);
    }
}

void render_logo(void) {
    if(!logo_loaded || !more_frames) return;

    static uint32_t next_frame = 0;
    uint32_t now = timer_read32();

    if(more_frames && now >= next_frame) {
        dprint("-- render a frame --\n");
        static int last_frame_delay = 0;
        int frame_delay;
        more_frames = GIF_playFrame(&logo_gif, &frame_delay);
        last_frame_delay = frame_delay;
        next_frame = timer_read32() + last_frame_delay;
    }
    else
    {
        dprint("-- no more frames --\n");
    }
    qp_drawtext(lcd,0,305,font_thintel15,"Wing70");
}

void reset_logo(void) {
    GIF_reset(&logo_gif);
}

void housekeeping_task_render_display(void) {
    switch(lcd_state.current_state) {
        case INIT:
            dprint("housekeeping render INIT\n");
            set_display_state(LOGO);
        case LOGO:
            if (!lcd_state.transition_complete) {
                display_blank();
                more_frames = true;
            }
            reset_logo();
            render_logo();
            break;
#ifdef CONFIG_MENU_ENABLE
        case MENU_CONFIG:
             if (!lcd_state.transition_complete) {
                display_blank();
                set_menu_dirty_bit(true);
            }
            render_menu();
            break;
#endif // CONFIG_MENU_ENABLE
#ifdef PONG_ENABLE
        case PONG:
             if (!lcd_state.transition_complete) {
                display_blank();
            }
            render_game_pong();
            break;
#endif // PONG_ENABLE
        default:
            break;
    }
    lcd_state.transition_complete = true;
}


void keyboard_post_init_display(void) {
    debug_enable = true;
    display_blank();
    uint8_t bllevel = BACKLIGHT_LEVELS;
    do
    {
        backlight_level(bllevel);
        wait_ms(20);
        bllevel = get_backlight_level() -1;
    } while (bllevel > 0);

    backlight_level((uint8_t)(BACKLIGHT_LEVELS));

    logo_loaded = GIF_openRAM(&logo_gif, (uint8_t *)logo_color_gif, logo_color_gif_size, &draw_gif_frame);
    qp_drawtext_recolor(lcd, 0, 300, font_thintel15, "Wing70", HSV_WHITE, HSV_BLACK);

    // register an RPC callback to update the display state
    transaction_register_rpc(STATE_SYNC_USER_DISPLAY, rpc_update_display_state);

}


