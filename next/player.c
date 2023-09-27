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

#include "player.h"

#include "globaldefs.h"
#include "keyb.h"
#include "control.h"
#include "print_tile.h"
#include "screen_game.h"
#include "anisprite.h"
#include "persist.h"

Player player_smarto;
Player player_dumber;

extern ASAnim animdefs_smarto;
extern ASAnim animdefs_dumber;

// initial jumptable

// #define JUMPTABLE_SIZE 36
// s8 jumptable[JUMPTABLE_SIZE] = {
//     -3,-3,-3,-3,
//     -2,-2,-2,
//     -1,-1,-1,-1,-1,-1,-1,-1,
//     0,0,0,0,0,
//     1,1,1,1,1,1,1,1,
//     2,2,2,
//     3,3,3,3,
//     4};
// #define JUMPIDX 0
// #define FALLIDX 20

// second jumptable

// #define JUMPTABLE_SIZE 41
// s8 jumptable[JUMPTABLE_SIZE] = {
// 	-3,-3,-2,
// 	-3,-2,-2,-1,
// 	-2,-1,-1,-1,-1,-1,-1,
// 	-1,-1,
// 	0,0,0,0,0,0,0,0,
// 	1,1,
// 	1,1,1,1,1,1,2,
// 	1,2,2,3,
// 	2,3,3,
//     4};
// #define JUMPIDX 0
// #define FALLIDX 24

// definitive jumptable
#define JUMPTABLE_SIZE 41
s8 jumptable[JUMPTABLE_SIZE] = {
	-3,-3,-3,
	-2,-2,-2,-2,
	-1,-1,-1,-1,-1,-1,-1,
	-1,-1,
	0,0,0,0,0,0,0,0,
	1,1,
	1,1,1,1,1,1,1,
	2,2,2,2,
	3,3,3,
    4};
#define JUMPIDX 0
#define FALLIDX 24


static bool down_blocked(Player* player);
static bool is_on_other(Player* player, Player* other);
static bool left_blocked(Player* player);
static bool right_blocked(Player* player);
static bool into_ceil(Player* player);
static bool into_floor(Player* player);
static bool up_blocked(Player* player);
static bool on_conveyor(Player* player);
static bool on_conveyor_left(Player* player);
static void process_on_crumb_floor(Player* player);
static bool into_nasty(Player* player);

static void player_initialize(Player* player);
static void player_adjust_posX(Player* player);
static void player_adjust_posY(Player* player);
static void player_update_pos(Player* player);
static void player_debug_pos(Player* player, u8 x, u8 y);
static void player_update(Player* player, Player* other);
static void players_swap();
static void player_process_dead(Player* player);

static bool prev_swap = false;
static bool actionLeft, actionRight, actionJump;

extern u8 collmap[];

void players_setup(LevelDef* levdef)
{
    player_initialize(&player_smarto);
    player_smarto.asdef.pal = SPAL_SMARTO;
    player_smarto.cx = levdef->smarto_ini_cx;
    player_smarto.cy = levdef->smarto_ini_cy;
    player_smarto.asdef.animdefs = &animdefs_smarto;
    if (levdef->smarto_ini_lookleft)
        player_smarto.asdef.mirrot = ATTR_X___;
    else
        player_smarto.asdef.mirrot = ATTR_____;

    player_initialize(&player_dumber);
    player_dumber.asdef.pal = SPAL_DUMBER;
    player_dumber.cx = levdef->dumber_ini_cx;
    player_dumber.cy = levdef->dumber_ini_cy;
    player_dumber.asdef.animdefs = &animdefs_dumber;
    if (levdef->dumber_ini_lookleft)
        player_dumber.asdef.mirrot = ATTR_X___;
    else
        player_dumber.asdef.mirrot = ATTR_____;

    players_swap();
}

static void player_initialize(Player* player)
{
    player->asdef.pal = 0;
    player->asdef.anim = 0xff;
    player->cx = 0;
    player->cy = 0;
    player->dx = 0;
    player->dy = 0;
    player->vx = 0;
    player->vy = 0;
    player->active = false;
    player->airborne = false;
    player->dead = false;
    player->jumpidx = 0;
}

void players_update()
{
    bool curr_swap = actnDown;
    if (curr_swap && !prev_swap)
        players_swap();
    prev_swap = curr_swap;

    actionLeft = actnLeft;
    actionRight = actnRight;
    actionJump = actnUp;

    player_update(&player_smarto, &player_dumber);
    player_update(&player_dumber, &player_smarto);

    anisprite_update(&(player_smarto.asdef));
    anisprite_update(&(player_dumber.asdef));

    // player_debug_pos(&player_smarto, 0, 8);
    // player_debug_pos(&player_dumber, 36, 8);
}

