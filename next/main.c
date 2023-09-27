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

#include <arch/zxn.h>
#include <z80.h>
#include <input.h>
#include <stdlib.h>

#include "types.h"
#include "util_next.h"

#include "tilemap_manager.h"
#include "sprite_manager.h"

#include "anisprite.h"

#include "print_tile.h"

#include "screen_game.h"
#include "screen_inter.h"
#include "screen_menu_main.h"
#include "screen_info1.h"
#include "screen_info2.h"
#include "screen_info3.h"
#include "screen_about.h"
#include "screen_control.h"
#include "screen_menu_game.h"
#include "screen_level_select.h"
#include "screen_debug_keyboard.h"
#include "screen_final1.h"
#include "screen_final2.h"

#include "palette.h"

#include "control.h"
#include "keyb.h"
#include "joystick.h"

#include "palette.h"

#include "music_player.h"
#include "persist.h"

static bool mustSkipDueToSlowMotion();

void main(void)
{
    zx_border(INK_BLACK);

    setCpuSpeed28();
    disableContention();

    // Set ACB mode for AY
__asm
    EXTERN  readNextReg
    LD      A, $08
    CALL    readNextReg
    OR      A, %00100000
    NEXTREG $08, A
__endasm

    palette_tilemap_all_black();

    tilemap_setup();

    // sprite_load_game_defs();
    sprite_setup();

    sc_switch_screen(smm_enter, smm_update, NULL);

    // for quick testing
    // sc_switch_screen(sg_enter, sg_update, NULL);
    // sc_switch_screen(si_enter, si_update, NULL);
    // sc_switch_screen(smg_enter, smg_update, NULL);
    // sc_switch_screen(sab_enter, sab_update, NULL);
    // sc_switch_screen(si1_enter, si1_update, NULL);
    // sc_switch_screen(si2_enter, si2_update, NULL);
    // sc_switch_screen(si3_enter, si3_update, NULL);
    // sc_switch_screen(sls_enter, sls_update, NULL);
    // sc_switch_screen(sdk_enter, sdk_update, NULL);
    // sc_switch_screen(sco_enter, sco_update, NULL);
    // sc_switch_screen(sf1_enter, sf1_update, NULL);
    // sc_switch_screen(sf1_enter, sf1_update, NULL);
    // sc_switch_screen(sf2_enter, sf2_update, NULL);

    control_init();

    music_song_init(NULL);

    persist_check_moot();
    persist_load_levdata();
    persist_load_control();

    while(1)
    {
        waitForScanline(255);

        if (mustSkipDueToSlowMotion()) continue;

        music_song_play_frame();
        
        control_update();
        
        sc_update();
    }

    return 0;
}

static u8 debug_ctr = 0;

static bool mustSkipDueToSlowMotion()
{
    if (moot && zxkeyCAP()) {
        // slow motion on CAPS SHIFT pressed 
        if (debug_ctr > 0) {
            debug_ctr--;
            return true;
        }
        else
            debug_ctr = 25;
    }
    return false;
}

