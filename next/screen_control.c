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

#include "screen_control.h"
#include "types.h"

#include "print_tile.h"
#include "palette.h"
#include "keyb.h"
#include "joystick.h"
#include "sprite_manager.h"
#include "tilemap_manager.h"

#include "screen_menu_main.h"
#include "control.h"

#include "persist.h"

#define CUR_SLOT 0
#define CUR_PATTERN 94
#define SPAL_CUR 10

#define TEXT_SLOT 1
#define SPAL_TEXT 10

#define ST1X 40
#define ST1Y 32
#define ST1 "CONTROL"

#define ST2X 40
#define ST2Y 160
#define ST2 "REDEFINE KEYS"

#define ST3X 40
#define ST3Y 184
#define ST3 "REDEFINE JOYSTICK"

#define ST4X 40
#define ST4Y 208
#define ST4 "BACK TO MENU"

#define TT0X 4
#define TT0Y 8
#define TT0 "keyboard and joystick (kempston)\ncan be used simultaneously"
#define TT0A "   press S to skip this step   "
#define TT0B "                               "

#define TT1X 22
#define TT1Y 11
#define TT1 "KEY    JOY"

#define TT2X 4
#define TT2Y 13
#define TT2 "JUMP   / UP    :"

#define TT3X 4
#define TT3Y 14
#define TT3 "SWAP   / DOWN  :"

#define TT4X 4
#define TT4Y 15
#define TT4 "LEFT           :"

#define TT5X 4
#define TT5Y 16
#define TT5 "RIGHT          :"

#define TT6X 4
#define TT6Y 17
#define TT6 "SELECT / PAUSE :"

#define TT7X 5
#define TT7Y 19
#define TT7 "   Press ENTER to accept,\nor SPACE to repeat process."

#define KEYX 23
#define KEYY 13

#define JOYX 30
#define JOYY 13

#define MODE_MENU 0
#define MODE_DEFKEY 1
#define MODE_DEFJOY 2

static u8 mode;
static u8 step;

static void on_fade_update(s8 palval);

static SpriteDef sdef;
static SpriteDef curdef;

static void setup_menu_cursor();
static void update_menu_cursor();

#define MENUX 24
#define MENUY 160
#define MENUDY 24

static void setup_mode_menu();
static void setup_mode_defkey();
static void setup_mode_defjoy();

void sco_enter_pg46()
{
    palette_fade_init(SCREEN_TYPE_MENU, on_fade_update);

    mode = MODE_MENU;
    setup_mode_menu();
}

static void setup_mode_menu()
{
    // clear all
    tilemap_clear_buf();
    sprite_clear_all_slots();

    sdef.slot = TEXT_SLOT;
    sdef.pal = SPAL_TEXT;

    sdef.x = ST1X; sdef.y = ST1Y; sprite_text_print(&sdef, ST1, false);
    sdef.x = ST2X; sdef.y = ST2Y; sprite_text_print(&sdef, ST2, false);
    sdef.x = ST3X; sdef.y = ST3Y; sprite_text_print(&sdef, ST3, false);
    sdef.x = ST4X; sdef.y = ST4Y; sprite_text_print(&sdef, ST4, false);

    setup_menu_cursor();
    update_menu_cursor();

    print_set_pos(TT0X, TT0Y); print_str(TT0);
    print_set_pos(TT1X, TT1Y); print_str(TT1);
    print_set_pos(TT2X, TT2Y); print_str(TT2);
    print_set_pos(TT3X, TT3Y); print_str(TT3);
    print_set_pos(TT4X, TT4Y); print_str(TT4);
    print_set_pos(TT5X, TT5Y); print_str(TT5);
    print_set_pos(TT6X, TT6Y); print_str(TT6);

    print_set_pos(KEYX, KEYY+0);
    print_char(keyb_ch4short[keyb_short_for_code(keycodeUp)]);
    print_set_pos(KEYX, KEYY+1);
    print_char(keyb_ch4short[keyb_short_for_code(keycodeDown)]);
    print_set_pos(KEYX, KEYY+2);
    print_char(keyb_ch4short[keyb_short_for_code(keycodeLeft)]);
    print_set_pos(KEYX, KEYY+3);
    print_char(keyb_ch4short[keyb_short_for_code(keycodeRight)]);
    print_set_pos(KEYX, KEYY+4);
    print_char(keyb_ch4short[keyb_short_for_code(keycodeSelect)]);

    print_set_pos(JOYX, JOYY+0);
    print_char(joystick_ch4short[joystick_short_for_code(joycodeUp)]);
    print_set_pos(JOYX, JOYY+1);
    print_char(joystick_ch4short[joystick_short_for_code(joycodeDown)]);
    print_set_pos(JOYX, JOYY+2);
    print_char(joystick_ch4short[joystick_short_for_code(joycodeLeft)]);
    print_set_pos(JOYX, JOYY+3);
    print_char(joystick_ch4short[joystick_short_for_code(joycodeRight)]);
    print_set_pos(JOYX, JOYY+4);
    print_char(joystick_ch4short[joystick_short_for_code(joycodeSelect)]);
}

