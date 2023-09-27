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

def levmap_cvt(ifn, ofn):
    return levmap_cvt_v2(ifn, ofn)

def levmap_cvt_v1(ifn, ofn):
    f = open(ifn, 'rt')
    tilemap_txt = f.read()
    f.close()

    tilemap_txt = tilemap_txt.replace('\n', '').rstrip(',')
    tilemap_arr = tilemap_txt.split(',')

    prev = -1
    cnt = 0
    cmp = []
    for tile in tilemap_arr:
        if prev == tile:
            cnt += 1
        else:
            if cnt != 0:
                cmp.append(cnt)
                cmp.append(prev)
            cnt = 1
            prev = tile
    if cnt != 0:
        cmp.append(cnt)
        cmp.append(prev)
    # print(len(cmp), cmp)

    for i in range(len(cmp)):
        cmp[i] = int(cmp[i])

    f = open(ofn, 'wb')
    f.write(bytes(cmp))
    f.close()

    return len(cmp)

def levmap_cvt_v2(ifn, ofn):
    f = open(ifn, 'rt')
    tilemap_txt = f.read()
    f.close()

    tilemap_txt = tilemap_txt.replace('\n', '').rstrip(',')
    tilemap_arr = tilemap_txt.split(',')

    prev = -1
    cnt = 0
    cmp = []
    for tile in tilemap_arr:
        if prev == tile:
            cnt += 1
        else:
            if cnt != 0:
                cmp.append((cnt,int(prev)))
            cnt = 1
            prev = tile
    if cnt != 0:
        cmp.append((cnt,int(prev)))
    # print(len(cmp), cmp)

    cmp2 = []
    for pair in cmp:
        cnt, val = pair
        while cnt > 120:
            cmp2.append((120, val))
            cnt -= 120
        cmp2.append((cnt, val))
    # print(len(cmp2), cmp2)

    cmp3 = []
    for pair in cmp2:
        cnt, val = pair
        if 1 == cnt:
            cmp3.append(val)
        elif 2 == cnt:
            cmp3.append(val)
            cmp3.append(val)
        else: 
            cmp3.append(128 | cnt)
            cmp3.append(val)
    # print(cmp3)

    for i in range(len(cmp3)):
        cmp3[i] = int(cmp3[i])

    f = open(ofn, 'wb')
    f.write(bytes(cmp3))
    f.close()

    return len(cmp3)

if __name__ == '__main__':
    ifn = '../levels/levmap_01.txt'
    ofn = '../res/tilemap_cmp.bin'
    sz = levmap_cvt(ifn, ofn)
    print(sz)
    