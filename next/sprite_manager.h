///////////////////////////////////////////////////////////////////////////////
//
//  Bean Brothers (remake) - for ZX Spectrum Next / N-Go
//
//  (c) 2023 David Crespo - https://github.com/dcrespo3d
//                          https://davidprograma.itch.io
//                          https://www.youtube.com/@Davidprograma
//
//  Based on Bean Brothers for ZX Spectrum 48/128K - (c) 2018 Dave Hughes
//
///////////////////////////////////////////////////////////////////////////////
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.  
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
// 
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>. 
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __sprite_manager_h__
#define __sprite_manager_h__

#include "types.h"
#include <stdbool.h>

extern void sprite_setup();
extern void sprite_load_menu_defs();
extern void sprite_load_game_defs();

void sprite_debug_visible();

void sprite_clear_all_slots();
void sprite_clear_all_from(u8 first_slot_to_clear);

void string_append_dec_byte(char* str, u8 val);
void string_append_dec_word(char* str, u16 val);

typedef struct
{
    u8  slot;       // sprite slot
    s16 x;          // x coordinate of sprite
    s16 y;          // y coordinate of sprite
    u8  pal;         // palette offset
    u8  mirrot;      // combined mirror X / mirror Y / rotation
    u8  pat;         // index of current pattern
    u8  scale;      // scale of current sprite 
} SpriteDef;

void __CALLEE__ sprite_update(SpriteDef* spr);

void __CALLEE__ sprite_text_print(SpriteDef* sdef, const char* msg, bool centerX);

#define SCALE_Y1 0x00
#define SCALE_Y2 0x02
#define SCALE_Y4 0x04
#define SCALE_Y8 0x06
#define SCALE_X1 0x00
#define SCALE_X2 0x08
#define SCALE_X4 0x10
#define SCALE_X8 0x18

#endif