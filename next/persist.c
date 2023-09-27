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

#include "persist.h"
#include "screen_game.h"
#include "control.h"
#include <string.h>
#include <arch/zxn/esxdos.h>
#include <errno.h>
#include "paging.h"

#define LEVDATA_FN "beanbros.lev"
#define LEVDATA_SZ 16
static ubyte levdata[LEVDATA_SZ];

void persist_load_levdata()
{
    storePrevPagesAtSlots0and1();
    putRomPagesAtSlots0and1();

    memset(levdata, ' ', 16);
    levdata[0] = DEF_FUL_ORI + 'A';
    levdata[1] = DEF_FUL_EXT + 'a' - 20;

    errno = 0;
    ubyte filehandle = esxdos_f_open(LEVDATA_FN,
        ESXDOS_MODE_R | ESXDOS_MODE_OE);
    if (!errno) {
        esxdos_f_read(filehandle, levdata, LEVDATA_SZ);
        esxdos_f_close(filehandle);
    }
    sg_ful_ori = levdata[0] - 'A';
    sg_ful_ext = levdata[1] - 'a' + 20;

    restorePrevPagesAtSlots0and1();
}

void persist_save_levdata()
{
    storePrevPagesAtSlots0and1();
    putRomPagesAtSlots0and1();

    memset(levdata, 32, 16);
    levdata[0] = sg_ful_ori + 'A';
    levdata[1] = sg_ful_ext + 'a' - 20;

    errno = 0;
    ubyte filehandle = esxdos_f_open(LEVDATA_FN,
        ESXDOS_MODE_W | ESXDOS_MODE_CT);
    if (!errno) {
        esxdos_f_write(filehandle, levdata, LEVDATA_SZ);
        esxdos_f_close(filehandle);
    }

    restorePrevPagesAtSlots0and1();
}

#define MOOT_FN "6031769" // back to 1983!
bool moot = false;

void persist_check_moot()
{
    storePrevPagesAtSlots0and1();
    putRomPagesAtSlots0and1();

    moot = false;
    errno = 0;
    ubyte filehandle = esxdos_f_open(MOOT_FN,
        ESXDOS_MODE_R | ESXDOS_MODE_OE);
    if (!errno) {
        moot = true;
    }

    restorePrevPagesAtSlots0and1();
}

#define CNTDATA_FN "beanbros.cnt"
#define CNTDATA_SZ 16
static ubyte cntdata[CNTDATA_SZ];

static void serialize_cntdata()
{
    u16* wptr = (u16*)cntdata;
    *wptr++ = keycodeUp;
    *wptr++ = keycodeDown;
    *wptr++ = keycodeLeft;
    *wptr++ = keycodeRight;
    *wptr++ = keycodeSelect;
    u8* bptr = (u8*)wptr;
    *bptr++ = joycodeUp;
    *bptr++ = joycodeDown;
    *bptr++ = joycodeLeft;
    *bptr++ = joycodeRight;
    *bptr++ = joycodeSelect;
    *bptr++ = 0;
}

static void deserialize_cntdata()
{
    u16* wptr = (u16*)cntdata;
    keycodeUp     = *wptr++;
    keycodeDown   = *wptr++;
    keycodeLeft   = *wptr++;
    keycodeRight  = *wptr++;
    keycodeSelect = *wptr++;
    u8* bptr = (u8*)wptr;
    joycodeUp     = *bptr++;
    joycodeDown   = *bptr++;
    joycodeLeft   = *bptr++;
    joycodeRight  = *bptr++;
    joycodeSelect = *bptr++;
}

void persist_load_control()
{
    storePrevPagesAtSlots0and1();
    putRomPagesAtSlots0and1();
    
    serialize_cntdata();

    errno = 0;
    ubyte filehandle = esxdos_f_open(CNTDATA_FN,
        ESXDOS_MODE_R | ESXDOS_MODE_OE);
    if (!errno) {
        esxdos_f_read(filehandle, cntdata, CNTDATA_SZ);
        esxdos_f_close(filehandle);
    }

    deserialize_cntdata();

    restorePrevPagesAtSlots0and1();
}

void persist_save_control()
{
    storePrevPagesAtSlots0and1();
    putRomPagesAtSlots0and1();

    serialize_cntdata();

    errno = 0;
    ubyte filehandle = esxdos_f_open(CNTDATA_FN,
        ESXDOS_MODE_W | ESXDOS_MODE_CT);
    if (!errno) {
        esxdos_f_write(filehandle, cntdata, CNTDATA_SZ);
        esxdos_f_close(filehandle);
    }

    restorePrevPagesAtSlots0and1();
}