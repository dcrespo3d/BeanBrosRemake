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
#include "print_tile.h"
#include <arch/zxn.h>
#include <string.h>

void __CALLEE__ sprite_update(SpriteDef* sprdef)
{
    ZXN_WRITE_REG(0x34, sprdef->slot);
    ZXN_WRITE_REG(0x35, sprdef->x);
    ZXN_WRITE_REG(0x36, sprdef->y);
    u8 val37 = JOINIB(sprdef->pal, sprdef->mirrot);
    val37 |= (sprdef->x >> 8) & 1;
    ZXN_WRITE_REG(0x37, val37);
    ZXN_WRITE_REG(0x38, 0xC0 | (sprdef->pat >> 1));
    u8 val39 = 0x80 | ((sprdef->pat & 1) << 6);
    val39 |= (sprdef->y >> 8) & 1;
    val39 |= sprdef->scale;
    ZXN_WRITE_REG(0x39, val39);
}

void __CALLEE__ sprite_text_print(SpriteDef* sdef, const char* msg, bool centerX)
{
    u16 msglen = strlen(msg);
    if (centerX)
        sdef->x = (320 - (msglen * 12)) >> 1;

    for (u8 i = 0; i < msglen; i++) {
        sdef->slot++;
        sdef->pat = 32 + msg[i];
        sprite_update(sdef);
        sdef->x += 12;
    }
}

#include <string.h>

void string_append_dec_byte(char* str, u8 val)
{
    u16 slen = strlen(str);
    char* ptr = str + slen;
    u8 cen = 0;
    u8 dec = 0;
    while (val >= 100) {
        val -= 100;
        cen++;
    }
    while (val >= 10) {
        val -= 10;
        dec++;
    }
    if (cen) *ptr++ = '0' + cen;
    if (dec) *ptr++ = '0' + dec;
    *ptr++ = '0' + val;
    *ptr == '\0';
}

void string_append_dec_word(char* str, u16 val)
{
    u16 slen = strlen(str);
    char* ptr = str + slen;
    u8 dmi = 0;
    u8 mil = 0;
    u8 cen = 0;
    u8 dec = 0;
    while (val >= 10000) {
        val -= 10000;
        dmi++;
    }
    while (val >= 1000) {
        val -= 1000;
        mil++;
    }
    while (val >= 100) {
        val -= 100;
        cen++;
    }
    while (val >= 10) {
        val -= 10;
        dec++;
    }
    if (dmi) *ptr++ = '0' + dmi;
    if (mil) *ptr++ = '0' + mil;
    if (cen) *ptr++ = '0' + cen;
    if (dec) *ptr++ = '0' + dec;
    *ptr++ = '0' + val;
    *ptr == '\0';
}


// void sprite_debug_visible()
// {
//     u8 y = 0;
//     u8 x = 36;
//     for (u8 i = 0; i < 128; i++)
//     {
//         ZXN_WRITE_REG(0x34, i);
//         u8 v38 = ZXN_READ_REG(0x38);
//         if (v38 & 0x80) {
//             print_set_pos(x, y++);
//             print_char('S');
//             print_hex_byte(i);
//         }

//     }
//     print_set_pos(x, y++);
//     print_str("----");
//     print_set_pos(x, y++);
//     print_str("----");
//     print_set_pos(x, y++);
//     print_str("----");
//     print_set_pos(x, y++);
//     print_str("----");
// }

