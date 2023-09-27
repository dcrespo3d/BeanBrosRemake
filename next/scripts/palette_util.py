###############################################################################
#
#  Bean Brothers (remake) - for ZX Spectrum Next / N-Go
#
#  (c) 2023 David Crespo - https://github.com/dcrespo3d
#                          https://davidprograma.itch.io
#                          https://www.youtube.com/@Davidprograma
#
#  Based on Bean Brothers for ZX Spectrum 48/128K - (c) 2018 Dave Hughes
#
###############################################################################
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.  
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY# without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
# 
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <https://www.gnu.org/licenses/>. 
#
###############################################################################

def int8pal_for_rgb333pal(rgb333pal):
    int8pal = []
    for rgb333color in rgb333pal:
        r = int(rgb333color[0])
        g = int(rgb333color[1])
        b = int(rgb333color[2])
        by1 = r << 5 | g << 2 | b >> 1
        by2 = b & 1
        int8pal.append(by1)
        int8pal.append(by2)
    return int8pal

def darkened_rgb333pal(rgb333pal, amount):
    respal = []
    for rgb333color in rgb333pal:
        r = int(rgb333color[0])
        g = int(rgb333color[1])
        b = int(rgb333color[2])
        r -= amount
        g -= amount
        b -= amount
        if r < 0: r = 0
        if g < 0: g = 0
        if b < 0: b = 0
        rescolor = str(r) + str(g) + str(b)
        respal.append(rescolor)
    return respal