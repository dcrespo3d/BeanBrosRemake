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

#include "object.h"
#include "globaldefs.h"
#include "print_tile.h"

// sprite slot
u8 ss_object;

u8 enemy_horiz_count;
u8 enemy_vert_count;
u8 enemy_diag_count;
u8 enemy_spikes_count;
u8 object_lever_count;
u8 object_door_count;
u8 object_conv_count;

EnemyHoriz  enemy_horiz_arr [ENEMY_HORIZ_MAX];
EnemyVert   enemy_vert_arr  [ENEMY_VERT_MAX];
EnemyDiag   enemy_diag_arr  [ENEMY_DIAG_MAX];
EnemySpikes enemy_spikes_arr[ENEMY_SPIKES_MAX];
ObjectDoor  object_door_arr [OBJECT_DOOR_MAX];
ObjectLever object_lever_arr[OBJECT_LEVER_MAX];
ObjectConv  object_conv_arr [OBJECT_CONV_MAX];

extern ASAnim animdefs_enemy1;
extern ASAnim animdefs_enemy2;
extern ASAnim animdefs_enemy3;
extern ASAnim animdefs_spikes;
extern ASAnim animdefs_lever;
extern ASAnim animdefs_door;

static void enemy_horiz_setup (ObjectDef* odef);
static void enemy_vert_setup  (ObjectDef* odef);
static void enemy_diag_setup  (ObjectDef* odef);
static void enemy_spikes_setup(ObjectDef* odef);
static void object_lever_setup(ObjectDef* odef);
static void object_door_setup (ObjectDef* odef);
static void object_conv_setup (ObjectDef* odef);

static void enemy_horiz_update (EnemyHoriz*  e);
static void enemy_vert_update  (EnemyVert*   e);
static void enemy_diag_update  (EnemyDiag*   e);
static void enemy_spikes_update(EnemySpikes* e);
static void object_lever_update(ObjectLever* o);
static void object_door_update (ObjectDoor*  o);
static void object_conv_update (ObjectConv*  o);

static void object_door_closure_toggle(ObjectDoor* o);
static void object_door_closure_update(ObjectDoor* o);
static void object_lever_closure_toggle(ObjectLever* o);
static void object_lever_closure_update(ObjectLever* o);

// collision map used for some objects
extern u8 collmap[];

// for debug traces
static u8 dbgy = 0;

void objects_setup(LevelDef* levdef)
{
    enemy_horiz_count = 0;
    enemy_vert_count = 0;
    enemy_diag_count = 0;
    enemy_spikes_count = 0;
    object_lever_count = 0;
    object_door_count = 0;
    object_conv_count = 0;
    ss_object = SS_OBJECT_FIRST;

    for (u8 ienemy = 0; ienemy < levdef->enemy_count; ienemy++) {
        ObjectDef* odef = &levdef->enemy_defs[ienemy];
        if      (odef->type == OTYPE_ENEMYH) enemy_horiz_setup (odef);
        else if (odef->type == OTYPE_ENEMYV) enemy_vert_setup  (odef);
        else if (odef->type == OTYPE_ENEMYD) enemy_diag_setup  (odef);
        else if (odef->type == OTYPE_SPIKES) enemy_spikes_setup(odef);
        else if (odef->type == OTYPE_LEVER)  object_lever_setup(odef);
        else if (odef->type == OTYPE_DOOR)   object_door_setup (odef);
        else if (odef->type == OTYPE_CONV)   object_conv_setup (odef);
    }
}

static void enemy_horiz_setup(ObjectDef* odef)
{
    if (enemy_horiz_count >= ENEMY_HORIZ_MAX)
        return;

    EnemyHoriz* e = &enemy_horiz_arr[enemy_horiz_count++];
    e->asdef.slot = ss_object++;
    e->asdef.x = 8 * ((u16)odef->ini_cx);
    e->asdef.y = 8 * ((u16)odef->ini_cy);
    e->advmask = odef->extra1 & 0x0F;
    u8 animidx = (odef->extra1 & 0xF0) >> 4;
    if (animidx == 0) {
        e->asdef.animdefs = &animdefs_enemy1;
        e->asdef.pal = SPAL_ENEMY1;
    }
    else if (animidx == 1) {
        e->asdef.animdefs = &animdefs_enemy2;
        e->asdef.pal = SPAL_ENEMY2;
    }
    else if (animidx == 2) {
        e->asdef.animdefs = &animdefs_enemy3;
        e->asdef.pal = SPAL_ENEMY3;
    }
    e->asdef.anim = 0xff;
    anisprite_set_anim(&e->asdef, 0);

    e->xmin = e->asdef.x + 8 * ((u16)odef->extra2);
    if (e->xmin > 10000) e->xmin = 0;
    e->xmax = e->asdef.x + 8 * ((u16)odef->extra3);
    if (e->advmask < 8)
        e->deltax = 1;
    else {
        e->deltax = -1;
        e->advmask = 16 - e->advmask; // 4 bit sign extension
    }
    e->fctr = e->advmask;
}

