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

#ifndef __enemy_h__
#define __enemy_h__

#include <stdbool.h>
#include "types.h"
#include "anisprite.h"
#include "screen_game.h"
#include "collrect.h"

typedef struct
{
    AniSprite asdef;
    u8 advmask;
    u16 xmin;
    u16 xmax;
    s8  deltax;
    u8  fctr;   // frame counter
} EnemyHoriz;

typedef struct
{
    AniSprite asdef;
    u8 advmask;
    u16 ymin;
    u16 ymax;
    s8  deltay;
    u8  fctr;   // frame counter
} EnemyVert;

typedef struct
{
    AniSprite asdef;
    bool active;
    u8 fctr;    // frame counter
} EnemySpikes;

typedef struct
{
    AniSprite asdef;
    s8 deltax;
    s8 deltay;
    u8 fperx;
    u8 fpery;
    u8 fctrx;
    u8 fctry;

} EnemyDiag;

typedef struct
{
    AniSprite asdef;
    bool closed;
} ObjectDoor;

typedef struct
{
    AniSprite asdef;
    bool closed;
    bool touch;
} ObjectLever;

typedef struct
{
    u8 cx1;
    u8 cx2;
    u8 cy;
    bool left;
    u8 idx;
} ObjectConv;


void objects_setup(LevelDef* levdef);
void objects_update();

u8 enemy_get_count();
bool enemy_get_rect(u8 index, CollRect* r);

u8 lever_get_count();
bool lever_get_rect(u8 index, CollRect* r);
ObjectLever* lever_get(u8 index);
void lever_move();


#endif