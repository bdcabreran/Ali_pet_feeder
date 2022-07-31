/**
 * @file printf_dbg.h
 * @author Bayron Cabrera (bdcabreran@unal.edu.co)
 * @brief  Custom printf with colors 
 * @version 0.1
 * @date 2022-05-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef PRINTF_DBG_H_
#define PRINTF_DBG_H_

//#include "uart_driver.h" 
#include <stdio.h>
#include <stdbool.h>
#include "itm_dbg.h"
#include "init_periph.h"

#define USE_PRINTF_ITM   (0)
#define USE_PRINTF_UART2 (0)

#define USE_PRINTF_COLOR (0)

typedef enum
{
  E_RED  ,
  E_GRN  ,
  E_YEL  ,
  E_BLU  ,
  E_MAG  ,
  E_CYN  ,
  E_WHT  ,
  E_RESET,
  E_LAST,
}printf_color_t;

extern const char *str_color[E_LAST];

#define DBG_MSG_DATA_CHUNK (50)
#define printf_dbg(format, ...)           printf(format, ##__VA_ARGS__)

#define printf_dbg_info(format, ...)     \
    do                                 \
    {                                  \
        printf(str_color[E_MAG]);      \
        printf(format, ##__VA_ARGS__); \
        printf(str_color[E_RESET]);    \
    } while (0)

#define printf_dbg_error(format, ...)    \
    do                                 \
    {                                  \
        printf(str_color[E_RED]);      \
        printf(format, ##__VA_ARGS__); \
        printf(str_color[E_RESET]);    \
    } while (0)

#define printf_dbg_warn(format, ...)     \
    do                                 \
    {                                  \
        printf(str_color[E_YEL]);      \
        printf(format, ##__VA_ARGS__); \
        printf(str_color[E_RESET]);    \
    } while (0)

#define printf_dbg_color(color, format, ...) \
    do                                     \
    {                                      \
        /*printf(str_color[color]);*/      \
        printf(format, ##__VA_ARGS__);     \
        /*printf(str_color[E_RESET]);*/       \
    } while (0)


void print_buffer(uint8_t format, uint8_t *buff, uint8_t len);


#endif
