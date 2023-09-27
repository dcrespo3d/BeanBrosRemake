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

#include "screen_final2.h"
#include "types.h"

#include "print_tile.h"
#include "palette.h"
#include "keyb.h"
#include "joystick.h"

#include "screen_menu_main.h"
#include "globaldefs.h"
#include "sprite_manager.h"
#include <stdlib.h>

static void on_fade_update(s8 palval);

#define Y0BG 10
#define DYBG (40*8)

#define Y0CN 15

#define X0DR 25
#define Y0DR 13

static u8 framecnt = 0;

#define SMARTO_X 232
#define SMARTO_Y 128
#define DUMBER_X 256
#define DUMBER_Y 128

#define SMARTO_SPAL 2
#define SMARTO_SLOT 0
#define SMARTO_PATTERN 40
#define DUMBER_SPAL 3
#define DUMBER_SLOT 1
#define DUMBER_PATTERN 44
#define SPAL_TEXT 10
#define TEXT_SLOT 20

// #define CUR_SLOT 40
// #define CUR_PATTERN 94
// #define SPAL_CUR 10

#define ST1 "WELL DONE!"
#define ST1Y 40

#define TTX 4
#define TTY 22
#define TT1 "You have rescued Fabe and Lubi."
#define TT2 "They are free to resume their"
#define TT3 "work at the bean factory..."

static SpriteDef sdef;

static void update_bros();

#define BEAN_PATTERN 48
#define BEAN_SPAL 3;
#define OBJ_SLOT 2
#define OBJA_PATTERN 49
#define OBJA_SPAL 4
#define OBJB_PATTERN 54
#define OBJB_SPAL 5
#define ODEF_Y 104
#define ODEF_DX 48

#define MASK_SLOT 18
#define MASK_PATTERN 61
#define MASK_SPAL 8
#define MASK_X 216
#define MASK_Y 104

#define CAM_SLOT 19
#define CAM_PATTERN 60
#define CAM_SPAL 7
#define CAM_X 200
#define CAM_Y  88

#define ODEF_COUNT 5
static SpriteDef odefs[ODEF_COUNT];

static u8 next_obj;
static u8 next_pal;

static bool is3;
static u8 beancnt;

static u8 objidx;

static bool is_bean;
static bool scanning;
static bool the_any;

static void choose_next_object()
{
    if (beancnt) {
        beancnt--;
        next_obj = BEAN_PATTERN;
        next_pal = BEAN_SPAL;
        return;
    }

    if (is3) {
        is3 = false;
        beancnt = 3;
    }
    else {
        is3 = true;
        beancnt = 4;
    }

    if (objidx < 5) {
        next_pal = OBJA_SPAL;
        next_obj = OBJA_PATTERN + objidx;
    }
    else {
        next_pal = OBJB_SPAL;
        next_obj = OBJB_PATTERN - 5 + objidx;
    }
    objidx++;
    if (objidx >= 11) objidx = 0;
}

static void setup_objects()
{
    for (u8 i = 0; i < ODEF_COUNT; i++) {
        SpriteDef* odef = &odefs[i];
        odef->slot = OBJ_SLOT+i;
        odef->pat = BEAN_PATTERN;
        odef->pal = BEAN_SPAL;
        odef->mirrot = ATTR_____;
        odef->x = -16 -ODEF_DX*(u16)i;
        odef->y = ODEF_Y;
        odef->scale = 0;
        // sprite_update(odef);
    }
}

static void update_objects()
{
    if (framecnt & 1) return;

    for (u8 i = 0; i < ODEF_COUNT; i++) {
        SpriteDef* odef = &odefs[i];
        if (odef->x == 160) {
            scanning = true;
            if (odef->pat == BEAN_PATTERN)
                is_bean = true;
            else
                is_bean = false;
        }
        if (odef->x == 216)
            odef->x = -24;
        if (odef->x == -16) {
            choose_next_object();
            odef->pat = next_obj;
            odef->pal = next_pal;
        }
        if (odef->x < -16) {
            s16 aux = odef->x;
            odef->x = -16;
            sprite_update(odef);
            odef->x = aux;
        }
        else {
            sprite_update(odef);
        }
        odef->x++;
    }
}

