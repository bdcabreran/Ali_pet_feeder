import cv2
import numpy as np

from PIL import Image
import struct, os, sys

def write_bin(f, pixel_list,img_name,img_size):
    f.write('/*'+str(img_size)+ '*/'+'\n')
    f.write('const uint16_t '+ img_name + '[]= {' )
    for pix in pixel_list:
        r = (pix[0] >> 3) & 0x1F
        g = (pix[1] >> 2) & 0x3F
        b = (pix[2] >> 3) & 0x1F
        f.write(( str(hex((r << 11) + (g << 5) + b)) )+ (','))
    f.write('}')


icon_folder = 'icons/'
files  = os.listdir(icon_folder);
for file in files :
    in_path = file
    img_name  = in_path.rsplit('.', 1)[0]
    out_path ='out_icon/'+img_name+'.c'
    print(img_name)
    img = Image.open(icon_folder+in_path).convert('RGB')
    pixels = list(img.getdata())
    img_size = img.size
    
    with open(out_path, 'w') as f:
        write_bin(f, pixels,img_name,img_size)

