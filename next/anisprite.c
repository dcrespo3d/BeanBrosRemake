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

#include <arch/zxn.h>
#include "anisprite.h"
#include "sprite_manager.h"

static u16 frame = 0;

void anisprite_tick()
{
    frame++;
}

void __CALLEE__ anisprite_set_anim(AniSprite* asdef, u8 anim)
{
    ASAnim* animdef = asdef->animdefs + anim;
    if (asdef->anim == anim)
        return;
    asdef->anim = anim;
    asdef->pat = animdef->start;
    asdef->frame = frame;
}

void __CALLEE__ anisprite_update(AniSprite* asdef)
{
    sprite_update((SpriteDef*) asdef);

    ASAnim* animdef = asdef->animdefs + asdef->anim;
    if (animdef->dur == 0)
        return;

    if (asdef->frame + animdef->dur > frame)
        return;

    asdef->frame = frame;

    if (animdef->end > animdef->start) {
        if (asdef->pat < animdef->end)
            asdef->pat++;
        else if (animdef->loop) 
            asdef->pat = animdef->start;
    }
    else {
        if (asdef->pat > animdef->end)
            asdef->pat--;
        else if (animdef->loop) 
            asdef->pat = animdef->start;
    }
}
