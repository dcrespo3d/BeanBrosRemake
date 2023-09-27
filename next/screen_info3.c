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

#include "screen_info3.h"
#include "types.h"

#include "print_tile.h"
#include "palette.h"
#include "keyb.h"
#include "joystick.h"
#include "sprite_manager.h"
#include "globaldefs.h"

#include "screen_menu_main.h"

static void on_fade_update(s8 palval);

static SpriteDef sdef;
#define TEXT_SLOT 0
#define TEXTY1 32
#define TEXT1 "20 ORIGINAL LEVELS"

#define TEXTY2 7
#define TEXT2 "identical to ZX Spectrum levels"

#define TEXT_SLOT 0
#define TEXTY3 80
#define TEXT3 "20 EXTENDED LEVELS"

#define TEXTY4 13
#define TEXT4 "exclusive for ZX Spectrum Next"

#define TEXT_SLOT 0
#define TEXTY5 128
#define TEXT5 "INFINITE LIVES"

#define TEXTY6 19
#define TEXT6 "die and repeat until you win!"

#define TEXT_SLOT 0
#define TEXTY7 176
#define TEXT7 "LEVEL BROWSER"

#define TEXTY8 25
#define TEXT8 "replay visited levels"

void si3_enter_pg46()
{
    palette_fade_init(SCREEN_TYPE_GAME, on_fade_update);

    sdef.slot = TEXT_SLOT;
    sdef.pal = SPAL_SPFONT;
    sdef.mirrot = ATTR_____;
    sdef.x = 0;
    sdef.y = TEXTY1;
    sprite_text_print(&sdef, TEXT1, true);

    print_set_pos(0, TEXTY2);
    println_ctr(TEXT2, 40);

    sdef.y = TEXTY3;
    sprite_text_print(&sdef, TEXT3, true);

    print_set_pos(0, TEXTY4);
    println_ctr(TEXT4, 40);

    sdef.y = TEXTY5;
    sprite_text_print(&sdef, TEXT5, true);

    print_set_pos(0, TEXTY6);
    println_ctr(TEXT6, 40);

    sdef.y = TEXTY7;
    sprite_text_print(&sdef, TEXT7, true);

    print_set_pos(0, TEXTY8);
    println_ctr(TEXT8, 40);

    print_set_pos(0, 29);
    println_ctr(MSG_PRESS_ANY_KEY, 40);
}

void si3_update_pg46()
{
    palette_fade_update();
}

static bool prev_any, curr_any;

static void on_fade_update(s8 palval)
{
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
        sc_switch_screen(smm_enter, smm_update, NULL);
    }
}

