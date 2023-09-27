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

ifn = '../art/tileset_colltable.txt'
ofn = '../res/tileset_colltable.bin'

def convert_colltable():
    print('*' * 60)
    print('**  Converting Collision Table for Tileset')
    print('*' * 60)

    f = open(ifn, 'rt')
    colltable_txt = f.read()
    f.close()

    colltable_txt = colltable_txt.replace('\n', '').rstrip(',')

    transtable = {'A':1,'B':3,'C':7,'D':15,'E':32,'F':64,'H':129}

    collarr = []
    for ch in colltable_txt:
        val = 0
        if ch in transtable: val = transtable[ch]
        collarr.append(val)
    print(collarr)

    f = open(ofn, 'wb')
    f.write(bytes(collarr))
    f.close()

if __name__ == '__main__':
    convert_colltable()
