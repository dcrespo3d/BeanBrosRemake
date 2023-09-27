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

ifn = '../art/tileset_palgroup.txt'
ofn = '../res/tileset_palgroup.bin'

def convert_palgroup():
    print('*' * 60)
    print('**  Converting Palette Groups for TileSet')
    print('*' * 60)

    f = open(ifn, 'rt')
    palgroup_txt = f.read()
    f.close()

    palgroup_txt = palgroup_txt.replace('\n', '').rstrip(',')
    # print(len(palgroup_txt))


    palgroup_arr = []
    for ch in palgroup_txt:
        palgroup_arr.append(int(ch) << 4)
    print(palgroup_arr)

    f = open(ofn, 'wb')
    f.write(bytes(palgroup_arr))
    f.close()

if __name__ == '__main__':
    convert_palgroup()