static void players_swap()
{
    AniSprite* asdef;
    if (!player_smarto.active) {
        player_smarto.active = true;
        player_smarto.asdef.slot = SS_PL_ACT;

        player_dumber.active = false;
        player_dumber.asdef.slot = SS_PL_INA;
    }
    else {
        player_dumber.active = true;
        player_dumber.asdef.slot = SS_PL_ACT;

        player_smarto.active = false;
        player_smarto.asdef.slot = SS_PL_INA;
    }
}

static void player_update(Player* player, Player* other)
{
    if (moot && player->active && zxkeyK() && zxkeySYM())
        player_kill(player);

    if (player->dead) {
        player_process_dead(player);
        return;
    }

    if (into_nasty(player)) {
        player_kill(player);
        return;
    }

    player->vx = 0;
    player->vy = 0;

    AniSprite* asdef = &(player->asdef);

    // evaluate player actions
    if (player->active) {
        if (actionLeft && !actionRight) {
            anisprite_set_anim(asdef, PLAYER_ANIM_WALK);
            asdef->mirrot = ATTR_X___;
            player->vx--;
        }
        else if (actionRight && !actionLeft) {
            anisprite_set_anim(asdef, PLAYER_ANIM_WALK);
            asdef->mirrot = ATTR_____;
            player->vx++;
        }
        else {
            anisprite_set_anim(asdef, PLAYER_ANIM_IDLE);
        }

        if (!player->airborne && actionJump && !up_blocked(player)) {
            player->airborne = true;
            player->jumpidx = JUMPIDX;
        }
    }
    else {
        anisprite_set_anim(asdef, PLAYER_ANIM_SLEEP);
    }

    // evaluate conveyor
    if (on_conveyor(player)) {
        if (on_conveyor_left(player))
            player->vx--;
        else
            player->vx++;
    }

    // apply horizontal velocity
    while (player->vx < 0 && !left_blocked(player)) {
        player->vx++;
        player->dx--;
        player_adjust_posX(player);
    }
    while (player->vx > 0 && !right_blocked(player)) {
        player->vx--;
        player->dx++;
        player_adjust_posX(player);
    }

    // evaluate player flight (I)
    if (player->airborne) {
        if (player->jumpidx < JUMPTABLE_SIZE)
            player->vy = jumptable[player->jumpidx];
        else
            player->vy = jumptable[JUMPTABLE_SIZE - 1];
        player->jumpidx++;
        player->dy += player->vy;
    }
    else {
        player->jumpidx = FALLIDX;
    }

    player_adjust_posY(player);

    // evaluate player on other
    if (player->vy >= 0 && is_on_other(player, other) && !down_blocked(player)) {
        player->airborne = other->airborne;
        player->dy = other->dy;
        player->cy = other->cy - 2;
    }
    if (player->vy < 0 && is_on_other(other, player) && !up_blocked(other)) {
        other->dy = player->dy;
        other->cy = player->cy - 2;
        other->jumpidx = player->jumpidx;
        // other->airborne = player->airborne;
    }

    // evaluate player_flight(2)
    if (player->airborne) {
        if (player->vy > 0) {
            if (into_floor(player) && !into_ceil(player)) {
                player->airborne = false;
                player->dy = 0;
                player->cy--;
            }
            else if (player->dy < 4 && down_blocked(player)) {
                player->airborne = false;
                player->dy = 0;
            }
        }
        else if (player->vy < 0 && into_ceil(player)) {
            player->dy = 8;
            player->jumpidx = FALLIDX;
        }
    }
    else {
        if (!down_blocked(player) && !is_on_other(player, other)) {
            player->airborne = true;
        }
    }

    player_adjust_posY(player)
    ;
    player_update_pos(player);

    if (player->airborne)
        player->crumb_ctr = 0;
    else
        process_on_crumb_floor(player);
}

static bool down_blocked(Player* player)
{
    u8* src = collmap + player->cx + 40 * (player->cy + 2);
    u8 A = *src++;
    u8 B = *src++;
    u8 C = *src;
	if (player->dx == 0) {
		if (A & TMD || B & TMD) return true;
    }
	else {
		if (A & TMD || B & TMD || C & TMD) return true;
    }
	return false;
}

static bool is_on_other(Player* player, Player* other)
{
    AniSprite* pasdef = &player->asdef;
    AniSprite* oasdef = &other->asdef;
    s16 x = pasdef->x - oasdef->x;
    s8  y = pasdef->y - oasdef->y;
    if (x < -12 || x >  12) return false;
    if (y < -18 || y > -14) return false;
    return true;
}