static void enemy_vert_setup(ObjectDef* odef)
{
    if (enemy_vert_count >= ENEMY_VERT_MAX)
        return;

    EnemyVert* e = &enemy_vert_arr[enemy_vert_count++];
    e->asdef.slot = ss_object++;
    e->asdef.x = 8 * ((u16)odef->ini_cx);
    e->asdef.y = 8 * ((u16)odef->ini_cy);
    e->advmask = odef->extra1 & 0x0F;
    u8 animidx = (odef->extra1 & 0xF0) >> 4;
    if (animidx == 0) {
        e->asdef.animdefs = &animdefs_enemy1;
        e->asdef.pal = SPAL_ENEMY1;
    }
    else if (animidx == 1) {
        e->asdef.animdefs = &animdefs_enemy2;
        e->asdef.pal = SPAL_ENEMY2;
    }
    else if (animidx == 2) {
        e->asdef.animdefs = &animdefs_enemy3;
        e->asdef.pal = SPAL_ENEMY3;
    }
    e->asdef.anim = 0xff;
    anisprite_set_anim(&e->asdef, 0);

    e->ymin = e->asdef.y + 8 * ((u16)odef->extra2);
    if (e->ymin > 10000) e->ymin = 0;
    e->ymax = e->asdef.y + 8 * ((u16)odef->extra3);
    if (e->advmask < 8)
        e->deltay = 1;
    else {
        e->deltay = -1;
        e->advmask = 16 - e->advmask; // 4 bit sign extension
    }
    e->fctr = e->advmask;
}

static void enemy_diag_setup(ObjectDef* odef)
{
    if (enemy_diag_count >= ENEMY_DIAG_MAX)
        return;

    EnemyDiag* e = &enemy_diag_arr[enemy_diag_count++];
    e->asdef.slot = ss_object++;
    e->asdef.x = 8 * ((u16)odef->ini_cx);
    e->asdef.y = 8 * ((u16)odef->ini_cy);
    u8 animidx = odef->extra1;
    if (animidx == 0) {
        e->asdef.animdefs = &animdefs_enemy1;
        e->asdef.pal = SPAL_ENEMY1;
    }
    else if (animidx == 1) {
        e->asdef.animdefs = &animdefs_enemy2;
        e->asdef.pal = SPAL_ENEMY2;
    }
    else if (animidx == 2) {
        e->asdef.animdefs = &animdefs_enemy3;
        e->asdef.pal = SPAL_ENEMY3;
    }
    e->asdef.anim = 0xff;
    anisprite_set_anim(&e->asdef, 0);

    if (odef->extra2 > 0) { // advx
        e->fperx = odef->extra2;
        e->deltax = 1;
    }
    else {
        e->fperx = -odef->extra2;
        e->deltax = -1;
    }
    e->fctrx = e->fperx;

    if (odef->extra3 > 0) { // advy
        e->fpery = odef->extra3;
        e->deltay = 1;
    }
    else {
        e->fpery = -odef->extra3;
        e->deltay = -1;
    }
    e->fctry = e->fpery;
}

static void enemy_spikes_setup(ObjectDef* odef)
{
    if (enemy_spikes_count >= ENEMY_SPIKES_MAX)
        return;

    EnemySpikes* e = &enemy_spikes_arr[enemy_spikes_count++];
    e->asdef.slot = ss_object++;
    e->asdef.x = 8 * ((u16)odef->ini_cx);
    e->asdef.y = 8 * ((u16)odef->ini_cy);
    e->asdef.animdefs = &animdefs_spikes;
    e->asdef.pal = SPAL_SPIKES;
    e->asdef.anim = 0xff;
    anisprite_set_anim(&e->asdef, ENEMY_SPIKES_LEAVE);

    e->active = false;
    e->fctr = 0;
}

