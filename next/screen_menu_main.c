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

#include "types.h"
#include <z80.h>
#include <string.h>

#include "print_tile.h"
#include "palette.h"
#include "sprite_manager.h"

#include "keyb.h"
#include "joystick.h"
#include "control.h"

#include "music_player.h"

#include "screen_game.h"
#include "screen_inter.h"
#include "screen_info1.h"
#include "screen_info2.h"
#include "screen_control.h"
#include "screen_about.h"
#include "screen_menu_game.h"
#include "layer2.h"
#include "paging.h"
#include "globaldefs.h"
#include "screen_debug_keyboard.h"
#include "persist.h"

// images
extern u8 layer2_img_title_bean[];
extern u8 layer2_img_title_brothers[];

#define SMARTO_X 24
#define SMARTO_Y 156
#define DUMBER_X 232
#define DUMBER_Y 156

#define SPAL_HEAD 0
#define SPAL_FEET 1
#define SPAL_TEXT 10
#define SMARTO_FEET_SLOT 0
#define SMARTO_HEAD_SLOT 4
#define SMARTO_HEAD_PATTERN 0
#define SMARTO_FEET_PATTERN 32
#define DUMBER_FEET_SLOT 20
#define DUMBER_HEAD_SLOT 24
#define DUMBER_HEAD_PATTERN 16
#define DUMBER_FEET_PATTERN 36
#define TEXT_SLOT 41

#define CUR_SLOT 40
#define CUR_PATTERN 94
#define SPAL_CUR 10

static SpriteDef sdef;

static void update_smarto(s8 dy);

static void setup_smarto()
{
    sdef.slot = SMARTO_FEET_SLOT;
    sdef.pal = SPAL_FEET;
    sdef.pat = SMARTO_FEET_PATTERN;
    sdef.mirrot = ATTR_____;
    sdef.x = SMARTO_X;  
    sdef.y = SMARTO_Y + 48;
    for (u8 x = 0; x < 4; x++) {
        sprite_update(&sdef);
        sdef.slot++;
        sdef.pat++;
        sdef.x += 16;
    }

    update_smarto(0);
}

static void update_smarto(s8 dy)
{
    sdef.slot = SMARTO_HEAD_SLOT;
    sdef.pal = SPAL_HEAD;
    sdef.pat = SMARTO_HEAD_PATTERN;
    sdef.mirrot = ATTR_____;
    sdef.x = SMARTO_X;
    sdef.y = SMARTO_Y + dy;
    for (u8 y = 0; y < 4; y++) {
        for (u8 x = 0; x < 4; x++) {
            sprite_update(&sdef);
            sdef.slot++;
            sdef.pat++;
            sdef.x += 16;
        }
        sdef.x -= 64;
        sdef.y += 16;
    }
}

static void update_dumber(s8 dy);

static void setup_dumber()
{
    SpriteDef sdef;

    sdef.slot = DUMBER_FEET_SLOT;
    sdef.pal = SPAL_FEET;
    sdef.pat = DUMBER_FEET_PATTERN;
    sdef.mirrot = ATTR_____;
    sdef.x = DUMBER_X;
    sdef.y = DUMBER_Y + 48;
    for (u8 x = 0; x < 4; x++) {
        sprite_update(&sdef);
        sdef.slot++;
        sdef.pat++;
        sdef.x += 16;
    }

    update_dumber(0);
}

static void update_dumber(s8 dy)
{
    sdef.slot = DUMBER_HEAD_SLOT;
    sdef.pal = SPAL_HEAD;
    sdef.pat = DUMBER_HEAD_PATTERN;
    sdef.mirrot = ATTR_____;
    sdef.x = DUMBER_X;
    sdef.y = DUMBER_Y + dy;
    for (u8 y = 0; y < 4; y++) {
        for (u8 x = 0; x < 4; x++) {
            sprite_update(&sdef);
            sdef.slot++;
            sdef.pat++;
            sdef.x += 16;
        }
        sdef.x -= 64;
        sdef.y += 16;
    }
}

static SpriteDef curdef;

static void setup_menu_cursor();
static void update_menu_cursor();

static void on_fade_update(s8 palval);

static bool title_loaded = false;