static bool left_blocked(Player* player)
{
    #define LMASK (TMA|TMG)
    u8* src = collmap + player->cx - 1 + 40 * (player->cy);
    u8 A = *src;
    src += 40;
    u8 B = *src;
    src += 40;
    u8 C = *src;
	if (player->dx != 0) return false;
    if (player->dy == 0) {
		if (A & LMASK || B & LMASK) return true;
    }
	else {
		if (A & LMASK || B & LMASK || C & LMASK) return true;
    }
	return false;
}

static bool right_blocked(Player* player)
{
    #define RMASK (TMA|TMG)
    u8* src = collmap + player->cx + 2 + 40 * (player->cy);
    u8 A = *src;
    src += 40;
    u8 B = *src;
    src += 40;
    u8 C = *src;
	if (player->dx != 0) return false;
    if (player->dy == 0) {
		if (A & RMASK || B & RMASK) return true;
    }
	else {
		if (A & RMASK || B & RMASK || C & RMASK) return true;
    }
	return false;
}

static bool into_ceil(Player* player)
{
    u8* src = collmap + player->cx + 40 * (player->cy);
    u8 A = *src++;
    u8 B = *src++;
    u8 C = *src;
	if (player->dx == 0) {
		if (A & TMA || B & TMA) return true;
    }
	else {
		if (A & TMA || B & TMA || C & TMA) return true;
    }
	return false;
}

static bool into_floor(Player* player)
{
    u8* src = collmap + player->cx + 40 * (player->cy + 1);
    u8 A = *src++;
    u8 B = *src++;
    u8 C = *src;
	if (player->dx == 0) {
		if (A & TMA || B & TMA) return true;
    }
	else {
		if (A & TMA || B & TMA || C & TMA) return true;
    }
	return false;
}

static bool up_blocked(Player* player)
{
    u8* src = collmap + player->cx + 40 * (player->cy - 1);
    u8 A = *src++;
    u8 B = *src++;
    u8 C = *src;
	if (player->dx == 0) {
		if (A & TMA || B & TMA) return true;
    }
	else {
		if (A & TMA || B & TMA || C & TMA) return true;
    }
	return false;
}

// 0000000000111111111122222222223333333333
// 0123456789012345678901234567890123456789
// 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ
const char* dbg_chtable = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

bool player_into_exit(Player* player)
{
    u8* src = collmap + player->cx + 40 * (player->cy);
    u8 A = *src++;
    u8 B = *src++;
    u8 C = *src;
    src += 38;
    u8 D = *src++;
    u8 E = *src++;
    u8 F = *src;
    if (A & TME || B & TME || D & TME || E & TME) return true;
    if ((player->dx != 0) && (C & TME || F & TME)) return true;
    if (player->dy != 0) {
        src += 38;
        u8 G = *src++;
        u8 H = *src++;
        u8 I = *src;
        if (G & TME || H & TME) return true;
        if ((player->dx != 0) && (I & TME)) return true;
    }
    return false;
}


static bool on_conveyor(Player* player)
{
    u8* src = collmap + player->cx + 40 * (player->cy + 2);
    u8 A = *src++;
    u8 B = *src++;
    u8 C = *src;
	if (player->dx == 0) {
		if (A & TMC || B & TMC) return true;
    }
	else {
		if (A & TMC || B & TMC || C & TMC) return true;
    }
	return false;
}

static bool on_conveyor_left(Player* player)
{
    u8* src = collmap + player->cx + 40 * (player->cy + 2);
    u8 A = *src++;
    u8 B = *src++;
    u8 C = *src;
	if (player->dx == 0) {
		if (A & TML || B & TML) return true;
    }
	else {
		if (A & TML || B & TML || C & TML) return true;
    }
	return false;
}

