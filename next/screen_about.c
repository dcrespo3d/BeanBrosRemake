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

#include "screen_about.h"
#include "types.h"

#include "print_tile.h"
#include "palette.h"
#include "keyb.h"
#include "joystick.h"

#include "screen_menu_main.h"
#include "globaldefs.h"
#include "sprite_manager.h"

#define TEXT00 "For ZX Spectrum Next (and others)"

#define TEXT02A "         By "
#define TEXT02B "David Crespo Tascon"

#define TEXT03 "(code, graphics, chiptunes)"

#define TEXT05 "davidprograma.itch.io"
#define TEXT06 "youtube.com/@Davidprograma"
#define TEXT07 "github.com/dcrespo3d "

#define TEXT09 "Based on Bean Brothers for Speccy"
#define TEXT10A "   \0032018 "
#define TEXT10B "Dave Hughes"
#define TEXT10C ", with permission"

#define TEXT12 "stonechatproductions.co.uk"

#define TEXT14A "    Theme songs by "
#define TEXT14B "Meteoritos Music"

#define TEXT16 "sptfy.com/meteo"

#define TEXT18 "Developed with these great tools:"

#define TEXT20 "Godot Engine | Z88DK"
#define TEXT21 "Arkos Tracker 2 | Aseprite"

#define TEXT22 "version 1.1 - press any key"

#define NEWLINE print_char('\n')
#define SETDEF  print_set_attr(TPAL_DEF << 4)
#define SETGRAY print_set_attr(TPAL_LGRAY << 4)
#define SETBLUE print_set_attr(TPAL_BLUE << 4)
#define SETRED  print_set_attr(TPAL_RED << 4)

static void on_fade_update(s8 palval);

static SpriteDef sdef;
#define TEXT_SLOT 0
#define TEXTY1 24
#define TEXTS1 "BEAN BROTHERS 2023"

void sab_enter_pg46()
{
    palette_fade_init(SCREEN_TYPE_GAME, on_fade_update);

    sdef.slot = TEXT_SLOT;
    sdef.pal = SPAL_SPFONT;
    sdef.mirrot = ATTR_____;
    sdef.x = 0;
    sdef.y = TEXTY1;
    sprite_text_print(&sdef, TEXTS1, true);

    SETDEF;

    print_set_pos(0, 6);
    println_ctr(TEXT00, 40);
    NEWLINE;

    print_str(TEXT02A);
    SETRED;
    print_str(TEXT02B);
    NEWLINE;
    SETDEF;

    println_ctr(TEXT03, 40);
    NEWLINE;

    SETBLUE;
    println_ctr(TEXT05, 40);
    println_ctr(TEXT06, 40);
    println_ctr(TEXT07, 40);
    NEWLINE;
    SETDEF;

    println_ctr(TEXT09, 40);
    print_str(TEXT10A);
    SETRED;
    print_str(TEXT10B);
    SETDEF;
    print_str(TEXT10C);
    NEWLINE;
    NEWLINE;

    SETBLUE;
    println_ctr(TEXT12, 40);
    NEWLINE;

    SETDEF;
    print_str(TEXT14A);
    SETRED;
    print_str(TEXT14B);
    SETDEF;
    NEWLINE;
    NEWLINE;

    SETBLUE;
    println_ctr(TEXT16, 40);
    SETDEF;
    NEWLINE;
    println_ctr(TEXT18, 40);

    SETRED;
    NEWLINE;
    println_ctr(TEXT20, 40);
    println_ctr(TEXT21, 40);
    SETDEF;

    print_set_pos(0, 29);
    println_ctr(TEXT22, 40);

    // removed white frame
    // SETGRAY;
    // print_set_symbol(129);
    // print_frame(0, 1, 40, 31);
    // SETDEF;
}

void sab_update_pg46()
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

