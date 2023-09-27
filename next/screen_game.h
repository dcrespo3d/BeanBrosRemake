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

#ifndef __screen_game_h__
#define __screen_game_h__

#include "types.h"
#include "screen_controller.h"

void sg_enter();
void sg_update();

void sg_on_player_killed();
void sg_decompress_tilemap(u8* levelmap);

// called from pause, restarts level killing player
void sg_restart();
// called from pause, goes back to main menu
void sg_exit();

extern u8 sg_level_index;

extern u16 sg_lives_lost;

#define DEF_FUL_ORI 0
#define DEF_FUL_EXT 20

// first unlocked levels (ful)
extern u8 sg_ful_ori;
extern u8 sg_ful_ext;

extern bool sg_is_ext_level;
extern bool sg_is_select_level;

typedef struct {
    u8 type;
    u8 ini_cx;
    u8 ini_cy;
    s8 extra1;
    s8 extra2;
    s8 extra3;
} ObjectDef;

typedef struct {
    u8 smarto_ini_cx;
    u8 smarto_ini_cy;
    u8 smarto_ini_lookleft;
    u8 dumber_ini_cx;
    u8 dumber_ini_cy;
    u8 dumber_ini_lookleft;
    u8 enemy_count;
    ObjectDef enemy_defs[];
} LevelDef;

#endif // __screen_game_h__
