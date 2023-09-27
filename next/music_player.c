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

#include "music_player.h"
#include "print_tile.h"

u8* current_song = 0;

u8 prev_page_a;
u8 prev_page_b;

static void panic();

void music_song_init(u8* song)
{
    if (current_song == song)
        return;

    current_song = song;

    if (!current_song) {
        panic();
        return;
    }

__asm
    EXTERN _PLY_AKG_Init
    EXTERN _PLY_AKG_Play

 	NEXTREG $50, 36
	NEXTREG $51, 37
    ld hl, (_current_song)   ; The address of the music.
    ld a,0                  ; What subsong to play (starts at 0)?
    call _PLY_AKG_Init      ; Call the init method of the player. 
__endasm

// PITFALL!!
// ld hl, _song_meteo        ; direct symbol
// ld hl, (_current_song)    ; indirect symbol !!!

}

void music_song_play_frame()
{
    if (!current_song)
        return;
__asm
    EXTERN _PLY_AKG_Play
    EXTERN readNextReg

    // store current pages
    LD A,   $50
    CALL    readNextReg
    LD      (_prev_page_a), A
    LD A,   $51
    CALL    readNextReg
    LD      (_prev_page_b), A

    // set music player and songs pages
    NEXTREG $50, 36
    NEXTREG $51, 37

    // play one frame of music
    call    _PLY_AKG_Play

    // restore original pages
    LD      A, (_prev_page_a)
    NEXTREG $50, A
    LD      A, (_prev_page_b)
    NEXTREG $51, A
__endasm
}

static void panic()
{
#define REG_MIXER       7
#define REG_ENV_SHAPE  13
__asm
    LD D, $FF
    LD A, REG_MIXER

    LD BC, $FFFD
    OUT (C), A

    LD BC, $BFFD
    OUT (C), D

    LD D, $00
    LD A, REG_ENV_SHAPE

    LD BC, $FFFD
    OUT (C), A

    LD BC, $BFFD
    OUT (C), D
__endasm
}

