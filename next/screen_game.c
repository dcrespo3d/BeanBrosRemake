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

#include "screen_game.h"
#include "screen_inter.h"
#include "screen_menu_main.h"
#include "screen_level_select.h"
#include "screen_final1.h"

#include "types.h"
#include "anisprite.h"
#include "print_tile.h"
#include "player.h"
#include "object.h"
#include "palette.h"

#include "keyb.h"

#include "levels.h"

#include "music_player.h"

#include "pause_dialog.h"
#include "persist.h"

extern u8 collmap[];

static void dump_collmap();
static void check_collisions_player_enemy();
static void check_collisions_player_lever();
static void check_exit_level();
static void exit_level();
static void on_state_play();
static void on_state_fadeout();
static void on_state_fadein();

static bool check_level_jump();

static u8* levmap;
static LevelDef* levdef;

// production: set to ZERO (0)
u8 sg_level_index = 0;

u16 sg_lives_lost = 0;

u8 sg_ful_ori = 0;
u8 sg_ful_ext = 20;
// u8 sg_ful_ori = 19;
// u8 sg_ful_ext = 39;
bool sg_is_ext_level = false;
bool sg_is_select_level = false;


static bool level_exited = false;
static bool player_killed = false;
static bool exit_to_menu = false;

static void on_fade_update(s8 palval);

void sg_enter()
{
    // unlock if needed
    if (!sg_is_select_level) {
        if (!sg_is_ext_level) {
            if (sg_ful_ori < sg_level_index) {
                sg_ful_ori = sg_level_index;
                music_song_init(NULL);
                persist_save_levdata();
            }
        }
        else {
            if (sg_ful_ext < sg_level_index) {
                sg_ful_ext = sg_level_index;
                music_song_init(NULL);
                persist_save_levdata();
            }
        }
    }

    // setup palette fade scheme
    palette_fade_init(SCREEN_TYPE_GAME, on_fade_update);

    // setup music
    u8 songidx = sg_level_index & 3;
    if      (0 == songidx) music_song_init(song_meteo);
    else if (1 == songidx) music_song_init(song_lonely);
    else if (2 == songidx) music_song_init(song_revolution);
    else                   music_song_init(song_leaving);

    AVOID_MUSIC_STUTTER

    // setup page for levels
    if (sg_level_index < 20) {
        ZXN_WRITE_REG(0x50, 34);
        ZXN_WRITE_REG(0x51, 35);
    }
    else {
        ZXN_WRITE_REG(0x50, 44);
        ZXN_WRITE_REG(0x51, 45);
    }

    level_exited = false;
    player_killed = false;
    levmap = levmap_array[sg_level_index];
    levdef = levdef_array[sg_level_index];

    // for (u16 i = 0; i < 32; i++)
    //     collmap[i] = i&15;
    // dump_collmap();

    AVOID_MUSIC_STUTTER

    sg_decompress_tilemap(levmap);

    // print_set_pos(4,31);
    // print_str("Bean Brothers [DAPR] DEV VERSION");

    AVOID_MUSIC_STUTTER

    players_setup(levdef);

    AVOID_MUSIC_STUTTER

    objects_setup(levdef);

    AVOID_MUSIC_STUTTER

    pause_dialog_init();
    exit_to_menu = false;
    
    // DEBUG
    // dump_collmap();
}

extern u8 tileset_palgroup[];
extern u8 tileset_colltable[];
extern u8 collmap[];

#define LEVMAP_CODEC_V2

#ifdef LEVMAP_CODEC_V1
void sg_decompress_tilemap(u8* levelmap)
{
    u8* src = levelmap;
    u8* dst = 0x4000;
    u8* coll = collmap;
    u16 idx = 0;

    while (idx < 2560) {
        u8 cnt = *src++;
        u8 dat = *src++;
        while (cnt--) {
            dst[idx++] = dat;
            dst[idx++] = tileset_palgroup[dat];
            *coll++ = tileset_colltable[dat];
        }
    }
}
#endif

#ifdef LEVMAP_CODEC_V2
void sg_decompress_tilemap(u8* levelmap)
{
    u8* src = levelmap;
    u8* dst = 0x4000;
    u8* coll = collmap;
    u16 idx = 0;

    while (idx < 2560) {
        u8 cnt = 1;
        u8 dat = *src++;
        if (dat & 128) {
            cnt = dat & 127;
            dat = *src++;
        }
        while (cnt--) {
            if (((u8)idx) == 0)
                AVOID_MUSIC_STUTTER
            dst[idx++] = dat;
            dst[idx++] = tileset_palgroup[dat];
            *coll++ = tileset_colltable[dat];
        }
    }
}
#endif

void displayLineTiming()
{
    u16 avl = getActiveVideoLineWord();
    print_set_pos(0,31);
    print_str("   ");
    print_set_pos(0,31);
    print_dec_word(avl);
}

void sg_update()
{
    if (check_level_jump()) return;

    if (moot && !level_exited && zxkeySYM() && zxkeyE()) exit_level();

    if (!pause_dialog_paused) {
        palette_fade_update();
        // sprite_debug_visible();
        if (moot && zxkeyC()) dump_collmap();
    }
    else
        pause_dialog_update();

    if (moot) displayLineTiming();
}

