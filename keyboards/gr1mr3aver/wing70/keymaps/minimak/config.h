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

#define PONG_ENABLE
#define CATERPILLAR_ENABLE
#define SPACE_INVADERS_ENABLE
#define CONFIG_MENU_ENABLE

// Allow for an extra sync command over the split
#define SERIAL_USE_MULTI_TRANSACTION
#define SPLIT_NUM_TRANSACTIONS_USER 4
#define SPLIT_TRANSACTION_IDS_USER STATE_SYNC_USER, STATE_SYNC_USER_PONG, STATE_SYNC_USER_MENU, STATE_SYNC_USER_DISPLAY

// Master to slave buffer size
#define RPC_M2S_BUFFER_SIZE 64
// Slave to master buffer size
#define RPC_S2M_BUFFER_SIZE 64


// Maximum time between state synchronizations across the halves
#define MAX_SYNC_INTERVAL_MS 500

#define RGB_MATRIX_STARTUP_MODE RGB_MATRIX_TYPING_HEATMAP
