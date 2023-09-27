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

#ifndef __anisprite_h__
#define __anisprite_h__

#include "types.h"

typedef struct
{
    u8 start;   // index of first animation pattern
    u8 end;     // index of last animation pattern
    u8 dur;     // animation pattern duration, in frames (fps = 50 / dur)
    u8 loop;    // loop animation (or else, stay on last frame)
} ASAnim;

typedef struct
{
    u8  slot;       // sprite slot
    u16 x;          // x coordinate of sprite
    u16 y;          // y coordinate of sprite
    u8  pal;        // palette offset
    u8  mirrot;     // combined mirror X / mirror Y / rotation
    u8  pat;        // index of current pattern
    u8  scale;      // scale of current sprite 

    // until here, same fields and structure as SpriteDef

    u8  anim;        // index of current animation index
    u16 frame;       // frame of last pattern change
    ASAnim *animdefs;   // definitions for anims
} AniSprite;

void anisprite_tick();

void __CALLEE__ anisprite_update(AniSprite* asdef);

void __CALLEE__ anisprite_set_anim(AniSprite* asdef, u8 anim);

#endif