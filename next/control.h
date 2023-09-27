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

#ifndef __control_h__
#define __control_h__

#include "types.h"

extern bool actnUp;
extern bool actnDown;
extern bool actnLeft;
extern bool actnRight;
extern bool actnSelect;

extern bool prevUp;
extern bool prevDown;
extern bool prevLeft;
extern bool prevRight;
extern bool prevSelect;

extern u16 keycodeUp;
extern u16 keycodeDown;
extern u16 keycodeLeft;
extern u16 keycodeRight;
extern u16 keycodeSelect;

extern u8 joycodeUp;
extern u8 joycodeDown;
extern u8 joycodeLeft;
extern u8 joycodeRight;
extern u8 joycodeSelect;

void control_init();
void control_update();

#endif