static void setup_mode_defkey()
{
    // clear all
    tilemap_clear_buf();
    sprite_clear_all_slots();

    sdef.slot = TEXT_SLOT;
    sdef.pal = SPAL_TEXT;

    sdef.x = ST1X; sdef.y = ST1Y; sprite_text_print(&sdef, ST2, false);
}

static void setup_mode_defjoy()
{
    // clear all
    tilemap_clear_buf();
    sprite_clear_all_slots();

    sdef.slot = TEXT_SLOT;
    sdef.pal = SPAL_TEXT;

    sdef.x = ST1X; sdef.y = ST1Y; sprite_text_print(&sdef, ST3, false);
}

void sco_update_pg46()
{
    palette_fade_update();
}

static s8 optY = 2;

static void setup_menu_cursor()
{
    curdef.slot = CUR_SLOT;
    curdef.pal = SPAL_CUR;
    curdef.pat = CUR_PATTERN;
    curdef.mirrot = ATTR_____;
    optY = 2;
}

static void update_menu_cursor()
{
    curdef.x = MENUX;
    curdef.y = MENUY + MENUDY * optY;
    sprite_update(&curdef);
}

static void on_mode_menu()
{
    if (actnUp && !prevUp) {
        optY--;
        if (optY < 0) optY = 2;
    }
    if (actnDown && !prevDown) {
        optY++;
        if (optY > 2) optY = 0;
    }
    update_menu_cursor();

    if (actnSelect && !prevSelect) {
    if (optY == 0) {
        mode = MODE_DEFKEY;
        step = 0;
        setup_mode_defkey();
    }
    else if (optY == 1) {
        mode = MODE_DEFJOY;
        step = 0;
        setup_mode_defjoy();
    }
    else if (optY == 2)
        palette_fade_out_begin();
    }
}

static bool key_chosen;

static bool key_already_used(u16 keycode)
{
    if (keycode == keycodeUp) return true;
    if (keycode == keycodeDown) return true;
    if (keycode == keycodeLeft) return true;
    if (keycode == keycodeRight) return true;
    if (keycode == keycodeSelect) return true;

    return false;
}

static bool process_defkey(u16* pkeycode)
{
    if (!key_chosen) {
        if (1 == keyb_count()) {
            u16 new_keycode = keyb_code();
            if (!key_already_used(new_keycode)) {
                *pkeycode = new_keycode;
                print_set_pos(KEYX, KEYY-1+step);
                print_char(keyb_ch4short[keyb_short_for_code(new_keycode)]);
                key_chosen = true;
            }
        }
    }
    else {
        if (!keyb_any()) {
            step++;
            key_chosen = false;
            return true;
        }
    }
    return false;
}

static void on_mode_defkey()
{
    if (step == 0) {
        if (!keyb_any()) {
            keycodeUp = keycodeDown = keycodeLeft = keycodeRight = keycodeSelect = 0xFFFF;
            print_set_pos(TT2X, TT2Y); print_str(TT2);
            key_chosen = false;
            step++;
        }
        return;
    }
    if (step == 1) {
        if (process_defkey(&keycodeUp)) {
            print_set_pos(TT3X, TT3Y); print_str(TT3);
        }
        return;
    }
    if (step == 2) {
        if (process_defkey(&keycodeDown)) {
            print_set_pos(TT4X, TT4Y); print_str(TT4);
        }
        return;
    }
    if (step == 3) {
        if (process_defkey(&keycodeLeft)) {
            print_set_pos(TT5X, TT5Y); print_str(TT5);
        }
        return;
    }
    if (step == 4) {
        if (process_defkey(&keycodeRight)) {
            print_set_pos(TT6X, TT6Y); print_str(TT6);
        }
        return;
    }
    if (step == 5) {
        if (process_defkey(&keycodeSelect)) {
        }
        return;
    }
    if (step == 6) {
        print_set_pos(TT7X, TT7Y); print_str(TT7);
        if (!keyb_any()) {
            step++;
        }
        return;
    }
    if (step == 7) {
        if (zxkeyENT()) {
            persist_save_control();
            mode = MODE_MENU;
            setup_mode_menu();
        }
        else if (zxkeySPC()) {
            mode = MODE_DEFKEY;
            step = 0;
            setup_mode_defkey();
        }
        return;
    }
}

