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

#ifndef __print_tile_h__
#define __print_tile_h__

#include "types.h"

// button border
#define BLOCK_BUTTON_BORDER 141
#define BLOCK_BCUR 141
#define BLOCK_BSTR 142

// board border
#define BLOCK_BOARD_BORDER 143
#define BLOCK_DCUR 143
#define BLOCK_DSTR 144

#define BLOCK_FIX1 145
#define BLOCK_FIX2 146

// fine border
#define BLOCK_FINE_BORDER 147
#define BLOCK_FCUR 147
#define BLOCK_FSTR 148

// outer border
#define BLOCK_OUTER_BORDER 149
#define BLOCK_OCUR 149
#define BLOCK_OSTR 150

#define BLOCK_STAR 151

#define DBT_OFFSET 128   // 152 - 32

// attributes for mirror X, mirror Y, Rotation
#define ATTR_____ 0
#define ATTR___R_ 2
#define ATTR__Y__ 4
#define ATTR__YR_ 6
#define ATTR_X___ 8
#define ATTR_X_R_ 10
#define ATTR_XY__ 12
#define ATTR_XYR_ 14

void print_set_pos(ubyte x, ubyte y);
void print_set_attr(ubyte attr);
void print_set_color(ubyte col);
void print_set_symbol(ubyte symbol);

void print_symbol();

void print_char(char ch);
void print_block(ubyte blkidx);
void print_str(const char* str);
void print_hex_word(uword val);
void print_dec_word(uword val);
void print_hex_byte(ubyte val);
void print_dec_byte(ubyte val);
void print_hex_nibble(ubyte val);

// pos, color and symbol must be set prior to calling this
void print_frame(ubyte x, ubyte y, ubyte w, ubyte h);

void print_rect_symbol(ubyte x, ubyte y, ubyte w, ubyte h);

void print_frame_filled(ubyte x, ubyte y, ubyte w, ubyte h);

void print_set_pos_inc(sbyte x, sbyte y);
void println(const char* txt);
void println_ctr(const char* txt, ubyte len);

#endif // __print_tile_h__