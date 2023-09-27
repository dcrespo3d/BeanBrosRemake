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

#include "screen_info1.h"
#include "screen_info2.h"
#include "types.h"

#include "print_tile.h"
#include "palette.h"
#include "keyb.h"
#include "joystick.h"
#include "sprite_manager.h"

#include "screen_menu_main.h"
#include "globaldefs.h"

static void on_fade_update(s8 palval);

#define SMARTO_X 240
#define SMARTO_Y 32
#define DUMBER_X 240
#define DUMBER_Y 96

#define SPAL_HEAD_SMARTO 0
#define SPAL_HEAD_DUMBER 1
#define SPAL_FEET 7
#define SPAL_TEXT 8
#define SMARTO_FEET_SLOT 0
#define SMARTO_HEAD_SLOT 2
#define SMARTO_HEAD_PATTERN 11
#define SMARTO_FEET_PATTERN 57
#define DUMBER_FEET_SLOT 6
#define DUMBER_HEAD_SLOT 8
#define DUMBER_HEAD_PATTERN 15
#define DUMBER_FEET_PATTERN 59

#define TEXT_SLOT1 40
#define TEXT_SLOT2 60
#define TEXT_SLOT3 80
#define TEXT_SLOT4 90

#define TEXT_X1 52
#define TEXT_Y1 40
#define TEXT_STR1 "THIS IS FABE"

#define TEXT_X2 52
#define TEXT_Y2 104
#define TEXT_STR2 "THIS IS LUBI"

#define TEXT_X3 52 + 60
#define TEXT_Y3 160
#define TEXT_STR3 "THEY ARE"

#define TEXT_X4 52
#define TEXT_Y4 180
#define TEXT_STR4 "THE BEAN BROTHERS!"

static SpriteDef sdef;

static void update_smarto(s8 dy);

static void setup_smarto()
{
    sdef.slot = SMARTO_FEET_SLOT;
    sdef.pal = SPAL_FEET;
    sdef.pat = SMARTO_FEET_PATTERN;
    sdef.mirrot = ATTR_X___;
    sdef.x = SMARTO_X + 16;
    sdef.y = SMARTO_Y + 16;
    for (u8 x = 0; x < 2; x++) {
        sprite_update(&sdef);
        sdef.slot++;
        sdef.pat++;
        sdef.x -= 16;
    }

    update_smarto(0);
}

static void update_smarto(s8 dy)
{
    sdef.slot = SMARTO_HEAD_SLOT;
    sdef.pal = SPAL_HEAD_SMARTO;
    sdef.pat = SMARTO_HEAD_PATTERN;
    sdef.mirrot = ATTR_X___;
    sdef.x = SMARTO_X + 16;
    sdef.y = SMARTO_Y + dy;
    for (u8 x = 0; x < 4; x++) {
        if (x == 2) {
            sdef.x += 32;
            sdef.y += 16;
        }
        sprite_update(&sdef);
        sdef.slot++;
        sdef.pat++;
        sdef.x -= 16;

    }
}

static void update_dumber(s8 dy);

static void setup_dumber()
{
    sdef.slot = DUMBER_FEET_SLOT;
    sdef.pal = SPAL_FEET;
    sdef.pat = DUMBER_FEET_PATTERN;
    sdef.mirrot = ATTR_____;
    sdef.x = DUMBER_X;
    sdef.y = DUMBER_Y + 16;
    for (u8 x = 0; x < 2; x++) {
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
    sdef.pal = SPAL_HEAD_DUMBER;
    sdef.pat = DUMBER_HEAD_PATTERN;
    sdef.mirrot = ATTR_____;
    sdef.x = DUMBER_X;
    sdef.y = DUMBER_Y + dy;
    for (u8 x = 0; x < 4; x++) {
        if (x == 2) {
            sdef.x -= 32;
            sdef.y += 16;
        }
        sprite_update(&sdef);
        sdef.slot++;
        sdef.pat++;
        sdef.x += 16;
    }
}

void si1_enter_pg46()
{
    palette_fade_init(SCREEN_TYPE_GAME, on_fade_update);

    sdef.slot = TEXT_SLOT1;
    sdef.pal = SPAL_TEXT;
    sdef.mirrot = ATTR_____;
    sdef.x = TEXT_X1;
    sdef.y = TEXT_Y1;
    sprite_text_print(&sdef, TEXT_STR1, false);

    setup_smarto();

    sdef.slot = TEXT_SLOT2;
    sdef.pal = SPAL_TEXT;
    sdef.mirrot = ATTR_____;
    sdef.x = TEXT_X2;
    sdef.y = TEXT_Y2;
    sprite_text_print(&sdef, TEXT_STR2, false);

    setup_dumber();

    sdef.slot = TEXT_SLOT3;
    sdef.pal = SPAL_TEXT;
    sdef.mirrot = ATTR_____;
    sdef.x = TEXT_X3;
    sdef.y = TEXT_Y3;
    sprite_text_print(&sdef, TEXT_STR3, false);

    sdef.slot = TEXT_SLOT4;
    sdef.pal = SPAL_TEXT;
    sdef.mirrot = ATTR_____;
    sdef.x = TEXT_X4;
    sdef.y = TEXT_Y4;
    sprite_text_print(&sdef, TEXT_STR4, false);

    //         1234567890123456789012345
    print_set_pos(0, 29);
    println_ctr(MSG_PRESS_ANY_KEY, 40);
}

void si1_update_pg46()
{
    palette_fade_update();
}

static void update_head_anim();

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
        sc_switch_screen(si2_enter, si2_update, NULL);
    }
}

#define HAT_LEN 32
#define HA_PHASE 11
#define HA_MASK 0x1F

static s8 head_anim_table[HAT_LEN] = {
    0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
};

static u8 head_anim_frame = 0;

static void update_head_anim()
{
    s8 smarto_dy = head_anim_table[head_anim_frame];
    s8 dumber_dy = head_anim_table[(head_anim_frame + HA_PHASE) & HA_MASK];
    // if (frame_num >  40) update_smarto(smarto_dy);
    // if (frame_num > 140) update_dumber(dumber_dy);
    update_smarto(smarto_dy);
    update_dumber(dumber_dy);
    head_anim_frame++;
    head_anim_frame &= HA_MASK;
}



