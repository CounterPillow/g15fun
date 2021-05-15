#!/usr/bin/env python
from PIL import Image

import sys

if len(sys.argv) != 2:
    sys.exit(1)

im_data = []

with Image.open(sys.argv[1]) as im:
    for row in range(0, 6):
        for col in range(0, 160):
            val = 0;
            for i in range(0,8):
                try:
                    px = im.getpixel((col, row * 8 + i))
                except:
                    px = 0
                val = val + ((1 if px > 0 else 0) << i)
            im_data.append(val)

s = "#include <stdint.h>\n\n"
s += "uint8_t img[{}]".format(len(im_data)) + " = {"
for d in im_data[:-1]:
    s += str(d) + ","
s += str(im_data[-1]) + "};"
print(s)