void sf2_enter_pg46()
{
    palette_fade_init(SCREEN_TYPE_MENU, on_fade_update);

    is3 = false;
    beancnt = 4;

    objidx = 0;

    is_bean = true;
    scanning = false;

    the_any = false;

    print_set_pos(0, Y0BG);
    print_set_attr(0);
    print_set_symbol(10);
    u16 cnt = DYBG;
    while (cnt--) {
        print_symbol();
    }
    cnt = 40;
    print_set_attr(2<<4); // palette 2
    print_set_symbol(20);
    while (cnt--) {
        print_symbol();
    }

    print_set_pos(0, Y0CN);
    cnt = 6;
    print_set_attr(3<<4); // palette 0
    while (cnt--) {
        print_char(14-128);
        print_char(14-128);
        print_char(13-128);
        print_char(14-128);
    }
    print_char(14-128);
    print_char(14-128);
    print_char(15-128);

    print_set_pos(0, Y0CN+1);
    cnt = 13;
    print_set_attr(4<<4); // palette 4
    while (cnt--) {
        print_char(26-128);
        print_char(27-128);
    }
    print_char(76-128);

    print_set_pos(0, Y0CN+2);
    cnt = 13;
    print_set_attr(4<<4); // palette 4
    while (cnt--) {
        print_char(42-128);
        print_char(43-128);
    }
    print_char(92-128);

    print_set_attr(0<<4); // palette 0
    print_set_symbol(9);
    print_set_pos(X0DR, Y0DR);
    print_symbol();
    print_symbol();
    print_set_pos(X0DR, Y0DR+1);
    print_symbol();
    print_symbol();

    framecnt = 0;

    print_set_pos(0, 29);
    println_ctr(MSG_PRESS_ANY_KEY, 40);

    sdef.slot = TEXT_SLOT;
    sdef.pal = SPAL_TEXT;
    sdef.mirrot = ATTR_____;
    sdef.x = 0;
    sdef.y = ST1Y;
    sprite_text_print(&sdef, ST1, true);

    sdef.slot = MASK_SLOT;
    sdef.pat = MASK_PATTERN;
    sdef.pal = MASK_SPAL;
    sdef.mirrot = ATTR_____;
    sdef.x = MASK_X;
    sdef.y = MASK_Y;
    sprite_update(&sdef);

    sdef.slot = CAM_SLOT;
    sdef.pat = CAM_PATTERN;
    sdef.pal = CAM_SPAL;
    sdef.mirrot = ATTR_____;
    sdef.x = CAM_X;
    sdef.y = CAM_Y;
    sprite_update(&sdef);

    update_bros();

    setup_objects();

    print_set_attr(TPAL_BLUE<<4); // palette for blue

    print_set_pos(TTX, TTY);
    println(TT1);
    println(TT2);
    println(TT3);

    print_set_attr(0);

}

static void update_conveyor()
{
    u8 conv_frame = ((framecnt >> 1) & 3) << 4;
    u8* ptr = 0x4000 + 80 * Y0CN;
    u8 cnt = 27;
    while(cnt--) {
        u8 tile = *ptr;
        tile &= 0xCF;
        tile |= conv_frame;
        *ptr = tile;
        ptr += 2;
    }
    // print_set_pos(0, 30);
    // print_hex_byte(conv_frame);
}

static void update_bros()
{
    u8 smarto_frame = ((framecnt >> 3) & 3);
    sdef.slot = SMARTO_SLOT;
    sdef.pal = SMARTO_SPAL;
    sdef.pat = SMARTO_PATTERN | smarto_frame;
    sdef.mirrot = ATTR_____;
    sdef.x = SMARTO_X;
    sdef.y = SMARTO_Y;
    sprite_update(&sdef);

    u8 dumber_frame = (((framecnt + 12) >> 3) & 3);
    sdef.slot = DUMBER_SLOT;
    sdef.pal = DUMBER_SPAL;
    sdef.pat = DUMBER_PATTERN | dumber_frame;
    sdef.mirrot = ATTR_X___;
    sdef.x = DUMBER_X;
    sdef.y = DUMBER_Y;
    sprite_update(&sdef);
}

#define SETDEF  print_set_attr(TPAL_DEF << 4)
#define SETGREEN print_set_attr(TPAL_GREEN << 4)
#define SETRED  print_set_attr(TPAL_RED << 4)
static void update_scanner()
{
    #define SCANX 28
    #define SCANY 10
    if (!scanning) {
        SETDEF;
        print_set_symbol(132);
        print_frame(SCANX, SCANY, 12, 3);
        print_set_pos(SCANX+1,SCANY+1);
        print_str(" SCANNING ");
    }
    else if (is_bean) {
        SETGREEN;
        print_set_symbol(132);
        print_frame(SCANX, SCANY, 12, 3);
        print_set_pos(SCANX+1,SCANY+1);
        print_str("   BEAN   ");
    }
    else {
        SETRED;
        print_set_symbol(132);
        print_frame(SCANX, SCANY, 12, 3);
        print_set_pos(SCANX+1,SCANY+1);
        print_str(" NOT BEAN ");
    }
    SETDEF;
}

static void update_anymsg()
{
    SETDEF;
    print_set_pos(0, 29);
    if (!the_any)
        println_ctr(MSG_PRESS_ANY_KEY, 40);
    else
        println_ctr(MSG_PRESS_THE_ANY_KEY, 40);
}

void sf2_update_pg46()
{
    palette_fade_update();
    update_conveyor();
    update_bros();
    update_objects();
    update_scanner();
    update_anymsg();
    framecnt++;
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
            if (!the_any)
                the_any = true;
            else
                palette_fade_out_begin();
        }
        prev_any = curr_any;
    }
    else if (palval >= 7) {
        sc_switch_screen(smm_enter, smm_update, NULL);
    }
}