void smm_enter()
{
    palette_fade_init(SCREEN_TYPE_MENU, on_fade_update);
    setup_smarto();
    setup_dumber();
    setup_menu_cursor();
    update_menu_cursor();

#define MENUX 120
#define MENUY 144
#define MENUINC 20

//               123456789ABCDEF
#define OP1TEXT "START"
#define OP2TEXT "INFO"
#define OP3TEXT "CONTROL"
#define OP4TEXT "ABOUT"

    sdef.slot = TEXT_SLOT;
    sdef.pal = SPAL_TEXT;
    sdef.mirrot = ATTR_____;
    sdef.x = MENUX;
    sdef.y = MENUY;
    sprite_text_print(&sdef, OP1TEXT, false);

    sdef.slot++;
    sdef.x = MENUX;
    sdef.y += MENUINC;
    sprite_text_print(&sdef, OP2TEXT, false);

    sdef.slot++;
    sdef.x = MENUX;
    sdef.y += MENUINC;
    sprite_text_print(&sdef, OP3TEXT, false);

    sdef.slot++;
    sdef.x = MENUX;
    sdef.y += MENUINC;
    sprite_text_print(&sdef, OP4TEXT, false);

/*
__asm
    EXTERN  readNextReg

    ; enable Layer 2
    LD      A, $69
    CALL    readNextReg
    OR      A, %1000000
    NEXTREG $69, A

    ; set Layer 2 resolution to 256 x 192
    LD      A, $70
    CALL    readNextReg
    AND     A, %11001111
    NEXTREG $70, A

    ; set Layer 2 starting page to 8
    NEXTREG $12, 8

    ; map Layer 2 first page to slots 0 and 1
    NEXTREG $50, 16
    NEXTREG $51, 17
__endasm
*/
    AVOID_MUSIC_STUTTER;

    if (!title_loaded) {
        title_loaded = true;

        z80_outp(0x123b,0x03);
        layer2_fill_page(0);

        z80_outp(0x123b,0x43);
        layer2_fill_page(0);

        z80_outp(0x123b,0x83);
        layer2_fill_page(0);

        // AVOID_MUSIC_STUTTER;

        storePrevPagesAtSlots2and3();

        putPagesAtSlots2and3(40);

        z80_outp(0x123b,0x03);

        layer2_uncompress_image(layer2_img_title_bean, 0);

        // AVOID_MUSIC_STUTTER;

        z80_outp(0x123b,0x43);

        layer2_uncompress_image(layer2_img_title_brothers, 0);

        // remove layer 2 mapping, leaving layer 2 enabled (visible)
        z80_outp(0x123b,0x02);

        restorePrevPagesAtSlots2and3();
    }
    else {
        // remove layer 2 mapping, leaving layer 2 enabled (visible)
        z80_outp(0x123b,0x02);
    }

    // u8 y = 0;
    // print_set_pos(36,y++); print_hex_word(song_meteo);
    // print_set_pos(36,y++); print_hex_word(song_lonely);
    // print_set_pos(36,y++); print_hex_word(song_revolution);
    // print_set_pos(36,y++); print_hex_word(song_leaving);

    // setup music
    music_song_init(song_meteo);

    print_set_pos(5, 29);
    print_str("keys: ");
    print_char(keyb_ch4short[keyb_short_for_code(keycodeUp)]);
    print_char(keyb_ch4short[keyb_short_for_code(keycodeDown)]);
    print_char(keyb_ch4short[keyb_short_for_code(keycodeLeft)]);
    print_char(keyb_ch4short[keyb_short_for_code(keycodeRight)]);
    print_char(keyb_ch4short[keyb_short_for_code(keycodeSelect)]);
    print_str("    joystick: ");
    print_char(joystick_ch4short[joystick_short_for_code(joycodeUp)]);
    print_char(joystick_ch4short[joystick_short_for_code(joycodeDown)]);
    print_char(joystick_ch4short[joystick_short_for_code(joycodeLeft)]);
    print_char(joystick_ch4short[joystick_short_for_code(joycodeRight)]);
    print_char(joystick_ch4short[joystick_short_for_code(joycodeSelect)]);
}

void smm_update()
{
    palette_fade_update();
}

#define HAT_LEN 32
#define HA_PHASE 11
#define HA_MASK 0x1F

static s8 head_anim_table[HAT_LEN] = {
    0, 0, 0, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1,
    0, 0, 0,-1,-1,-1,-2,-2,-2,-2,-2,-2,-2,-1,-1,-1, 
};

static u8 head_anim_frame = 0;

static void update_head_anim()
{
    s8 smarto_dy = head_anim_table[head_anim_frame];
    s8 dumber_dy = head_anim_table[(head_anim_frame + HA_PHASE) & HA_MASK];
    update_smarto(smarto_dy);
    update_dumber(dumber_dy);
    head_anim_frame++;
    head_anim_frame &= HA_MASK;
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
    if (palval < 0)
    {
        // -7 <= palval <= -1: fade in 
    }
    if (palval == 0)
    {
        // palval = 0: normal operation
        update_head_anim();

        if (actnDown && !prevDown)
            menu_option++;
        if (actnUp && !prevUp)
            menu_option--;
        menu_option &= 3;
        update_menu_cursor();

        // debug
        if (moot && zxkeySYM() && zxkeyC()) {
            // remove layer2 mapping
            z80_outp(0x123b,0x00);
            sc_switch_screen(sdk_enter, sdk_update, NULL);
            return;
        }

        if (actnSelect && !prevSelect)
            palette_fade_out_begin();
    }
    else if (palval >= 7)
    {
        // 1 <= palval <= 7:

        // remove layer2 mapping
        z80_outp(0x123b,0x00);

        switch(menu_option) {
            // case 0: sc_switch_screen(si_enter,  si_update,  NULL); break;
            case 0: sc_switch_screen(smg_enter, smg_update,  NULL); break;
            case 1: sc_switch_screen(si1_enter, si1_update, NULL); break;
            case 2: sc_switch_screen(sco_enter, sco_update, NULL); break;
            case 3: sc_switch_screen(sab_enter, sab_update, NULL); break;
        }
    }
}

