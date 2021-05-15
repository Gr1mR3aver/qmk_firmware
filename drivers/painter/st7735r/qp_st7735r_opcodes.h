/* Copyright 2020 Paul Cotter (@Gr1mR3aver)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Quantum Painter ST7735R command opcodes
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Panel Function Commands

#define ST7735R_SET_FRAME_CTL_NORMAL 0xB1    // Set frame ctl (normal)
#define ST7735R_SET_FRAME_CTL_IDLE 0xB2      // Set frame ctl (idle)
#define ST7735R_SET_FRAME_CTL_PARTIAL 0xB3   // Set frame ctl (partial)
#define ST7735R_SET_INVERSION_CTL 0xB4       // Set inversion ctl
#define ST7735R_SET_FUNCTION_CTL 0xB6        // Set function ctl
#define ST7735R_SET_POWER_CTL_1 0xC0         // Set power ctl 1
#define ST7735R_SET_POWER_CTL_2 0xC1         // Set power ctl 2
#define ST7735R_SET_POWER_CTL_NORMAL 0xC2    // Set power ctl 3
#define ST7735R_SET_POWER_CTL_IDLE 0xC3      // Set power ctl 4
#define ST7735R_SET_POWER_CTL_PARTIAL 0xC4   // Set power ctl 5
#define ST7735R_SET_VCOM_CTL_1 0xC5          // Set VCOM ctl 1
#define ST7735R_SET_VCOM_OFFSET 0xC7         // Set VCOM offset ctl
#define ST7735R_SET_ID2 0xD1                 // Get ID2
#define ST7735R_SET_ID3 0xD2                 // Get ID3
#define ST7735R_SET_POWER_CTL_6 0xFC         // Set power ctl 6
#define ST7735R_CTL_NVMEM 0xD9               // Enable NVMEM access
#define ST7735R_GET_NVMEM 0xDE               // Get NVMEM data
#define ST7735R_SET_NVMEM 0xDF               // Set NVMEM data
#define ST7735R_SET_PGAMMA 0xE0              // Set positive gamma
#define ST7735R_SET_NGAMMA 0xE1              // Set negative gamma
#define ST7735R_SET_EXT_CTL 0xF0             // Set External Control Extension
#define ST7735R_SET_VCOM4_LVL 0xFF           // Set VCOM4 level
