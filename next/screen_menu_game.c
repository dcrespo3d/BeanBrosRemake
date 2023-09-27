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

#include "screen_menu_game.h"
#include "types.h"

#include "print_tile.h"
#include "palette.h"
#include "keyb.h"
#include "sprite_manager.h"

#include "screen_menu_main.h"
#include "screen_game.h"
#include "screen_inter.h"
#include "screen_level_select.h"
#include "globaldefs.h"
#include "control.h"

#define CUR_SLOT 0
#define CUR_PATTERN 94
#define SPAL_CUR 10

#define TEXT_SLOT 1
#define SPAL_TEXT 10

#define ST1X 32
#define ST1Y 24
#define ST1 "ORIGINAL LEVELS"

#define ST2X 32
#define ST2Y 72
#define ST2 "SELECT ORIGINAL LEVEL"

#define ST3X 32
#define ST3Y 120
#define ST3 "EXTENDED LEVELS"

#define ST4X 32
#define ST4Y 168
#define ST4 "SELECT EXTENDED LEVELS"

#define ST5X 32
#define ST5Y 216
#define ST5 "BACK TO MENU"

#define TT1X 4
#define TT1Y 6
#define TT1 "play from start to end"

#define TT2X 4
#define TT2Y 12
#define TT2 "play any completed level"

#define TT3X 4
#define TT3Y 18
#define TT3 "play from start to end"

#define TT4X 4
#define TT4Y 24
#define TT4 "play any completed level"

#define TT1X 4
#define TT1Y 6

static void on_fade_update(s8 palval);

static SpriteDef sdef;
static SpriteDef curdef;

static void setup_menu_cursor();
static void update_menu_cursor();

#define MENUX ST1X
#define MENUY ST1Y
#define MENUINC 48

void smg_enter_pg46()
{
    palette_fade_init(SCREEN_TYPE_MENU, on_fade_update);

    sdef.slot = TEXT_SLOT;
    sdef.pal = SPAL_TEXT;

    sdef.x = ST1X;
    sdef.y = ST1Y;
    sprite_text_print(&sdef, ST1, false);

    sdef.x = ST2X;
    sdef.y = ST2Y;
    sprite_text_print(&sdef, ST2, false);

    sdef.x = ST3X;
    sdef.y = ST3Y;
    sprite_text_print(&sdef, ST3, false);

    sdef.x = ST4X;
    sdef.y = ST4Y;
    sprite_text_print(&sdef, ST4, false);

    sdef.x = ST5X;
    sdef.y = ST5Y;
    sprite_text_print(&sdef, ST5, false);

    print_set_pos(TT1X, TT1Y);
    print_str(TT1);

    print_set_pos(TT2X, TT2Y);
    print_str(TT2);

    print_set_pos(TT3X, TT3Y);
    print_str(TT3);

    print_set_pos(TT4X, TT4Y);
    print_str(TT4);

    setup_menu_cursor();
    update_menu_cursor();
}

void smg_update_pg46()
{
    palette_fade_update();
}

static s8 menu_option = 0;

static void setup_menu_cursor()
{
    curdef.slot = CUR_SLOT;
    curdef.pal = SPAL_CUR;
    curdef.pat = CUR_PATTERN;
    curdef.mirrot = ATTR_____;
    menu_option = 0;
}

static void update_menu_cursor()
{
    curdef.x = MENUX - 16;
    curdef.y = MENUY + menu_option * MENUINC;
    sprite_update(&curdef);
}

static void on_fade_update(s8 palval)
{
    if (palval == 0) {
        if (actnDown && !prevDown) {
            menu_option++;
            if (menu_option > 4) menu_option = 0;
        }
        if (actnUp && !prevUp) {
            menu_option--;
            if (menu_option < 0) menu_option = 4;
        }
        update_menu_cursor();

        if (actnSelect && !prevSelect)
            palette_fade_out_begin();
    }
    else if (palval >= 7) {
        switch(menu_option) {
        case 0:
            sg_is_ext_level = false;
            sg_is_select_level = false;
            sg_level_index = 0;
            sg_lives_lost = 0;
            sc_switch_screen(si_enter, si_update,  NULL);
            break;
        case 1: 
            sg_is_ext_level = false;
            sg_is_select_level = true;
            sg_level_index = 0;
            sg_lives_lost = 0;
            sc_switch_screen(sls_enter, sls_update,  NULL);
            break;
        case 2:
            sg_is_ext_level = true;
            sg_is_select_level = false;
            sg_level_index = 20;
            sg_lives_lost = 0;
            sc_switch_screen(si_enter, si_update,  NULL);
            break;
        case 3:
            sg_is_ext_level = true;
            sg_is_select_level = true;
            sg_level_index = 20;
            sg_lives_lost = 0;
            sc_switch_screen(sls_enter, sls_update,  NULL);
            break;
        case 4:
            sc_switch_screen(smm_enter, smm_update, NULL);
            return;
        }
    }
}

