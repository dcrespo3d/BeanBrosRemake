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

#include <string.h>

#include "screen_final1.h"
#include "screen_final2.h"
#include "types.h"

#include "print_tile.h"
#include "palette.h"
#include "sprite_manager.h"

#include "keyb.h"
#include "joystick.h"

#include "screen_menu_main.h"
#include "screen_game.h"
#include "globaldefs.h"

#define SMARTO_X 80
#define SMARTO_Y 96
#define DUMBER_X 176
#define DUMBER_Y 96

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

#define ST1A "GAME COMPLETE!"
#define ST1B "GAME (EXT) COMPLETE!"
#define ST1X 24
#define ST1Y 48

#define ST2A "YOU USED "
#define ST2B " LIVES"
#define ST2X 52
#define ST2Y 192

void sf1_enter_pg46()
{
    palette_fade_init(SCREEN_TYPE_MENU, on_fade_update);

    setup_smarto();
    setup_dumber();

    sdef.slot = TEXT_SLOT;
    sdef.pal = SPAL_TEXT;
    sdef.mirrot = ATTR_____;
    sdef.x = ST1X;
    sdef.y = ST1Y;
    if (!sg_is_ext_level) 
        sprite_text_print(&sdef, ST1A, true);
    else
        sprite_text_print(&sdef, ST1B, true);

    u16 lives = sg_lives_lost;
    char msg[32] = ST2A;
    string_append_dec_word(msg, lives);
    strcat(msg, ST2B);

    sdef.x = ST2X;
    sdef.y = ST2Y;
    sprite_text_print(&sdef, msg, true);

    print_set_pos(0, 29);
    println_ctr(MSG_PRESS_ANY_KEY, 40);
}

void sf1_update_pg46()
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

static bool prev_any, curr_any;

static void on_fade_update(s8 palval)
{
    update_head_anim();
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
        sc_switch_screen(sf2_enter, sf2_update, NULL);
    }
}

