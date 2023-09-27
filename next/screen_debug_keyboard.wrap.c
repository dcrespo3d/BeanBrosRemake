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

#include "screen_debug_keyboard.h"
#include "paging.h"

extern void sdk_enter_pg46();
extern void sdk_update_pg46();

void sdk_enter()
{
    storePrevPagesAtSlots0and1();
    putPagesAtSlots0and1(46);
    sdk_enter_pg46();
    restorePrevPagesAtSlots0and1();
}

void sdk_update()
{
    storePrevPagesAtSlots0and1();
    putPagesAtSlots0and1(46);
    sdk_update_pg46();
    restorePrevPagesAtSlots0and1();
}
