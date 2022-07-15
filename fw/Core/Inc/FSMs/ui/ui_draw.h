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
#include "event_manager_fsm.h"

#define DRAWER_NUMBER_FONT Font20
#define DRAWER_SELECTION_COLOR LCD_COLOR_BLUE

#define FEEDER_HEADER_FONT  Font16
#define UI_SELECTION_COLOR  LCD_COLOR_BLUE


typedef enum
{
    UI_ITEM_SELECT_INVALID,
    UI_ITEM_SELECT,
    UI_ITEM_DESELECT,
    UI_ITEM_SELECT_LAST,
}ui_select_t;

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
    ui_window_t hour;
    ui_window_t min;
    ui_window_t am_fm;
}ui_time_t;

typedef struct 
{
    ui_window_t month;
    ui_window_t day;
}ui_date_t;


typedef enum
{
    UI_MAIN_MENU_ITEM_DRAWERS,
    UI_MAIN_MENU_ITEM_THERMOSTAT,
    UI_MAIN_MENU_ITEM_PETCALL,
    UI_MAIN_MENU_ITEM_BATTERY,
    UI_MAIN_MENU_ITEM_DATE_TIME,
    UI_MAIN_MENU_ITEMn
}ui_main_menu_sel_item_t;

typedef enum
{
    UI_DT_ITEM_INVALID,
    UI_DT_ITEM_HOUR,
    UI_DT_ITEM_MIN,
    UI_DT_ITEM_DAY,
    UI_DT_ITEM_MONTH,
    UI_DT_ITEM_LAST
}ui_date_time_menu_sel_item_t;

typedef enum
{
    THERM_ICON_CNF_INVALID,
    THERM_ICON_SET_SENSED_TEMP,
    THERM_ICON_CNF_LAST,
}ui_thermostat_icon_config_t;

typedef enum
{
    THERM_INVALID,
    THERM_SET_TEMPERATURE,
    THERM_SET_UNIT,
    THERM_ENABLE_DISABLE,
    THERM_LAST,
}ui_thermostat_config_list_t;


typedef enum
{
    UI_PETCALL_CNF_INVALID,
    UI_PETCALL_CNF_ENABLE_DISABLE,
    UI_PETCALL_CNF_REC_START_STOP,
    UI_PETCALL_CNF_SCORE_START_STOP,
    UI_PETCALL_CNF_DELETE_RECORDING,
    UI_PETCALL_CNF_EXIT,
    UI_PETCALL_CNF_LAST,
}ui_petcall_config_list_t;


//------------- UI Battery Configuration ----------------//
typedef struct
{
    struct { ui_window_t main;   }win;
    struct { ui_window_t charge; }shape;
    struct {
        ui_icon_t   batt;
        ui_icon_t   error;
        ui_icon_t   warn;
    }icon;
    ui_window_t  text;
}ui_battery_t;

typedef struct 
{
    battery_st_t set;
    uint8_t      charge;
    ui_select_t  select;
}ui_battery_config_t;

//------------- UI Drawers Configuration ----------------//
typedef struct
{
    struct
    {
        ui_window_t main;
    }win;

    struct
    {
        ui_window_t main;
        ui_window_t single;
    }shape;

}ui_drawers_t;

typedef struct
{
    struct
    {
        drawer_st_t st;
        drawer_no_t no;
    }drawer;

    struct
    {
     ui_select_t main;
     ui_select_t single;
    }select;

}ui_drawers_config_t;

//------------- UI Feeder Menu Configuration ----------------//
typedef struct
{
    struct
    {
        ui_time_t open;
        ui_time_t close;
    } time;
    ui_date_t date;
}feeder_time_date_t;

typedef struct
{
    struct 
    {
        ui_window_t main;
        ui_window_t daily;    
    }win;

    feeder_time_date_t meal_td[FEEDER_MEALn];
}ui_feeder_menu_t;

typedef struct
{
    feeder_meal_t meal;
    feeder_meal_config_t set;

    struct
    {
        struct
        {
            uint8_t hour;
            uint8_t minute;
            time_am_fm_t am_fm;
        } open;

        struct
        {
            uint8_t hour;
            uint8_t minute;
            time_am_fm_t am_fm;
        } close;

    } time;

    struct
    {
        uint8_t day;
        uint8_t month;
        feeder_daily_st_t daily_st;
    } date;

    struct
    {
        ui_select_t main;
        ui_select_t single;
    }select;

}ui_feeder_config_t;

//------------- UI Date Time Main ----------------//

typedef struct
{
    
    struct 
    {
        ui_window_t main;
    }win;

    ui_time_t time;
    ui_date_t date;
}ui_date_time_menu_t;

typedef struct
{
    date_time_config_t set;
    struct
    {
        struct
        {
            uint8_t hour;
            uint8_t min;
            time_am_fm_t am_fm;
        } time;

        struct
        {
            uint8_t day;
            uint8_t month;
        }date;
    };

    struct{
        ui_select_t main;
        ui_select_t single;
    }select;

}ui_date_time_config_t;

//------------- UI Thermostat Icon ----------------//
typedef struct
{
    struct { ui_window_t main; } win;
    struct { ui_icon_t  therm; } icon;
    struct { ui_window_t  temp;} text;

    struct { 
        ui_window_t temp; 
        ui_window_t circle;
    } shape;

}ui_thermostat_t;