static void object_lever_setup(ObjectDef* odef)
{
    if (object_lever_count >= OBJECT_LEVER_MAX)
        return;

    ObjectLever* o = &object_lever_arr[object_lever_count++];
    o->asdef.slot = ss_object++;

    o->asdef.x = 8 * ((u16)odef->ini_cx);
    o->asdef.y = 8 * ((u16)odef->ini_cy);
    o->asdef.animdefs = &animdefs_lever;
    if (odef->extra2)
        o->asdef.mirrot = ATTR_X___;
    else
        o->asdef.mirrot = ATTR_____;
    o->asdef.pal = SPAL_LEVER;
    o->asdef.anim = 0xff;
    o->closed = (bool)odef->extra1;
    o->touch = false;
    if (o->closed)
        anisprite_set_anim(&o->asdef, OBJECT_LEVER_CLOSE);
    else
        anisprite_set_anim(&o->asdef, OBJECT_LEVER_OPEN);
}

static void object_door_setup(ObjectDef* odef)
{
    if (object_door_count >= OBJECT_DOOR_MAX)
        return;

    ObjectDoor* o = &object_door_arr[object_door_count++];
    o->asdef.slot = ss_object++;

    o->asdef.x = 8 * ((u16)odef->ini_cx);
    o->asdef.y = 8 * ((u16)odef->ini_cy);
    o->asdef.animdefs = &animdefs_door;
    o->asdef.pal = SPAL_DOOR;
    o->asdef.anim = 0xff;
    o->closed = (bool)odef->extra1;
    object_door_closure_update(o);
}

static void object_conv_setup(ObjectDef* odef)
{
    if (object_conv_count >= OBJECT_CONV_MAX)
        return;

    ObjectConv* o = &object_conv_arr[object_conv_count++];
    o->cx1 = odef->ini_cx;
    o->cy  = odef->ini_cy;
    o->cx2 = odef->extra1;
    o->left = odef->extra2;
    o->idx = 0;

    // u8 x=36, y=0;
    // print_set_pos(x, y++);
    // print_str("CONV");
}

void objects_update()
{
    for (u8 iobj = 0; iobj < enemy_horiz_count; iobj++) {
        EnemyHoriz* e = &enemy_horiz_arr[iobj];
        enemy_horiz_update(e);
    }
    for (u8 iobj = 0; iobj < enemy_vert_count; iobj++) {
        EnemyVert* e = &enemy_vert_arr[iobj];
        enemy_vert_update(e);
    }
    for (u8 iobj = 0; iobj < enemy_diag_count; iobj++) {
        EnemyDiag* e = &enemy_diag_arr[iobj];
        enemy_diag_update(e);
    }
    for (u8 iobj = 0; iobj < enemy_spikes_count; iobj++) {
        EnemySpikes* e = &enemy_spikes_arr[iobj];
        enemy_spikes_update(e);
    }
    for (u8 iobj = 0; iobj < object_lever_count; iobj++) {
        ObjectLever* o = &object_lever_arr[iobj];
        object_lever_update(o);
    }
    for (u8 iobj = 0; iobj < object_door_count; iobj++) {
        ObjectDoor* o = &object_door_arr[iobj];
        object_door_update(o);
    }
    for (u8 iobj = 0; iobj < object_conv_count; iobj++) {
        ObjectConv* o = &object_conv_arr[iobj];
        object_conv_update(o);
    }
}

static void enemy_horiz_update(EnemyHoriz* e)
{
    anisprite_update(&e->asdef);   

    if (--e->fctr > 0) return;
    e->fctr = e->advmask;

    e->asdef.x += e->deltax;
    if (e->asdef.x >= e->xmax)
        e->deltax = -1;
    if (e->asdef.x <= e->xmin)
        e->deltax = 1;

    u8 cx = e->asdef.x >> 3;
    u8 cy = e->asdef.y >> 3;
    u8 dx = e->asdef.x & 7;

    #define RMASK (TMA|TMG)
    if (dx == 0) {
        if (e->deltax > 0) {
            u8* src = collmap + cx + 2 + 40 * (cy);
            u8 A = *src;
            src += 40;
            u8 B = *src;
            if (A & RMASK || B & RMASK)
                e->deltax = -1;
        }
        else {
            u8* src = collmap + cx - 1 + 40 * (cy);
            u8 A = *src;
            src += 40;
            u8 B = *src;
            if (A & RMASK || B & RMASK)
                e->deltax = 1;
        }
    }
}

