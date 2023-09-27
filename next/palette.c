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

#include "palette.h"
#include "paging.h"

static u8* tileset_palette_arr[] = {
    tileset_palette,
    tileset_palette_1,
    tileset_palette_2,
    tileset_palette_3,
    tileset_palette_4,
    tileset_palette_5,
    tileset_palette_6,
    tileset_palette_7,
};

static u8* sprite_menu_palette_arr[] = {
    sprite_menu_palette,
    sprite_menu_palette_1,
    sprite_menu_palette_2,
    sprite_menu_palette_3,
    sprite_menu_palette_4,
    sprite_menu_palette_5,
    sprite_menu_palette_6,
    sprite_menu_palette_7,
};

static u8* sprite_game_palette_arr[] = {
    sprite_game_palette,
    sprite_game_palette_1,
    sprite_game_palette_2,
    sprite_game_palette_3,
    sprite_game_palette_4,
    sprite_game_palette_5,
    sprite_game_palette_6,
    sprite_game_palette_7,
};

static u8* layer2_menu_imgs_pal_arr[] = {
    layer2_menu_imgs_pal,
    layer2_menu_imgs_pal_1,
    layer2_menu_imgs_pal_2,
    layer2_menu_imgs_pal_3,
    layer2_menu_imgs_pal_4,
    layer2_menu_imgs_pal_5,
    layer2_menu_imgs_pal_6,
    layer2_menu_imgs_pal_7,
};

static u8 state;
#define STATE_FADEIN  1
#define STATE_PLAY    2
#define STATE_FADEOUT 3
static u8 fade_ctr = 0;

static FadeCb fadecb = NULL;
static void on_state_fadeout();
static void on_state_fadein();

static u8 screenType = 0;

#define FADE_SLOWNESS_EXPONENT 1

#include "sprite_manager.h"
#include "tilemap_manager.h"

#include "music_player.h"

// load tilemap palette
void palette_tilemap_load(u8* palette);

// load sprite palette
void palette_sprite_load(u8* palette);

// load layer 2 palette
void palette_layer2_load(u8* palette);


void palette_fade_init(u8 _screenType, FadeCb _fadecb)
{
    storePrevPagesAtSlots0and1();
    putPagesAtSlots0and1(38);

    // set everything to black
    palette_tilemap_all_black();
    palette_sprite_all_black();
    palette_layer2_all_black();

    // clear tilemap buffer
    tilemap_clear_buf();

    // clear sprites
    sprite_clear_all_slots();

    AVOID_MUSIC_STUTTER;

    screenType = _screenType;
    if (screenType == SCREEN_TYPE_MENU)
    {
        sprite_load_menu_defs();
    }
    else if (screenType == SCREEN_TYPE_GAME)
    {
        sprite_load_game_defs();
    }

    fadecb = _fadecb;

    fade_ctr = 1 << FADE_SLOWNESS_EXPONENT;
    state = STATE_FADEIN;

    AVOID_MUSIC_STUTTER;

    restorePrevPagesAtSlots0and1();
}

void palette_fade_update()
{
    // print_set_pos(36,4);
    // print_hex_byte(state);
    // print_set_pos(36,5);
    // print_hex_byte(fade_ctr);

    if (state == STATE_PLAY)
    {
        fadecb(0);
    }
    else if (state == STATE_FADEOUT)
    {
        on_state_fadeout();
    }
    else if (state == STATE_FADEIN)
    {
        on_state_fadein();
    }

}

void palette_fade_out_begin()
{
    fade_ctr = 1 << FADE_SLOWNESS_EXPONENT;
    state = STATE_FADEOUT;
}

static void update_palette(u8 palval)
{
    storePrevPagesAtSlots0and1();
    putPagesAtSlots0and1(38);

    u8* pal;
    pal = tileset_palette_arr[palval];
    palette_tilemap_load(pal);
    if (screenType == SCREEN_TYPE_MENU)
    {
        pal = sprite_menu_palette_arr[palval];
        palette_sprite_load(pal);
        pal = layer2_menu_imgs_pal_arr[palval];
        palette_layer2_load(pal);
    }
    else if (screenType == SCREEN_TYPE_GAME)
    {
        pal = sprite_game_palette_arr[palval];
        palette_sprite_load(pal);
    }

    restorePrevPagesAtSlots0and1();
}

static void on_state_fadeout()
{
    u8 palval = fade_ctr >> FADE_SLOWNESS_EXPONENT;
    if (palval > 7) palval = 7;
    update_palette(palval);

    fadecb(palval);
    fade_ctr++;
}

static void on_state_fadein()
{
    u8 palval = 7 - (fade_ctr >> FADE_SLOWNESS_EXPONENT);
    if (palval > 7) palval = 7;
    update_palette(palval);

    fadecb(-palval);
    fade_ctr++;
    if (palval < 1) {
        state = STATE_PLAY;
    }
}


