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

#include <arch/zxn.h>
#include "screen_info2.h"
#include "types.h"

#include "print_tile.h"
#include "palette.h"
#include "keyb.h"
#include "joystick.h"
#include "levels.h"
#include "music_player.h"
#include "player.h"
#include "object.h"
#include "anisprite.h"

#include "screen_info3.h"
#include "globaldefs.h"

static void on_fade_update(s8 palval);

static u8* levmap;
static LevelDef* levdef;
static void decompress_tilemap();
extern u8 tileset_palgroup[];
#define DEMO_LEVEL 6

#define TEXT1X 6
#define TEXT1Y 7
#define TEXT1A "1) GET BROS >"
#define TEXT1B "TO THE EXIT >"

#define TEXT2X 12
#define TEXT2Y 14
#define TEXT2A "2) AVOID"
#define TEXT2B "BAD GUYS"

#define TEXT3X 26
#define TEXT3Y 10
#define TEXT3A "3) USE ^"
#define TEXT3B "SWITCHES"

#define TEXT4X 14
#define TEXT4Y 23
#define TEXT4A "4) WORK TOGETHER"
#define TEXT4B "TO SOLVE PUZZLES"

extern u8 collmap[];

void si2_enter()
{
    palette_fade_init(SCREEN_TYPE_GAME, on_fade_update);

    // setup page for levels
    ZXN_WRITE_REG(0x50, 34);
    ZXN_WRITE_REG(0x51, 35);

    levmap = levmap_array[DEMO_LEVEL];
    levdef = levdef_array[DEMO_LEVEL];

    AVOID_MUSIC_STUTTER

    sg_decompress_tilemap(levmap);

    AVOID_MUSIC_STUTTER

    players_setup(levdef);

    AVOID_MUSIC_STUTTER

    objects_setup(levdef);

    players_update();
    objects_update();
    lever_move();

    // manual collision
    collmap[30+40*25] = 3;

    print_set_symbol(132);
    print_frame(TEXT1X-1, TEXT1Y-1, 15, 4);
    print_set_pos(TEXT1X, TEXT1Y);
    print_str(TEXT1A);
    print_set_pos(TEXT1X, TEXT1Y+1);
    print_str(TEXT1B);

    print_set_symbol(132);
    print_frame(TEXT2X-1, TEXT2Y-1, 10, 4);
    print_set_pos(TEXT2X, TEXT2Y);
    print_str(TEXT2A);
    print_set_pos(TEXT2X, TEXT2Y+1);
    print_str(TEXT2B);

    print_set_symbol(132);
    print_frame(TEXT3X-1, TEXT3Y-1, 10, 4);
    print_set_pos(TEXT3X, TEXT3Y);
    print_str(TEXT3A);
    print_set_pos(TEXT3X, TEXT3Y+1);
    print_str(TEXT3B);

    print_set_symbol(132);
    print_frame(TEXT4X-1, TEXT4Y-1, 18, 4);
    print_set_pos(TEXT4X, TEXT4Y);
    print_str(TEXT4A);
    print_set_pos(TEXT4X, TEXT4Y+1);
    print_str(TEXT4B);

    print_set_pos(0, 29);
    println_ctr(MSG_PRESS_ANY_KEY, 40);

}


void si2_update()
{
    palette_fade_update();
}

static bool prev_any, curr_any;

static void on_fade_update(s8 palval)
{
    anisprite_tick();

    players_update();
    objects_update();

    if (palval < 0)
    {
        prev_any = keyb_any() || joystick_any();
    }
    else if (palval == 0)
    {
        curr_any = keyb_any() || joystick_any();
        if (!prev_any && curr_any) {
            palette_fade_out_begin();
        }
        prev_any = curr_any;
    }
    else if (palval >= 7) {
        sc_switch_screen(si3_enter, si3_update, NULL);
    }
}