static bool joy_chosen;

static bool joy_already_used(u8 joycode)
{
    if (joycode == joycodeUp) return true;
    if (joycode == joycodeDown) return true;
    if (joycode == joycodeLeft) return true;
    if (joycode == joycodeRight) return true;
    if (joycode == joycodeSelect) return true;

    return false;
}

static bool process_defjoy(u8* pjoycode)
{
    if (!joy_chosen) {
        if (1 == joystick_count()) {
            u8 new_joycode = joystick_code();
            if (!joy_already_used(new_joycode)) {
                *pjoycode = new_joycode;
                print_set_pos(KEYX, KEYY-1+step);
                print_char(joystick_ch4short[joystick_short_for_code(new_joycode)]);
                joy_chosen = true;
            }
        }
    }
    else {
        if (!joystick_any()) {
            step++;
            joy_chosen = false;
            return true;
        }
    }
    return false;
}

static u8 prev_joycodeUp;
static u8 prev_joycodeDown;
static u8 prev_joycodeLeft;
static u8 prev_joycodeRight;
static u8 prev_joycodeSelect;

static void on_mode_defjoy()
{
    if (step == 0) {
        if (!joystick_any()) {
            print_set_pos(TT0X, TT0Y+1); print_str(TT0A);
            prev_joycodeUp = joycodeUp;
            prev_joycodeDown = joycodeDown;
            prev_joycodeLeft = joycodeLeft;
            prev_joycodeRight = joycodeRight;
            prev_joycodeSelect = joycodeSelect;
            joycodeUp = joycodeDown = joycodeLeft = joycodeRight = joycodeSelect = 0xFF;
            print_set_pos(TT2X, TT2Y); print_str(TT2);
            joy_chosen = false;
            step++;
        }
        return;
    }
    if (step == 1) {
        if (zxkeyS()) {
            joycodeUp = prev_joycodeUp;
            joycodeDown = prev_joycodeDown;
            joycodeLeft = prev_joycodeLeft;
            joycodeRight = prev_joycodeRight;
            joycodeSelect = prev_joycodeSelect;
            mode = MODE_MENU;
            setup_mode_menu();
        }
        if (process_defjoy(&joycodeUp)) {
            print_set_pos(TT0X, TT0Y+1); print_str(TT0B);
            print_set_pos(TT3X, TT3Y); print_str(TT3);
        }
        return;
    }
    if (step == 2) {
        if (process_defjoy(&joycodeDown)) {
            print_set_pos(TT4X, TT4Y); print_str(TT4);
        }
        return;
    }
    if (step == 3) {
        if (process_defjoy(&joycodeLeft)) {
            print_set_pos(TT5X, TT5Y); print_str(TT5);
        }
        return;
    }
    if (step == 4) {
        if (process_defjoy(&joycodeRight)) {
            print_set_pos(TT6X, TT6Y); print_str(TT6);
        }
        return;
    }
    if (step == 5) {
        if (process_defjoy(&joycodeSelect)) {
        }
        return;
    }
    if (step == 6) {
        print_set_pos(TT7X, TT7Y); print_str(TT7);
        if (!joystick_any()) {
            step++;
        }
        return;
    }
    if (step == 7) {
        if (zxkeyENT()) {
            persist_save_control();
            mode = MODE_MENU;
            setup_mode_menu();
        }
        else if (zxkeySPC()) {
            joycodeUp = prev_joycodeUp;
            joycodeDown = prev_joycodeDown;
            joycodeLeft = prev_joycodeLeft;
            joycodeRight = prev_joycodeRight;
            joycodeSelect = prev_joycodeSelect;
            mode = MODE_DEFJOY;
            step = 0;
            setup_mode_defjoy();
        }
        return;
    }
}

static void on_fade_update(s8 palval)
{
    if (palval == 0) {
        switch(mode) {
            case MODE_MENU:   on_mode_menu();   break;
            case MODE_DEFKEY: on_mode_defkey(); break;
            case MODE_DEFJOY: on_mode_defjoy(); break;
        }
    }
    else if (palval >= 7) {
        sc_switch_screen(smm_enter, smm_update, NULL);
    }
}