static void enemy_vert_update(EnemyVert* e)
{
    anisprite_update(&e->asdef);   

    if (--e->fctr > 0) return;
    e->fctr = e->advmask;

    e->asdef.y += e->deltay;
    if (e->asdef.y >= e->ymax)
        e->deltay = -1;
    if (e->asdef.y <= e->ymin)
        e->deltay = 1;
}

static void enemy_diag_update(EnemyDiag* e)
{
    anisprite_update(&e->asdef);

    bool processX = true;
    bool processY = true;

    e->fctrx--;
    if (e->fctrx > 0) processX = false;
    else e->fctrx = e->fperx;

    e->fctry--;
    if (e->fctry > 0) processY = false;
    else e->fctry = e->fpery;

    if (!processX && !processY)
        return;

    u8 cx = e->asdef.x >> 3;
    u8 cy = e->asdef.y >> 3;
    u8 dx = e->asdef.x & 7;
    u8 dy = e->asdef.y & 7;

    #define RMASK (TMA|TMG)
    if (processX) {
        if (dx == 0) {
            if (e->deltax > 0) {
                u8* src = collmap + cx + 2 + 40 * (cy);
                u8 A = *src;
                src += 40;
                u8 B = *src;
                if (A & RMASK || B & RMASK) {
                    e->deltax = -1;
                    e->asdef.x -= e->deltax;
                }
            }
            else {
                u8* src = collmap + cx - 1 + 40 * (cy);
                u8 A = *src;
                src += 40;
                u8 B = *src;
                if (A & RMASK || B & RMASK) {
                    e->deltax = 1;
                    e->asdef.x -= e->deltax;
                }
            }
        }
        e->asdef.x += e->deltax;
    }

    #define DMASK (TMA|TMG|TMD)
    if (processY) {
        if (dy == 0) {
            if (e->deltay > 0) {
                u8* src = collmap + cx + 40 * (cy + 2);
                u8 A = *src;
                src++;
                u8 B = *src;
                if (A & DMASK || B & DMASK) {
                    e->deltay = -1;
                    e->asdef.y -= e->deltay;
                }
            }
            else {
                u8* src = collmap + cx + 40 * (cy - 1);
                u8 A = *src;
                src++;
                u8 B = *src;
                if (A & RMASK || B & RMASK) {
                    e->deltay = 1;
                    e->asdef.y -= e->deltay;
                }
            }
        }
        e->asdef.y += e->deltay;
    }

}

static void enemy_spikes_update(EnemySpikes* e)
{
    anisprite_update(&e->asdef);   

    if (!e->active) {
        if (e->fctr >= ENEMY_SPIKES_FR_OFF) {
            anisprite_set_anim(&e->asdef, ENEMY_SPIKES_ENTER);
            e->active = true;
            e->fctr = 0;
        }
        else
            e->fctr++;
    }
    else {
        if (e->fctr >= ENEMY_SPIKES_FR_ON) {
            anisprite_set_anim(&e->asdef, ENEMY_SPIKES_LEAVE);
            e->active = false;
            e->fctr = 0;
        }
        else
            e->fctr++;
    }
}

static void object_lever_update(ObjectLever* o)
{
    anisprite_update(&o->asdef);
}

static void object_door_update(ObjectDoor* o)
{
    anisprite_update(&o->asdef);
}

u8 enemy_get_count()
{
    return enemy_horiz_count + enemy_vert_count + enemy_diag_count +
        enemy_spikes_count + object_door_count;
}