typedef struct
{
    ui_thermostat_icon_config_t set;
    struct
    {
        uint8_t val;
        temperature_unit_t unit;
    }temp;

    ui_select_t select;
}ui_thermostat_config_t;


//------------- UI Thermostat Config Menu ----------------//

typedef struct
{
    struct 
    {
        ui_window_t main;
        ui_window_t set_temp;
        ui_window_t unit_c;
        ui_window_t unit_f;
        ui_window_t temp_on;
        ui_window_t temp_off;
        ui_window_t selection;
    }win;

    struct
    {
        ui_icon_t therm;
    }icon;

    struct
    {
        ui_window_t ctrl_temp;
    }text;

}ui_thermostat_menu_t;


typedef struct
{
    ui_thermostat_config_list_t set;
    struct
    {
        uint8_t val;
        temperature_unit_t unit;
        temperature_ctrl_st_t status;
    }temp;


    struct{
        ui_select_t main;
        ui_select_t single;
    }select;
}
ui_thermostat_menu_config_t;

//------------- PetCall icon ----------------//

typedef struct
{
    struct
    {
        ui_window_t main;
    }win;

    struct
    {
        ui_icon_t mic;
    }icon;
    
}ui_petcall_t;

typedef struct
{
    struct {
        ui_select_t main;
        ui_select_t single;
    }select;
}ui_petcall_config_t;


//------------- PetCall icon ----------------//

typedef struct
{
    struct
    {
        ui_window_t win;
        ui_window_t title;
    } main;

    struct
    {
        ui_window_t win;
        ui_window_t title;
    } on_off;

    struct
    {
        ui_window_t win;
        ui_window_t title;
    } mic;

    struct
    {
        ui_window_t win;
        ui_window_t title;
    } play;

    struct
    {
        ui_window_t win;
        ui_window_t title;
    }erase;

} ui_petcall_menu_t;

typedef struct
{
    ui_petcall_config_list_t set;

    struct {
        ui_select_t main;
        ui_select_t single;
    }select;

    petcall_rec_file_t  delete_file;
    petcall_recording_t rec_start_stop;
    petcall_score_t     play_stop;
    petcall_st          en_dis;

}ui_petcall_menu_config_t;


//------------- Notification  ----------------//
typedef struct
{
    struct 
    {
        ui_window_t main;
        ui_window_t text;
    }win;
}ui_notification_msg_t;


//-------------------------------------------//


extern ui_battery_t    ui_battery;
extern ui_drawers_t    ui_drawers;
extern ui_thermostat_t ui_thermostat;
extern ui_petcall_t    ui_petcall;

extern ui_date_time_menu_t    ui_date_time_menu;
extern ui_feeder_menu_t       ui_feeder_menu;
extern ui_thermostat_menu_t   ui_therm_menu;
extern ui_petcall_menu_t      ui_petcall_menu;
extern ui_notification_msg_t  ui_notification;


/* Battery Icon related functions */
void ui_battery_init(ui_battery_t *batt);
void ui_battery_show(ui_battery_t *batt, bool show);
void ui_battery_set_config(ui_battery_t *batt, ui_battery_config_t *config);

/* Drawers related functions */
void ui_drawers_init(ui_drawers_t *drawers);
void ui_drawers_show(ui_drawers_t *drawers, bool show);
void ui_drawers_set_config(ui_drawers_t *drawers, ui_drawers_config_t *config);

/* Feeder Config Menu functions */
void ui_feeder_menu_init(ui_feeder_menu_t *menu);
void ui_feeder_menu_show(ui_feeder_menu_t *menu, bool show);
void ui_feeder_menu_set_config(ui_feeder_menu_t *menu, ui_feeder_config_t *config);

/* Date Time Config Menu Functions */
void ui_date_time_menu_init(ui_date_time_menu_t *menu);
void ui_date_time_menu_show(ui_date_time_menu_t *menu, bool show);
void ui_date_time_menu_set_config(ui_date_time_menu_t *menu, ui_date_time_config_t *config);

/* Thermostat icon Functions */
void ui_thermostat_init(ui_thermostat_t *therm);
void ui_thermostat_show(ui_thermostat_t *therm, bool show);
void ui_thermostat_set_config(ui_thermostat_t *therm, ui_thermostat_config_t *config);

/* Thermostat Config Menu Functions */
void ui_thermostat_menu_init(ui_thermostat_menu_t *menu);
void ui_thermostat_menu_show(ui_thermostat_menu_t *menu, bool show);
void ui_thermostat_menu_set_config(ui_thermostat_menu_t *menu, ui_thermostat_menu_config_t *config);

/* Pet Call Icon Function */
void ui_petcall_init(ui_petcall_t *menu);
void ui_petcall_show(ui_petcall_t *menu, bool show);
void ui_petcall_set_config(ui_petcall_t *menu, ui_petcall_config_t *config);

/* Pet Call Config Menu Function */
void ui_petcall_menu_init(ui_petcall_menu_t *menu);
void ui_petcall_menu_show(ui_petcall_menu_t *menu, bool show);
void ui_petcall_menu_set_config(ui_petcall_menu_t *menu, ui_petcall_menu_config_t *config);

/* Notification messages */
void ui_notification_msg_init(ui_notification_msg_t *menu);
void ui_notification_msg_show(ui_notification_msg_t *menu, bool show);
void ui_notification_msg_set(ui_notification_msg_t *menu, char *msg);


#endif
