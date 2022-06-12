/**
 * @file ui_draw.h
 * @author Bayron Cabrera (bdcabreran@unal.edu.co)
 * @brief This file contains the icons and the window elements to be displayed in the application
 * @version 0.1
 * @date 2022-06-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef UI_DRAW_H
#define UI_DRAW_H

#include "stdint.h"
#include "lcd_api.h"
#include "bmp.h"
#include "stdbool.h"

extern BITMAPSTRUCT battery_icon[];
extern BITMAPSTRUCT batt_error_icon[];
extern BITMAPSTRUCT batt_warning_icon[];

typedef struct
{
    uint16_t x; 
    uint16_t y; 
    uint16_t w; 
    uint16_t h;     
}ui_window_t;

typedef struct
{   
    uint16_t x; 
    uint16_t y; 
    BITMAPSTRUCT *ptr;
}ui_icon_t;

typedef struct
{
    uint16_t x; 
    uint16_t y;
    sFONT    *f; 
}ui_font_t;

typedef struct
{
    ui_window_t win;
    ui_window_t charge;
    ui_icon_t   cont;
    ui_icon_t   error;
    ui_icon_t   warn;
    ui_font_t   font;
}ui_battery_t;

extern ui_battery_t ui_battery;

/* Common functions */
void ui_win_show(ui_window_t *win, bool show);

/* Battery related functions */
void ui_battery_init(ui_battery_t *batt);
void ui_battery_show(ui_battery_t *batt, bool show);
void ui_battery_error(ui_battery_t *batt, bool show);
void ui_battery_warn(ui_battery_t *batt, bool show);
void ui_battery_charge(ui_battery_t *batt, uint8_t batt_level, bool show);



#endif