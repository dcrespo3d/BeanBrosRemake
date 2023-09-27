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

#include "sprite_manager.h"
#include "globaldefs.h"
#include "print_tile.h"
#include "control.h"
#include "screen_game.h"

#define PD_BGPAT  63
#define PD_X1   96
#define PD_Y1   88
#define PD_Y2   104

#define PD_TXT0 "CONTINUE"
#define PD_TX0  112
#define PD_TY0  96

#define PD_TXT1 "RESTART"
#define PD_TX1  112
#define PD_TY1  120

#define PD_TXT2 "EXIT"
#define PD_TX2  112
#define PD_TY2  144

#define PD_CURX 100
#define PD_CURDY 24
#define PD_CURPAT 94


bool pause_dialog_paused;
bool pause_dialog_visible;

static SpriteDef pdef;

static u8 option;

// frame counter for inhibiting too frequent pauses
// u8 inh_frame = 0;
// #define INH_FRAME_COUNT 100

static void update_cursor();

static void pause_dialog_show()
{
    pdef.slot = SS_PAUSE_DLG;
    pdef.pal = SPAL_SPFONT;
    pdef.mirrot = ATTR_____;
    pdef.pat = PD_BGPAT;
    pdef.scale = SCALE_X8 | SCALE_Y4;

    pdef.x = PD_X1;
    pdef.y = PD_Y1;
    sprite_update(&pdef);
    pdef.slot++;

    pdef.x = PD_X1;
    pdef.y = PD_Y2;
    sprite_update(&pdef);
    pdef.slot++;

    pdef.scale = SCALE_X1 | SCALE_Y1;

    pdef.x = PD_TX0;
    pdef.y = PD_TY0;
    sprite_text_print(&pdef, PD_TXT0, false);

    pdef.x = PD_TX1;
    pdef.y = PD_TY1;
    sprite_text_print(&pdef, PD_TXT1, false);

    pdef.x = PD_TX2;
    pdef.y = PD_TY2;
    sprite_text_print(&pdef, PD_TXT2, false);
}

static void pause_dialog_hide()
{
    sprite_clear_all_from(SS_PAUSE_DLG);
}

void pause_dialog_init()
{
    pause_dialog_paused = false;
    pause_dialog_visible = false;
    // inh_frame = INH_FRAME_COUNT;
}

void pause_dialog_update()
{
    if (!pause_dialog_paused) {
        if (actnSelect && !prevSelect) //  && inh_frame > INH_FRAME_COUNT)
            pause_dialog_paused = true;
        // inh_frame++;
    }
    else {
        if (!pause_dialog_visible) {
            // NOT visible
            if (!actnSelect && prevSelect) {
                pause_dialog_visible = true;
                pause_dialog_show();
                option = 0;
                update_cursor();
            }
        }
        else {
            // VISIBLE
            if (actnSelect && !prevSelect) {
                pause_dialog_paused = false;
                pause_dialog_visible = false;
                pause_dialog_hide();
                if (option == 1)
                    sg_restart();
                else if (option == 2)
                    sg_exit();
                // inh_frame = 0;
            }
            else if (actnUp && !prevUp) {
                if (option > 0) option--;
                update_cursor();
            }
            else if (actnDown && !prevDown) {
                option++;
                if (option > 2) option = 0;
                update_cursor();
            }
        }
    }
}

static void update_cursor()
{
    pdef.slot = SS_PAUSE_CUR;
    pdef.pal = SPAL_SPFONT;
    pdef.mirrot = ATTR_____;
    pdef.pat = PD_CURPAT;
    pdef.scale = SCALE_X1 | SCALE_Y1;

    pdef.x = PD_CURX;
    pdef.y = PD_TY0 + option * PD_CURDY;
    sprite_update(&pdef);
}