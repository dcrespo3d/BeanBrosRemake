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

#pragma codeseg PAGE_46_MENUS
#pragma constseg PAGE46_MENUS
#pragma bssseg PAGE46_MENUS

#include "screen_level_select.h"
#include "types.h"

#include "print_tile.h"
#include "palette.h"
#include "keyb.h"
#include "sprite_manager.h"

#include "screen_menu_main.h"
#include "screen_menu_game.h"
#include "screen_inter.h"
#include "screen_game.h"
#include "globaldefs.h"
#include "control.h"

#define CUR_SLOT 0
#define CUR_PATTERN 94
#define SPAL_CUR 10

#define TEXT_SLOT 1
#define SPAL_TEXT 10

#define SPAL_LOCK 9
#define LOCK_PATTERN 63

#define ST1X 40
#define ST1Y 16
#define ST1A "SELECT ORIGINAL LEVEL"
#define ST1B "SELECT EXTENDED LEVEL"

#define ST2X 40
#define ST2Y 216
#define ST2 "BACK TO MENU"

static void on_fade_update(s8 palval);

static SpriteDef sdef;
static SpriteDef curdef;

static void setup_menu_cursor();
static void update_menu_cursor();

#define MENUX 40
#define MENUY 56
#define MENUDX 56
#define MENUDY 40


void sls_enter_pg46()
{
    u8 threshold;

    palette_fade_init(SCREEN_TYPE_MENU, on_fade_update);

    sdef.slot = TEXT_SLOT;
    sdef.pal = SPAL_TEXT;

    sdef.x = ST1X;
    sdef.y = ST1Y;
    if (!sg_is_ext_level) {
        sprite_text_print(&sdef, ST1A, false);
        threshold = sg_ful_ori;
    }
    else {
        sprite_text_print(&sdef, ST1B, false);
        threshold = sg_ful_ext - 20;
    }

    sdef.x = ST2X;
    sdef.y = ST2Y;
    sprite_text_print(&sdef, ST2, false);

    u8 idx = 0;
    char numstr[3];
    for (u8 y = 0; y < 4; y++) {
        sdef.y = MENUY + MENUDY * y;
        for (u8 x = 0; x < 5; x++) {
            if (idx <= threshold) {
                sdef.x = MENUX + MENUDX * x;
                if (idx < 9) sdef.x += 2;
                else if (idx < 19) sdef.x -= 6;
                else               sdef.x -= 4; 
                numstr[0] = 0;
                numstr[1] = 0;
                numstr[2] = 0;
                string_append_dec_byte(numstr, 1+idx);
                sprite_text_print(&sdef, numstr, false);
            }
            else {
                sdef.slot++;
                sdef.x = MENUX + MENUDX * x;
                sdef.pal = SPAL_LOCK;
                sdef.pat = LOCK_PATTERN;
                sprite_update(&sdef);
            }
            idx++;
        }
    }
    setup_menu_cursor();
    update_menu_cursor();
}

void sls_update_pg46()
{
    palette_fade_update();
}

static s8 optX = 0;
static s8 optY = 0;

static void setup_menu_cursor()
{
    curdef.slot = CUR_SLOT;
    curdef.pal = SPAL_CUR;
    curdef.pat = CUR_PATTERN;
    curdef.mirrot = ATTR_____;
    optX = optY = 0;

    u8 levidx = sg_level_index;
    if (sg_is_ext_level) levidx -= 20;
    // print_set_pos(36,0); print_dec_byte(levidx);
    while (levidx) {
        optX++;
        if (optX > 4) {
            optX = 0;
            optY++;
        }
        levidx--;
    }
}

static void update_menu_cursor()
{
    curdef.x = MENUX + MENUDX * optX - 16;
    curdef.y = MENUY + MENUDY * optY;
    sprite_update(&curdef);
}

static bool canGo()
{
	if (optY == 4)
		return true;
	u8 levnum = optX + optY * 5;
    if (!sg_is_ext_level)
		return levnum <= sg_ful_ori;
    else
        return levnum <= sg_ful_ext - 20;
    return false;
}


static void on_fade_update(s8 palval)
{
    if (palval == 0) {
        if (actnLeft && !prevLeft) {
            optX--;
            if (optX < 0) optX = 4;
        }
        if (actnRight && !prevRight) {
            optX++;
            if (optX > 4) optX = 0;
        }
        if (actnUp && !prevUp) {
            optY--;
            if (optY < 0) optY = 4;
        }
        if (actnDown && !prevDown) {
            optY++;
            if (optY > 4) optY = 0;
        }
        if (optY == 4) optX = 0;
        update_menu_cursor();

        if (actnSelect && !prevSelect && canGo())
            palette_fade_out_begin();
    }
    else if (palval >= 7) {
        if (optY < 4) {
            u8 levnum = optX + optY * 5;
            sg_level_index = levnum;
            if (sg_is_ext_level)
                sg_level_index += 20;
            sc_switch_screen(sg_enter, sg_update, NULL);
        }
        else {
            sc_switch_screen(smg_enter, smg_update, NULL);
        }
    }
}