static void on_fade_update(s8 palval)
{
    anisprite_tick();

    players_update();
    objects_update();

    check_collisions_player_enemy();
    check_collisions_player_lever();

    if (palval == 0) {
        pause_dialog_update();
        check_exit_level();
    }
    else if (palval >= 7) {
        if (exit_to_menu) {
            if (!sg_is_select_level)
                sc_switch_screen(smm_enter, smm_update, NULL);
            else
                sc_switch_screen(sls_enter, sls_update, NULL);
        }
        else if (!level_exited)
            sg_enter();
        else if (!sg_is_select_level) {
            sg_level_index++;
            if (sg_level_index == 20 || sg_level_index == 40) {
                sc_switch_screen(sf1_enter, sf1_update, NULL);
                return;
            }
            if (sg_level_index >= level_count) sg_level_index = 0;
            sc_switch_screen(si_enter, si_update, NULL);
        }
        else
            sc_switch_screen(sls_enter, sls_update, NULL);
    }
}

static void dump_collmap()
{
    u8* src = collmap;
    u8* dst = (u8*)0x4000;

    for (u8 y = 0; y < 32; y++) {
        for (u8 x = 0; x < 40; x++) {
            u8 cc = src[x + y * 40];
            if (cc < 10) cc += 128 + 48;
            else         cc += 128 + 48 + 7;
            *dst++ = cc;
            *dst++ = 1 << 4;
        }
    }
}

static void check_exit_level()
{
    if (level_exited) return;

    if (player_into_exit(&player_smarto) && player_into_exit(&player_dumber)) {
        exit_level();
    }
}

static void exit_level()
{
    level_exited = true;
    palette_fade_out_begin();

}

// #define DEBUG_COLLISION_RECTS

static void check_collisions_player_enemy()
{
    CollRect rs;
    CollRect rd;
    CollRect re;

    player_get_collrect(&player_smarto, &rs);
    player_get_collrect(&player_dumber, &rd);

#ifdef DEBUG_COLLISION_RECTS
    u8 y = 4;
    print_set_pos(0, y++);
    print_str("SMAR");
    print_set_pos(0, y++);
    print_dec_word(rs.x0);
    print_set_pos(0, y++);
    print_dec_word(rs.x1);
    print_set_pos(0, y++);
    print_dec_word(rs.y0);
    print_set_pos(0, y++);
    print_dec_word(rs.y1);
    y++;

    print_set_pos(0, y++);
    print_str("DUMB");
    print_set_pos(0, y++);
    print_dec_word(rs.x0);
    print_set_pos(0, y++);
    print_dec_word(rs.x1);
    print_set_pos(0, y++);
    print_dec_word(rs.y0);
    print_set_pos(0, y++);
    print_dec_word(rs.y1);
    y++;
#endif

    u8 ecount = enemy_get_count();
    for (u8 idx = 0; idx < ecount; idx++) {
        bool rok = enemy_get_rect(idx, &re);
        if (!rok) continue;

#ifdef DEBUG_COLLISION_RECTS
        print_set_pos(0, y++);
        print_char('E');
        print_dec_byte(idx);
        print_set_pos(0, y++);
        print_dec_word(re.x0);
        print_set_pos(0, y++);
        print_dec_word(re.x1);
        print_set_pos(0, y++);
        print_dec_word(re.y0);
        print_set_pos(0, y++);
        print_dec_word(re.y1);
        y++;
#endif        
        if (collrect_isec(&re, &rs)) {
            player_kill(&player_smarto);
        }
        if (collrect_isec(&re, &rd)) {
            player_kill(&player_dumber);
        }

    }
}

static void check_collisions_player_lever()
{
    CollRect rs;
    CollRect rd;
    CollRect rl;

    player_get_collrect(&player_smarto, &rs);
    player_get_collrect(&player_dumber, &rd);

    u8 ocount = lever_get_count();
    for (u8 idx = 0; idx < ocount; idx++) {
        ObjectLever* lever = lever_get(idx);
        rl.x0 = lever->asdef.x + 1;
        rl.x1 = lever->asdef.x + 14;
        rl.y0 = lever->asdef.y + 1;
        rl.y1 = lever->asdef.y + 14;

        bool stouch = collrect_isec(&rl, &rs);
        bool dtouch = collrect_isec(&rl, &rd);
        if (stouch || dtouch) {
            if (!lever->touch) {
                lever_move();
                lever->touch = true;
            }
        }
        else {
            lever->touch = false;
        }
    }
}

void sg_on_player_killed()
{
    if (player_killed) return;
    player_killed = true;
    sg_lives_lost++;
    palette_fade_out_begin();
}

static bool jump_key_pressed = false;
static bool check_level_jump()
{
    if (!moot) return false;

    if (zxkey0()) {
        jump_key_pressed = true;
        return true;
    }
    else if (jump_key_pressed) {
        jump_key_pressed = false;
        sg_level_index = 0;
        if (zxkey1()) sg_level_index |= 1;
        if (zxkey2()) sg_level_index |= 2;
        if (zxkey3()) sg_level_index |= 4;
        if (zxkey4()) sg_level_index |= 8;
        if (zxkey5()) sg_level_index |= 16;
        if (zxkey6()) sg_level_index |= 32;
        if (sg_level_index == 0) return false;
        sg_level_index--;
        // set everything to black
        palette_tilemap_all_black();
        palette_sprite_all_black();
        // reload
        sg_enter();
        return true;
    }
    return false;
}

// called from pause, restarts level killing player
void sg_restart()
{
    if (player_smarto.active) player_kill(&player_smarto);
    if (player_dumber.active) player_kill(&player_dumber);
}

// called from pause, goes back to main menu
void sg_exit()
{
    if (exit_to_menu) return;
    exit_to_menu = true;
    palette_fade_out_begin();
}