static void process_on_crumb_floor(Player* player)
{
    if (player->crumb_ctr > 0) {
        player->crumb_ctr--;
        return;
    }
    player->crumb_ctr = CRUMB_CTR_MAX;

    u16 offset = player->cx + 40 * (player->cy + 2);

    u8* tcol = collmap + offset;
    u8 A = *tcol;
    if (A & TMR) {
        u8* tgra = 0x4000 + offset + offset;
        u8 hl = *tgra;
        u8 h = hl & 0xF0;
        u8 l = hl & 0x0F;
        l --;
        *tgra = h|l;
        if (l == 0)
            *tcol = 0;
    }

    tcol++;
    u8 B = *tcol;
    if (B & TMR) {
        u8* tgra = 0x4000 + offset + offset + 2;
        u8 hl = *tgra;
        u8 h = hl & 0xF0;
        u8 l = hl & 0x0F;
        l --;
        *tgra = h|l;
        if (l == 0)
            *tcol = 0;
    }

    if (player->dx == 0)
        return;

    tcol++;
    u8 C = *tcol;
    if (C & TMR) {
        u8* tgra = 0x4000 + offset + offset + 4;
        u8 hl = *tgra;
        u8 h = hl & 0xF0;
        u8 l = hl & 0x0F;
        l --;
        *tgra = h|l;
        if (l == 0)
            *tcol = 0;
    }
}

    // if (player->active) {
    //     u8 y = 0;
    //     print_set_pos(0,y++); print_str("cx:"); print_hex_byte(player->cx);
    //     print_set_pos(0,y++); print_str("cy:"); print_hex_byte(player->cy);
    //     print_set_pos(0,y++); print_str("dx:"); print_hex_byte(player->dx);
    //     print_set_pos(0,y++); print_str("dy:"); print_hex_byte(player->dy);
    // }
    
    // if (player->active) {
    //     u8 y = 0;
    //     print_set_pos(36,y++); print_str("A:"); print_hex_byte(A);
    //     print_set_pos(36,y++); print_str("B:"); print_hex_byte(B);
    //     print_set_pos(36,y++); print_str("C:"); print_hex_byte(C);
    //     print_set_pos(36,y++); print_str("D:"); print_hex_byte(D);
    //     print_set_pos(36,y++); print_str("E:"); print_hex_byte(E);
    //     print_set_pos(36,y++); print_str("F:"); print_hex_byte(F);
    // }
    

// A B C
// D E F
// G H I
static bool into_nasty(Player* player)
{
    u8* src = collmap + player->cx + 40 * (player->cy);
    u8 A = *src++;
    u8 B = *src++;
    u8 C = *src;
    src += 38;
    u8 D = *src++;
    u8 E = *src++;
    u8 F = *src;
    if (A & TMN || B & TMN || D & TMN || E & TMN) return true;
    if ((player->dx != 0) && (C & TMN || F & TMN)) return true;
    if (player->dy != 0) {
        src += 38;
        u8 G = *src++;
        u8 H = *src++;
        u8 I = *src;
        if (G & TMN || H & TMN) return true;
        if ((player->dx != 0) && (I & TMN)) return true;
    }
    return false;
}


static void player_adjust_posX(Player* player)
{
    player->cx += player->dx >> 3;
    player->dx &= 7;
}
static void player_adjust_posY(Player* player)
{
    player->cy += player->dy >> 3;
    player->dy &= 7;
}

static void player_update_pos(Player* player)
{
    u16 x = player->cx * 8 + player->dx;
    u16 y = player->cy * 8 + player->dy;
    AniSprite* asdef = &player->asdef;
    asdef->x = x;
    asdef->y = y;
}

void player_kill(Player* player)
{
    if (player->dead)
        return;

    anisprite_set_anim(&player->asdef, PLAYER_ANIM_DEAD);
    player->dead = true;
    if (player->cy < 16)
        player->vy = -16;
    else
        player->vy = -24;
}

static void player_process_dead(Player* player)
{
    if (player->asdef.y < 260) {
        player->asdef.x += player->vx;
        player->asdef.y += player->vy >> 2;
        player->vy += 1;
    }
    else {
        sg_on_player_killed();
    }
}

void player_get_collrect(Player* player, CollRect* r)
{
    r->x0 = player->asdef.x + 1;
    r->x1 = player->asdef.x + 14;
    r->y0 = player->asdef.y + 1;
    r->y1 = player->asdef.y + 14;
}

// static void player_debug_pos(Player* player, u8 x, u8 y)
// {
//     print_set_pos(x, y++);
//     print_str("cx");
//     print_hex_byte(player->cx);
//     print_set_pos(x, y++);
//     print_str("dx");
//     print_hex_byte(player->dx);
//     print_set_pos(x, y++);
//     print_str("cy");
//     print_hex_byte(player->cy);
//     print_set_pos(x, y++);
//     print_str("dy");
//     print_hex_byte(player->dy);
//     print_set_pos(x, y++);
//     print_str("ji");
//     u8 ji = player->jumpidx;
//     if (ji < 10) print_char(' ');
//     print_dec_byte(player->jumpidx);
//     print_set_pos(x, y++);
//     print_str("jt");
//     s8 jt = jumptable[player->jumpidx];
//     if (jt < 0) {
//         print_char('-');
//         print_hex_nibble(-jt);
//     }
//     else {
//         print_char('+');
//         print_hex_nibble(jt);
//     }

// }
