 
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
#endif 
# define   WIDTH   88
# define   HEIGHT  53

const BITMAPSTRUCT battery_icon __attribute__((aligned)) =
    {
        {
         0x4d42u,
         sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER) + (WIDTH * HEIGHT * 2),
         0x0000u,
         0x0000u,
         sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER)
         },
        {
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
         },
        {
    /*(88, 53)*/

			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xef7d,0xdedb,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,
			0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,
			0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd69a,0xd6ba,0xe71c,0xffdf,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xe71c,0x6b4d,0x3186,0x2945,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,
			0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,
			0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2924,0x2945,0x4208,0xad75,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffdf,0x9cd3,
			0x3165,0x20e3,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,
			0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,
			0x20e4,0x20e4,0x20e4,0x20e4,0x20e3,0x20e3,0x5acb,0xe6fc,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x9cf3,0x2104,0x20e3,0x20e3,0x20e3,0x2104,0x2104,
			0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,
			0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x20e4,0x20e4,
			0x20e4,0x20e4,0x2925,0xe71c,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xbdd7,0x2104,0x20e4,0x20e4,0x20e3,0x6b4d,0xbdd7,0xc638,0xce59,0xce59,0xce59,0xce59,0xce59,
			0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,
			0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,0xce59,0xc618,0x9cd3,0x39c7,0x20e4,0x20e4,0x20e3,0x632c,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffdf,0x5acb,0x20e3,0x20e3,0x2945,0xad55,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xe73c,0x4a69,0x20e3,0x20e3,0x20e4,0xc638,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xef7d,0x2945,0x20e4,0x20e3,0x7bcf,0xffdf,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xef5d,0x3165,0x20e4,0x20e3,0x5acb,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xbdf7,0x18e3,0x20e3,0x20e4,0xd6ba,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x7bcf,0x20e3,0x20e4,0x31a6,0xef7d,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x9cf3,0x18e3,0x20e4,
			0x2945,0xf7be,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x9cd3,0x20e4,0x20e3,0x3186,0xe73c,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x9cf3,0x18e3,0x20e4,0x2965,0xf7be,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0x9cf3,0x20e4,0x20e4,0x3166,0xe73c,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x9cf3,0x18e3,0x20e4,0x2965,0xf7be,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x9cf3,0x20e4,0x20e4,
			0x3166,0xe73c,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x9cf3,0x18e3,0x20e4,0x2965,0xf7be,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x9cf3,0x20e4,0x20e4,0x3166,0xe73c,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x8c51,0x20e3,0x20e4,0x2965,0xf7be,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x9cf3,0x20e4,0x20e4,0x3166,0xe73c,0xffff,0xffff,0xffff,0xffff,0xffdf,0xf7be,0xf7be,0xffdf,
			0xf7be,0xf7be,0xdefb,0x2924,0x20e4,0x20e4,0x2965,0xf7be,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x9cf3,0x20e4,0x20e4,0x3166,0xe73c,0xffff,0xffff,0xffff,0x9cd3,0x4a49,0x39c7,0x39c7,0x39c7,0x39c7,0x39c7,0x2945,0x20e4,0x20e4,
			0x20e4,0x2965,0xf7be,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x9cf3,0x20e4,0x20e4,0x3166,0xe73c,0xffff,0xffff,0xbdd7,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x2965,0xf7be,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0x9cf3,0x20e4,0x20e4,0x3166,0xe73c,0xffff,0xffff,0x5aab,0x20e3,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x2965,0xf7be,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x9cf3,0x20e4,
			0x20e4,0x3166,0xe73c,0xffff,0xffdf,0x4a49,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x2965,0xf7be,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x9cf3,0x20e4,0x20e4,0x3166,0xe73c,0xffff,0xffdf,
			0x4a49,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x2965,0xf7be,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x9cf3,0x20e4,0x20e4,0x3166,0xe73c,0xffff,0xffdf,0x4a49,0x20e4,0x20e4,0x20e4,0x20e4,
			0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x2965,0xf7be,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x9cf3,0x20e4,0x20e4,0x3166,0xe73c,0xffff,0xffdf,0x4a49,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,
			0x20e4,0x20e4,0x2965,0xf7be,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x9cf3,0x20e4,0x20e4,0x3166,0xe73c,0xffff,0xffdf,0x4a49,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x2965,0xf7be,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0x9cf3,0x20e4,0x20e4,0x3166,0xe73c,0xffff,0xffdf,0x4a49,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x2965,0xf7be,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x9cf3,
			0x20e4,0x20e4,0x3166,0xe73c,0xffff,0xffdf,0x4a49,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x2965,0xf7be,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x9cf3,0x20e4,0x20e4,0x3166,0xe73c,0xffff,
			0xffdf,0x4a49,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x2965,0xf7be,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x9cf3,0x20e4,0x20e4,0x3166,0xe73c,0xffff,0xffdf,0x4a49,0x20e4,0x20e4,0x20e4,
			0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x2965,0xf7be,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x9cf3,0x20e4,0x20e4,0x3166,0xe73c,0xffff,0xffdf,0x4a49,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,
			0x20e4,0x20e4,0x20e4,0x2965,0xf7be,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x9cf3,0x20e4,0x20e4,0x3166,0xe73c,0xffff,0xffdf,0x4a49,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x2965,0xf7be,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0x9cf3,0x20e4,0x20e4,0x3166,0xe73c,0xffff,0xffdf,0x4a49,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x2965,0xf7be,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0x9cf3,0x20e4,0x20e4,0x3166,0xe73c,0xffff,0xffdf,0x4a49,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x2965,0xf7be,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x9cf3,0x20e4,0x20e4,0x3166,0xe73c,
			0xffff,0xffdf,0x4a49,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x2965,0xf7be,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x9cf3,0x20e4,0x20e4,0x3166,0xe73c,0xffff,0xffdf,0x4a49,0x20e4,0x20e4,
			0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x2965,0xf7be,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x9cf3,0x20e4,0x20e4,0x3166,0xe73c,0xffff,0xffdf,0x4a49,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,
			0x20e4,0x20e4,0x20e4,0x20e4,0x2965,0xf7be,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x9cf3,0x20e4,0x20e4,0x3166,0xe73c,0xffff,0xffdf,0x4a49,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x2965,
			0xf7be,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x9cf3,0x20e4,0x20e4,0x3166,0xe73c,0xffff,0xffdf,0x4a49,0x20e3,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x2965,0xf7be,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0x9cf3,0x20e4,0x20e4,0x3166,0xe73c,0xffff,0xffff,0x52aa,0x20e3,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x2965,0xf7be,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x9cf3,0x20e4,0x20e4,0x3166,
			0xe73c,0xffff,0xffff,0xad75,0x18e3,0x20e3,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x2965,0xf7be,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x9cf3,0x20e4,0x20e4,0x3166,0xe73c,0xffff,0xffff,0xffff,0x8c51,
			0x31a6,0x2945,0x2925,0x2925,0x2925,0x2925,0x2104,0x20e4,0x20e3,0x20e4,0x2965,0xf7be,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x9cf3,0x20e4,0x20e4,0x3166,0xe73c,0xffff,0xffff,0xffff,0xffff,0xf7be,0xf79e,0xf79e,0xf79e,0xf79e,
			0xf79e,0xce79,0x20e4,0x20e4,0x20e4,0x2965,0xf7be,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x9cf3,0x20e4,0x20e4,0x3166,0xe73c,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x8430,0x20e3,0x20e4,
			0x2965,0xf7be,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x9cf3,0x20e4,0x20e4,0x3166,0xe73c,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x9cf3,0x18e3,0x20e4,0x2965,0xf7be,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0x9cf3,0x20e4,0x20e4,0x3166,0xe73c,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x9cf3,0x18e3,0x20e4,0x2965,0xf7be,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x9cf3,0x20e4,0x20e4,
			0x3166,0xe73c,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x9cf3,0x18e3,0x20e4,0x2965,0xf7be,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x9cf3,0x20e4,0x20e4,0x3186,0xe73c,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x9cf3,0x18e3,0x20e4,0x2945,0xf7be,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x9cd3,0x20e3,0x20e4,0x3186,0xe73c,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xbdd7,0x20e3,0x20e3,0x20e4,0xdedb,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x7bef,0x20e3,0x20e4,0x31a6,0xef7d,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xef5d,0x2925,
			0x20e3,0x20e3,0x8430,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xf79e,0x31a6,0x20e4,0x20e3,0x52aa,0xffdf,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffdf,0x528a,0x20e3,0x20e4,0x2965,0xbdf7,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xef7d,0x5acb,0x20e4,0x20e3,0x20e4,0xbdd7,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xad75,0x2104,0x20e3,0x20e4,0x2104,0x8410,0xc638,0xce79,0xce79,0xce79,0xce79,
			0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,
			0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xce79,0xad75,0x4a49,0x20e3,0x20e4,0x20e4,
			0x5acb,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x8c51,0x20e3,0x20e4,0x20e4,0x20e4,0x2104,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,
			0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,
			0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2124,0x2104,0x20e4,0x20e4,0x20e3,0x2104,0xd6ba,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xf7be,0x8c31,0x2924,0x20e3,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,
			0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,
			0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e4,0x20e3,0x20e4,0x18e3,0x4a49,0xd69a,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xd69a,0x4a69,0x2945,0x2124,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,
			0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,
			0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2104,0x2925,0x31a6,0x8c71,0xf7be,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
			0xffff,0xffff,0xffff,0xffff,0xffdf,0xe71c,0xce79,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,
			0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,0xc638,
			0xc638,0xc638,0xc638,0xc638,0xc638,0xce59,0xd6ba,0xf79e,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
}};

#ifdef  __GNUC__
#pragma pack (pop)
#elif   defined(__CC_ARM)
#pragma pop
#endif
