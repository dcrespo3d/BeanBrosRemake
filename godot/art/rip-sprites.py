# https://pillow.readthedocs.io/en/stable/reference/index.html

from PIL import Image

ifn = 'beanbros.sna'
ofn = 'beanbros-sprites.png'
f = open(ifn, 'rb')
barr = f.read()
f.close()
print('read', ifn)

iarr = []
for b in barr:
    iarr.append(int(b))

print(len(iarr), 'bytes read')
iarr = iarr[27:]
print('trimming to', len(iarr))

w = 1040
h = 64
img = Image.new('L', (w, h))

trim = 120+64 + 64*319
iarr = iarr[trim:]

cols = 16
rows = h

x0 = 0
x = 0
y = 0
def b8(i):
    b8 = bin(i).replace('0b','')
    while len(b8) < 8:
        b8 = '0' + b8
    return b8


try:
    for i in iarr:
        if x >= cols:
            x = 0
            y += 1
        if y >= rows:
            y = 0
            x0 += cols
        bstr = b8(i)
        for bit in bstr:
            val = int(bit) * 255
            img.putpixel((x0+x,y),val)
            x += 1
except:
    pass


img.save(ofn)
