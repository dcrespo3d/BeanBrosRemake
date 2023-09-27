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

#ifndef __globaldefs_h__
#define __globaldefs_h__

// tile masks
#define TMD 1 // collision in down direction only
#define TMA 2 // collision in all directions
#define TMC 4 // conveyor flag
#define TML 8 // conveyor left flag
#define TMG 16 // door/gate flag
#define TME 32 // exit
#define TMN 64 // nasty
#define TMR 128 // cRumb

// player animations
#define PLAYER_ANIM_WALK 0
#define PLAYER_ANIM_IDLE 1
#define PLAYER_ANIM_SLEEP 2
#define PLAYER_ANIM_DEAD 3

// crumb inhibition counter max value
#define CRUMB_CTR_MAX 3

// sprite slots for game screen
#define SS_OBJECT_FIRST 0
#define SS_PL_INA 58
#define SS_PL_ACT 59
#define SS_PAUSE_DLG 60
#define SS_PAUSE_CUR 62
#define SS_PAUSE_TEXT 64

// object types
#define OTYPE_ENEMYH 1
#define OTYPE_ENEMYV 2
#define OTYPE_ENEMYD 3
#define OTYPE_SPIKES 4
#define OTYPE_DOOR   5
#define OTYPE_LEVER  6
#define OTYPE_CONV   7

// maximum counts for array dimensioning
#define ENEMY_HORIZ_MAX  8
#define ENEMY_VERT_MAX   8
#define ENEMY_DIAG_MAX   8
#define ENEMY_SPIKES_MAX 10
#define OBJECT_DOOR_MAX  8
#define OBJECT_LEVER_MAX 8
#define OBJECT_CONV_MAX  8

// palette indices for sprites
#define SPAL_SMARTO 0
#define SPAL_DUMBER 1
#define SPAL_ENEMY1 2
#define SPAL_ENEMY2 3
#define SPAL_ENEMY3 4
#define SPAL_SPIKES 2
#define SPAL_LEVER  5
#define SPAL_DOOR   6
#define SPAL_SDFEET 7
#define SPAL_SPFONT 8

// palette indices for tiles
#define TPAL_DEF 0
#define TPAL_RED_ON_GRAY 10
#define TPAL_GREEN_ON_GRAY 11
#define TPAL_GREEN 12
#define TPAL_LGRAY 13
#define TPAL_RED   14
#define TPAL_BLUE  15

// for EnemySpikes
#define ENEMY_SPIKES_LEAVE 0
#define ENEMY_SPIKES_ENTER 1
#define ENEMY_SPIKES_FR_ON 50
#define ENEMY_SPIKES_FR_OFF 200

// anims for door
#define OBJECT_DOOR_OPEN 0
#define OBJECT_DOOR_CLOSE 1

// anims for lever
#define OBJECT_LEVER_OPEN 0
#define OBJECT_LEVER_CLOSE 1

// generic messages
#define MSG_PRESS_ANY_KEY "press any key"
#define MSG_PRESS_THE_ANY_KEY "press the any key"

// special chars
#define CHAR_SKULL     '\x0B'
#define CHAR_BUT1      '\x0C'
#define CHAR_BUT2      '\x0D'
#define CHAR_BUT3      '\x0E'
#define CHAR_BUT4      '\x0F'
#define CHAR_UP        '\x10'
#define CHAR_DOWN      '\x11'
#define CHAR_LEFT      '\x12'
#define CHAR_RIGHT     '\x13'
#define CHAR_DELETE    '\x14'
#define CHAR_EDIT      '\x15'
#define CHAR_BREAK     '\x16'
#define CHAR_INVVIDEO  '\x17'
#define CHAR_TRUVIDEO  '\x18'
#define CHAR_GRAPH     '\x19'
#define CHAR_CAPSLOCK  '\x1A'
#define CHAR_EXTEND    '\x1B'
#define CHAR_SPACE     '\x1C'
#define CHAR_ENTER     '\x1D'
#define CHAR_CAPSSHIFT '\x1E'
#define CHAR_SYMBSHIFT '\x1F'

#endif