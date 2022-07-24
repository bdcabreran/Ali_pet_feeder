from PIL import Image
import struct, os, sys

def write_image_header(f, img_size) :

 header_start = """ 
#include <stdint.h>
#include "lcd.h"
#include "bmp.h"
        
#if LCD_REVERSE16 == 0
#define  RC(a)   a
#endif
#if LCD_REVERSE16 == 1
#define  RC(a)   ((((a) & 0xFF) << 8) | (((a) & 0xFF00) >> 8))
#endif

// struct packing
#ifdef  __GNUC__
#pragma pack(push, 1)
#elif   defined(__CC_ARM)
#pragma push
#pragma pack(1)
#endif """
 f.write(header_start)
 str_pixel_size  = "\n# define   WIDTH   " + str(img_size[0]) + '\n' + "# define   HEIGHT  " + str(img_size[1]) + '\n'
 f.write(str_pixel_size)


def write_image_end(f) :
 file_end = """
#ifdef  __GNUC__
#pragma pack (pop)
#elif   defined(__CC_ARM)
#pragma pop
#endif
"""
 f.write(file_end)

def write_bin(f, pixel_list,img_name,img_size):
    write_image_header(f, img_size)

    struct_header_str = """
const BITMAPSTRUCT {name} __attribute__((aligned)) =
    {{
        {{
         0x4d42u,
         sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER) + (WIDTH * HEIGHT * 2),
         0x0000u,
         0x0000u,
         sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER)
         }},
        {{
         sizeof(BITMAPINFOHEADER),
         WIDTH,
         HEIGHT,
         1u,
         16,
         0x00000003u,
         (WIDTH * HEIGHT * 2),
         0x00000000ul,
         0x00000000ul,
         0x00000000ul,
         0x00000000ul
         }},
        {{
    """.format(name = img_name)

    f.write(struct_header_str)

    f.write('/*'+str(img_size)+ '*/'+'\n')
    # f.write('const uint16_t '+ img_name + '[]= {' + '\n')
    pixel_cnt = 0
    f.write(('\n\t\t\t'))
    for pix in pixel_list:
        pixel_cnt += 1;
        r = (pix[0] >> 3) & 0x1F
        g = (pix[1] >> 2) & 0x3F
        b = (pix[2] >> 3) & 0x1F
        f.write(( str(hex((r << 11) + (g << 5) + b)) )+ (','))
        if pixel_cnt > 30:
            pixel_cnt = 0
            f.write(('\n\t\t\t'))
    f.write('\n}};')
    f.write(('\n'))
    write_image_end(f)




icon_folder = 'icons/'
files  = os.listdir(icon_folder);
for file in files :
    in_path = file
    img_name  = in_path.rsplit('.', 1)[0]
    out_path ='out_icon/'+img_name+'.c'
    print(img_name)
    img = Image.open(icon_folder+in_path).convert('RGB')
    pixels = list(img.getdata())
    # pixels.reverse()
    img_size = img.size
    
    with open(out_path, 'w') as f:
        write_bin(f, pixels,img_name,img_size)

