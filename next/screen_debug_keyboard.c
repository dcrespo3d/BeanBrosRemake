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

#include "screen_debug_keyboard.h"
#include "types.h"

#include "print_tile.h"
#include "palette.h"
#include "keyb.h"
#include "joystick.h"
#include "control.h"

#include "screen_menu_main.h"
#include "globaldefs.h"

static void on_fade_update(s8 palval);

void sdk_enter_pg46()
{
    palette_fade_init(SCREEN_TYPE_GAME, on_fade_update);
}

void sdk_update_pg46()
{
    palette_fade_update();
}

static void on_fade_update(s8 palval)
{
    u8 x, y;
    if (palval == 0) {

        print_set_pos(0, 20); print_str("JOYSTICK:");
        joystick_codes_debug(2, 22);
        joystick_print_debug(2, 24);
        
        print_set_pos(0, 26); print_str("KEYBOARD:");
        keyb_codes_debug(2, 28);
        keyb_print_debug(0, 30);

        x = 4, y = 4;

        print_set_pos(x,y++); print_char(CHAR_UP);    print_hex_nibble(keyb_is_pressed(keycodeUp));
        print_set_pos(x,y++); print_char(CHAR_DOWN);  print_hex_nibble(keyb_is_pressed(keycodeDown));
        print_set_pos(x,y++); print_char(CHAR_LEFT);  print_hex_nibble(keyb_is_pressed(keycodeLeft));
        print_set_pos(x,y++); print_char(CHAR_RIGHT); print_hex_nibble(keyb_is_pressed(keycodeRight));
        print_set_pos(x,y++); print_char(CHAR_ENTER); print_hex_nibble(keyb_is_pressed(keycodeSelect));
        y++;
        print_set_pos(x,y++); print_char(CHAR_UP);    print_hex_nibble(joystick_is_pressed(joycodeUp));
        print_set_pos(x,y++); print_char(CHAR_DOWN);  print_hex_nibble(joystick_is_pressed(joycodeDown));
        print_set_pos(x,y++); print_char(CHAR_LEFT);  print_hex_nibble(joystick_is_pressed(joycodeLeft));
        print_set_pos(x,y++); print_char(CHAR_RIGHT); print_hex_nibble(joystick_is_pressed(joycodeRight));
        print_set_pos(x,y++); print_char(CHAR_BUT1);  print_hex_nibble(joystick_is_pressed(joycodeSelect));

        if (zxkeySYM() && zxkeyC()) {
            palette_fade_out_begin();
        }
    }
    else if (palval >= 7) {
        sc_switch_screen(smm_enter, smm_update, NULL);
    }
}

