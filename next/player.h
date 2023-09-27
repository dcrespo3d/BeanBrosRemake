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

#ifndef __player_h__
#define __player_h__

#include <stdbool.h>
#include "types.h"
#include "anisprite.h"
#include "screen_game.h"
#include "collrect.h"

typedef struct
{
    AniSprite asdef;
    u8 cx;      // cell X  [0, 40]
    s8 dx;      // disp X  [0, 7]
    s8 vx;
    u8 cy;      // cell Y  [0, 32]
    s8 dy;      // disp Y  [0, 7]
    s8 vy;

    bool active;
    bool airborne;
    bool dead;
    u8 jumpidx;
    u8 crumb_ctr;
} Player;

extern Player player_smarto;
extern Player player_dumber;

void players_setup(LevelDef* levdef);
void players_update();

void player_kill(Player* player);
void player_get_collrect(Player* player, CollRect* dst);

bool player_into_exit(Player* player);

#endif

