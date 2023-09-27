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

def levdef_cvt(ifn, ofn):
    f = open(ifn, 'rt')
    levdef_txt = f.read()
    f.close()

    levdef_arr = []
    for line in levdef_txt.splitlines():
        line = line.lstrip()
        if line.startswith(('{','}')): continue
        toks = line.split(',')
        val = int(toks[0])
        while val < 0: val += 256
        # print(toks[0], val)
        levdef_arr.append(val)

    f = open(ofn, 'wb')
    f.write(bytes(levdef_arr))
    f.close()

    return len(levdef_arr)

if __name__ == '__main__':
    ifn = '../levels/levdef_01.txt'
    ofn = '../res/levdef_01.bin'
    levdef_cvt(ifn, ofn)