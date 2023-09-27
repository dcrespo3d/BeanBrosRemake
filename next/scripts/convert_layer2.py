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

# http://omz-software.com/pythonista/docs/ios/PIL.html

from PIL import Image
from palette_util import *

ifns = [
    '../art/title-bean.png',
    '../art/title-brothers.png',
]

ofns = [
    '../res/title-bean.bin',
    '../res/title-brothers.bin',
]

def convert_layer2():
    igrps = [
        [
            '../art/title-bean.png',
            '../art/title-brothers.png',
        ],
    ]

    ogrps = [
        {
            'palfn'  : '../res/layer2_menu_imgs_pal.bin',
            'bmpfns' : [
                '../res/title-bean.bin',
                '../res/title-brothers.bin',
            ] 
        }
    ]
    convert_layer2_image_groups(igrps, ogrps)

def convert_layer2_image_groups(igrps, ogrps):
    if len(igrps) != len(ogrps):
        raise Exception('igrps has not same len as ogrps')
    
    for i in range(len(igrps)):
        igrp = igrps[i]
        ogrp = ogrps[i]
        opalfn = ogrp['palfn']
        obmpfns = ogrp['bmpfns']
        convert_layer2_image_group(igrp, opalfn, obmpfns)

def convert_layer2_image_group(ifns, opalfn, obmpfns):
    print('converting image group')
    # print(ifns)
    # print(opalfn)
    # print(obmpfns)
    rgb333imgs = []
    for ifn in ifns:
        img = Image.open(ifn)
        w = img.size[0]
        h = img.size[1]
        img = img.convert('RGB')
        print('read {}: {}x{} {}'.format(ifn, w, h, img.mode))
    
        rgb333img = rgb333img_for_rgb_img(img)
        rgb333imgs.append(rgb333img)

    grp_pal = extract_palette_for_rgb333_images(rgb333imgs)
    print('group palette: ', len(grp_pal), grp_pal)

    indeximgs = []
    for rgb333img in rgb333imgs:
        indeximg = indeximg_for_rgb333img_and_pal(rgb333img, grp_pal)
        indeximgs.append(indeximg)

    cmpimgs = []
    for indeximg in indeximgs:
        cmpimg = rlecmp_for_int8img(indeximg)
        cmpimgs.append(cmpimg)

    for i in range(len(cmpimgs)):
        cmpimg = cmpimgs[i]
        ofn = ofns[i]
        f = open(ofn, 'wb')
        f.write(bytes(cmpimg))
        f.close()
        print('written ' + ofn)

    int8pal = int8pal_for_rgb333pal(grp_pal)
    f = open(opalfn, 'wb')
    f.write(bytes(int8pal))
    f.close()
    print('written ' + opalfn)

    for i in range(1,8):
        darkfn = opalfn.replace('.bin', '_' + str(i) + '.bin')
        darkpal = darkened_rgb333pal(grp_pal, i)
        # print(darkpal)
        int8pal = int8pal_for_rgb333pal(darkpal)
        f = open(darkfn, 'wb')
        f.write(bytes(int8pal))
        f.close()
        print('written ' + darkfn + ' - darkened by ' + str(i))

def extract_palette_for_rgb333_images(rgb333imgs):
    palset = set()
    for rgb333img in rgb333imgs:
        for rgb333 in rgb333img:
            palset.add(rgb333)
    palarr = list(palset)
    palarr.sort()
    return palarr

def indeximg_for_rgb333img_and_pal(rgb333img, pal):
    paltbl = {}
    for idx in range(len(pal)):
        palentry = pal[idx]
        paltbl[palentry] = idx

    indeximg = []
    for rgb333 in rgb333img:
        indexpix = paltbl[rgb333]
        indeximg.append(indexpix)

    return indeximg 


def jarl():
    for i in range(len(ifns)):
        ifn = ifns[i]
        ofn = ofns[i]
        convert_l2_image(ifn, ofn)

def convert_l2_image(ifn, ofn):
    img = Image.open(ifn)
    w = img.size[0]
    h = img.size[1]
    img = img.convert('RGB')
    print('{}: {}x{} {}'.format(ifn, w, h, img.mode))

    rgb333img = rgb333img_for_rgb_img(img)
    rgb332img = rgb332img_for_rgb333img(rgb333img)

    cmpimg = rlecmp_for_int8img(rgb332img)

    f = open(ofn, 'wb')
    f.write(bytes(cmpimg))
    f.close()

def rgb333img_for_rgb_img(img):
    w, h = img.size
    rgb333img = []
    for y in range(h):
        for x in range(w):
            pixel = img.getpixel((x,y))
            r, g, b = pixel
            pixel = r, g, b
            s = ''
            for c in pixel:
                s += str(c >> 5)
            # print(s, pixel)
            rgb333img.append(s)
    return rgb333img

def rgb332img_for_rgb333img(rgb333img):
    rgb332img = []
    for rgb333 in rgb333img:
        r = int(rgb333[0])
        g = int(rgb333[1])
        b = int(rgb333[2])
        rgb332 = (r << 5) | (g << 2) | (b >> 1)
        rgb332img.append(rgb332)
    return rgb332img

def rlecmp_for_int8img(rgb332img):
    prev = -1
    cnt = 0
    cmp = []
    for pix in rgb332img:
        if prev == pix:
            cnt += 1
        else:
            if cnt != 0:
                cmp.append(cnt)
                cmp.append(prev)
            cnt = 1
            prev = pix
    if cnt != 0:
        cmp.append(cnt)
        cmp.append(prev)
    # print(len(cmp), cmp)

    for i in range(len(cmp)):
        cmp[i] = int(cmp[i])
    cmp.append(0)

    return cmp
    

if __name__ == '__main__':
    convert_layer2()