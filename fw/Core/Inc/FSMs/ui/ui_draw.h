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

#define DRAWER_SELECTION_FONT Font20
#define DRAWER_SELECTION_COLOR LCD_COLOR_BLUE

#define FEEDER_HEADER_FONT  Font16
#define FEEDER_CONFIG_SELECTION_COLOR  LCD_COLOR_BLUE


typedef enum
{
    FEEDER_MEAL_BREAKFAST,
    FEEDER_MEAL_SNACK_1,
    FEEDER_MEAL_LUNCH,
    FEEDER_MEAL_SNACK_2,
    FEEDER_MEAL_DINNER,
    FEEDER_MEAL_SNACK_3,
    FEEDER_MEALn,
}feeder_meals_t;

typedef enum
{
    TIME_AM,
    TIME_FM,
    TIMEn,
}time_am_fm_t;

typedef enum
{
    DRAWER_1,
    DRAWER_2,
    DRAWER_3,
    DRAWER_4,
    DRAWERn,
}drawers_t;


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
    sFONT    *font; 
}ui_font_t;

typedef struct
{
    struct
    {
        ui_window_t main;
    }win;

    struct
    {
        ui_window_t charge;
    }form;

    struct
    {
        ui_icon_t   icon_batt;
        ui_icon_t   icon_error;
        ui_icon_t   icon_warn;
    }icon;

    struct
    {
        ui_font_t   charge;
    }text;
}ui_battery_t;

typedef struct
{
    struct
    {
        ui_window_t main;
    }win;

    struct
    {
        ui_window_t temp_lvl;   //rectangle
    }form;

    struct
    {
        ui_icon_t   therm;
    }icon;

    struct 
    {
        ui_font_t   temp_lvl;
    }text;
    
}ui_thermostat_t;

typedef struct
{
    struct
    {
        ui_window_t main;
    }win;

    struct
    {
        ui_window_t main_frame;
        ui_window_t single_frame;
    }form;

}ui_drawers_t;

typedef struct
{
    struct 
    {
        ui_window_t main;
        ui_window_t hour;
        ui_window_t minute;
        ui_window_t month;
        ui_window_t day;
    }win;

    struct
    {
        ui_font_t hour;
        ui_font_t minute;
        ui_font_t month;
        ui_font_t day;
    }text;

}ui_date_time_t;

typedef struct
{
    struct 
    {
        ui_window_t main;
        ui_window_t set_temp;
        ui_window_t enable_f_c;
        ui_window_t enable_temp;
    }win;

    struct
    {
        ui_icon_t temp_units;
        ui_icon_t therm;
    }icon;

    struct
    {
        ui_font_t set_temp;
    }text;

}ui_thermostat_config_t;


typedef struct 
{
    struct {
        uint16_t x;
        uint16_t y;
        char val[3];
    }hour;

    struct {
        uint16_t x;
        uint16_t y;
        char val[3];
    }min;

    struct {
        uint16_t x;
        uint16_t y;
    }am_fm;

}time_config_t;

typedef struct 
{
    struct {
        uint16_t x;
        uint16_t y;
        char val[3];
    }month;

    struct {
        uint16_t x;
        uint16_t y;
        char val[3];
    }day;

}date_config_t;

typedef struct
{
    time_config_t open_time;
    time_config_t close_time;
    date_config_t date;
}feeder_time_date_t;

typedef struct
{
    struct 
    {
        ui_window_t main;
        ui_window_t daily_en;    
    }win;

    feeder_time_date_t config[FEEDER_MEALn];
}feeder_config_t;

extern ui_battery_t ui_battery;
extern ui_thermostat_t ui_thermostat;
extern ui_drawers_t ui_drawers;
extern ui_date_time_t ui_date_time;
extern ui_thermostat_config_t ui_therm_conf;
extern feeder_config_t ui_feeder_conf;

/* Common functions */
void ui_win_show(ui_window_t *win,uint16_t color, bool show);

/* Battery related functions */
void ui_battery_init(ui_battery_t *batt);
void ui_battery_show(ui_battery_t *batt, bool show);
void ui_battery_error(ui_battery_t *batt, bool show);
void ui_battery_warn(ui_battery_t *batt, bool show);
void ui_battery_charge(ui_battery_t *batt, uint8_t batt_level, bool show);

/* Drawers related functions */
void ui_drawers_init(ui_drawers_t *drawers);
void ui_drawers_select(ui_drawers_t *drawers, uint8_t drawer_no);

/* Feeder Configuration functions */
void ui_feeder_config_init(feeder_config_t *feeder_conf);
void ui_feeder_config_set_open_time_hour(feeder_config_t *feeder_conf, uint8_t meal, uint8_t open_hour);
void ui_feeder_config_confirm_open_time_hour(feeder_config_t *feeder_conf, uint8_t meal, uint8_t open_hour);
void ui_feeder_config_set_open_time_min(feeder_config_t *feeder_conf, uint8_t meal, uint8_t open_min);
void ui_feeder_config_confirm_open_time_min(feeder_config_t *feeder_conf, uint8_t meal, uint8_t open_min);
void ui_feeder_config_set_am_fm(feeder_config_t *feeder_conf, uint8_t meal, uint8_t am_fm);
void ui_feeder_config_confirm_am_fm(feeder_config_t *feeder_conf, uint8_t meal, uint8_t am_fm);





#endif