bool enemy_get_rect(u8 index, CollRect* r)
{
    if (index < enemy_horiz_count) {
        EnemyHoriz* e = &enemy_horiz_arr[index];
        r->x0 = e->asdef.x + 1;
        r->x1 = e->asdef.x + 14;
        r->y0 = e->asdef.y + 1;
        r->y1 = e->asdef.y + 14;
        return true;
    }
    index -= enemy_horiz_count;

    if (index < enemy_vert_count) {
        EnemyVert* e = &enemy_vert_arr[index];
        r->x0 = e->asdef.x + 1;
        r->x1 = e->asdef.x + 14;
        r->y0 = e->asdef.y + 1;
        r->y1 = e->asdef.y + 14;
        return true;
    }
    index -= enemy_vert_count;

    if (index < enemy_diag_count) {
        EnemyDiag* e = &enemy_diag_arr[index];
        r->x0 = e->asdef.x + 1;
        r->x1 = e->asdef.x + 14;
        r->y0 = e->asdef.y + 1;
        r->y1 = e->asdef.y + 14;
        return true;
    }
    index -= enemy_diag_count;

    if (index < enemy_spikes_count) {
        EnemySpikes* e = &enemy_spikes_arr[index];
        if (!e->active) return false;
        r->x0 = e->asdef.x + 1;
        r->x1 = e->asdef.x + 14;
        r->y0 = e->asdef.y + 1;
        r->y1 = e->asdef.y + 14;
        return true;
    }
    index -= enemy_spikes_count;

    if (index < object_door_count) {
        ObjectDoor* o = &object_door_arr[index];
        if (!o->closed) return false;
        r->x0 = o->asdef.x + 1;
        r->x1 = o->asdef.x + 14;
        r->y0 = o->asdef.y + 1;
        r->y1 = o->asdef.y + 14;
        return true;
    }

    return false;
}

u8 lever_get_count()
{
    return object_lever_count;
}

ObjectLever* lever_get(u8 index)
{
    return &object_lever_arr[index];
}

bool lever_get_rect(u8 index, CollRect* r)
{
    if (index < object_lever_count) {
        ObjectLever* e = &object_lever_arr[index];
        r->x0 = e->asdef.x + 1;
        r->x1 = e->asdef.x + 14;
        r->y0 = e->asdef.y + 1;
        r->y1 = e->asdef.y + 14;
        return true;
    }
    return false;
}

static void object_door_closure_toggle(ObjectDoor* o)
{
    o->closed = !o->closed;
    object_door_closure_update(o);
}

static void object_door_closure_update(ObjectDoor* o)
{
    if (o->closed)
        anisprite_set_anim(&o->asdef, OBJECT_DOOR_CLOSE);
    else
        anisprite_set_anim(&o->asdef, OBJECT_DOOR_OPEN);

    u8 cx = o->asdef.x >> 3;
    u8 cy = o->asdef.y >> 3;

    u8* ptr = collmap;
    ptr += 40 * cy;
    ptr += cx;

    if (o->closed) *ptr |= TMG;
    else           *ptr &= ~TMG;

    ptr++;

    if (o->closed) *ptr |= TMG;
    else           *ptr &= ~TMG;

    ptr += 39;

    if (o->closed) *ptr |= TMG;
    else           *ptr &= ~TMG;

    ptr++;

    if (o->closed) *ptr |= TMG;
    else           *ptr &= ~TMG;
}

static void object_lever_closure_toggle(ObjectLever* o)
{
    o->closed = !o->closed;
    object_lever_closure_update(o);
}

extern u8 collmap[];

static void object_lever_closure_update(ObjectLever* o)
{
    if (o->closed)
        anisprite_set_anim(&o->asdef, OBJECT_LEVER_CLOSE);
    else
        anisprite_set_anim(&o->asdef, OBJECT_LEVER_OPEN);
}

void lever_move()
{
    for (u8 iobj = 0; iobj < object_lever_count; iobj++) {
        ObjectLever* lever = &object_lever_arr[iobj];
        object_lever_closure_toggle(lever);
    }
    for (u8 iobj = 0; iobj < object_door_count; iobj++) {
        ObjectDoor* door = &object_door_arr[iobj];
        object_door_closure_toggle(door);
    }
}

static void object_conv_update(ObjectConv* o)
{
    u8* ptr = 0x4000;
    ptr += 80 * o->cy;
    ptr += 2 * o->cx1;
    u8 cnt = 1 + o->cx2 - o->cx1;
    u8 oidx = o->idx << 4;
    while (cnt--) {
        u8 val = *ptr;
        val &= 0x0F;
        val |= oidx;
        *ptr = val;
        ptr+= 2;
    }
    if (o->left) {
        o->idx--;
        if (o->idx > 3) o->idx = 3; // unsigned logic
    }
    else {
        o->idx++;
        if (o->idx > 3) o->idx = 0;
    }
}