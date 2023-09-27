;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;  Bean Brothers (remake) - for ZX Spectrum Next / N-Go
;
;  (c) 2023 David Crespo - https://github.com/dcrespo3d
;                          https://davidprograma.itch.io
;                          https://www.youtube.com/@Davidprograma
;
;  Based on Bean Brothers for ZX Spectrum 48/128K - (c) 2018 Dave Hughes
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;  This program is free software: you can redistribute it and/or modify
;  it under the terms of the GNU General Public License as published by
;  the Free Software Foundation, either version 3 of the License, or
;  (at your option) any later version.  
;
;  This program is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;  GNU General Public License for more details.
; 
;  You should have received a copy of the GNU General Public License
;  along with this program.  If not, see <https://www.gnu.org/licenses/>. 
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

SECTION PAGE_38_PALETTE_DATA

ORG $0000

;---------------------------------------------------------------------

PUBLIC _tileset_palette
PUBLIC _tileset_palette_len

_tileset_palette:
	INCBIN "res/tileset_palette.bin"
_tileset_palette_len: EQU $-_tileset_palette

PUBLIC _tileset_palette_1
PUBLIC _tileset_palette_2
PUBLIC _tileset_palette_3
PUBLIC _tileset_palette_4
PUBLIC _tileset_palette_5
PUBLIC _tileset_palette_6
PUBLIC _tileset_palette_7

_tileset_palette_1:
	INCBIN "res/tileset_palette_1.bin"
_tileset_palette_2:
	INCBIN "res/tileset_palette_2.bin"
_tileset_palette_3:
	INCBIN "res/tileset_palette_3.bin"
_tileset_palette_4:
	INCBIN "res/tileset_palette_4.bin"
_tileset_palette_5:
	INCBIN "res/tileset_palette_5.bin"
_tileset_palette_6:
	INCBIN "res/tileset_palette_6.bin"
_tileset_palette_7:
	INCBIN "res/tileset_palette_7.bin"

;---------------------------------------------------------------------

PUBLIC _sprite_game_palette
PUBLIC _sprite_game_palette_len

_sprite_game_palette:
    INCBIN "res/sprite_game_defs_pal.bin"
_sprite_game_palette_len: EQU $-_sprite_game_palette

PUBLIC _sprite_game_palette_1
PUBLIC _sprite_game_palette_2
PUBLIC _sprite_game_palette_3
PUBLIC _sprite_game_palette_4
PUBLIC _sprite_game_palette_5
PUBLIC _sprite_game_palette_6
PUBLIC _sprite_game_palette_7

_sprite_game_palette_1:
    INCBIN "res/sprite_game_defs_pal_1.bin"
_sprite_game_palette_2:
    INCBIN "res/sprite_game_defs_pal_2.bin"
_sprite_game_palette_3:
    INCBIN "res/sprite_game_defs_pal_3.bin"
_sprite_game_palette_4:
    INCBIN "res/sprite_game_defs_pal_4.bin"
_sprite_game_palette_5:
    INCBIN "res/sprite_game_defs_pal_5.bin"
_sprite_game_palette_6:
    INCBIN "res/sprite_game_defs_pal_6.bin"
_sprite_game_palette_7:
    INCBIN "res/sprite_game_defs_pal_7.bin"

;---------------------------------------------------------------------

PUBLIC _sprite_menu_palette
PUBLIC _sprite_menu_palette_len

_sprite_menu_palette:
    INCBIN "res/sprite_menu_defs_pal.bin"
_sprite_menu_palette_len: EQU $-_sprite_menu_palette

PUBLIC _sprite_menu_palette_1
PUBLIC _sprite_menu_palette_2
PUBLIC _sprite_menu_palette_3
PUBLIC _sprite_menu_palette_4
PUBLIC _sprite_menu_palette_5
PUBLIC _sprite_menu_palette_6
PUBLIC _sprite_menu_palette_7

_sprite_menu_palette_1:
    INCBIN "res/sprite_menu_defs_pal_1.bin"
_sprite_menu_palette_2:
    INCBIN "res/sprite_menu_defs_pal_2.bin"
_sprite_menu_palette_3:
    INCBIN "res/sprite_menu_defs_pal_3.bin"
_sprite_menu_palette_4:
    INCBIN "res/sprite_menu_defs_pal_4.bin"
_sprite_menu_palette_5:
    INCBIN "res/sprite_menu_defs_pal_5.bin"
_sprite_menu_palette_6:
    INCBIN "res/sprite_menu_defs_pal_6.bin"
_sprite_menu_palette_7:
    INCBIN "res/sprite_menu_defs_pal_7.bin"

;---------------------------------------------------------------------

PUBLIC _layer2_menu_imgs_pal
_layer2_menu_imgs_pal:
    INCBIN "res/layer2_menu_imgs_pal.bin"

PUBLIC _layer2_menu_imgs_pal_1
PUBLIC _layer2_menu_imgs_pal_2
PUBLIC _layer2_menu_imgs_pal_3
PUBLIC _layer2_menu_imgs_pal_4
PUBLIC _layer2_menu_imgs_pal_5
PUBLIC _layer2_menu_imgs_pal_6
PUBLIC _layer2_menu_imgs_pal_7

_layer2_menu_imgs_pal_1:
    INCBIN "res/layer2_menu_imgs_pal_1.bin"
_layer2_menu_imgs_pal_2:
    INCBIN "res/layer2_menu_imgs_pal_2.bin"
_layer2_menu_imgs_pal_3:
    INCBIN "res/layer2_menu_imgs_pal_3.bin"
_layer2_menu_imgs_pal_4:
    INCBIN "res/layer2_menu_imgs_pal_4.bin"
_layer2_menu_imgs_pal_5:
    INCBIN "res/layer2_menu_imgs_pal_5.bin"
_layer2_menu_imgs_pal_6:
    INCBIN "res/layer2_menu_imgs_pal_6.bin"
_layer2_menu_imgs_pal_7:
    INCBIN "res/layer2_menu_imgs_pal_7.bin"

