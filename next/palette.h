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

#ifndef __palette_h__
#define __palette_h__

#include "types.h"

// default palettes
extern u8 tileset_palette[];
extern u8 sprite_game_palette[];
extern u8 sprite_menu_palette[];
extern u8 layer2_menu_imgs_pal[];

extern u8 tileset_palette_1[];
extern u8 tileset_palette_2[];
extern u8 tileset_palette_3[];
extern u8 tileset_palette_4[];
extern u8 tileset_palette_5[];
extern u8 tileset_palette_6[];
extern u8 tileset_palette_7[];

extern u8 sprite_game_palette_1[];
extern u8 sprite_game_palette_2[];
extern u8 sprite_game_palette_3[];
extern u8 sprite_game_palette_4[];
extern u8 sprite_game_palette_5[];
extern u8 sprite_game_palette_6[];
extern u8 sprite_game_palette_7[];

extern u8 sprite_menu_palette_1[];
extern u8 sprite_menu_palette_2[];
extern u8 sprite_menu_palette_3[];
extern u8 sprite_menu_palette_4[];
extern u8 sprite_menu_palette_5[];
extern u8 sprite_menu_palette_6[];
extern u8 sprite_menu_palette_7[];

extern u8 layer2_menu_imgs_pal_1[];
extern u8 layer2_menu_imgs_pal_2[];
extern u8 layer2_menu_imgs_pal_3[];
extern u8 layer2_menu_imgs_pal_4[];
extern u8 layer2_menu_imgs_pal_5[];
extern u8 layer2_menu_imgs_pal_6[];
extern u8 layer2_menu_imgs_pal_7[];

extern u8 palette_temp[];

// set tilemap palette all black
void palette_tilemap_all_black();

// set sprite palette all black
void palette_sprite_all_black();

// set layer 2 palette all black
void palette_layer2_all_black();

#define SCREEN_TYPE_MENU 1
#define SCREEN_TYPE_GAME 2

// FadeCb is a pointer to a function accepting a signed s8 argument
// and returning void
typedef void (*FadeCb)(s8);

void palette_fade_init(u8 _screenType, FadeCb _fadecb);

void palette_fade_update();

void palette_fade_out_begin();

#endif