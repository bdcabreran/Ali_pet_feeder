/**
 * @file feeder.h
 * @author Mauro r (emauriciorg@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-06-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __FEEDER_FSM_H__
#define __FEEDER_FSM_H__

#include "event_manager_fsm.h"

#define FEEDER_UPDATE_TIME_MS   (60000) //1 min 

typedef enum
{
    FEEDER_MEAL_BREAKFAST,
    FEEDER_MEAL_LUNCH,
    FEEDER_MEAL_SNACK_2,
    FEEDER_MEAL_DINNER,
    FEEDER_MEAL_SNACK_3,
    FEEDER_MEALn,
}feeder_meal_t;


typedef enum
{
    FEEDER_CNF_OPEN_TIME_HOUR,
    FEEDER_CNF_OPEN_TIME_MIN,
    FEEDER_CNF_OPEN_TIME_AM_FM,
    FEEDER_CNF_CLOSE_TIME_HOUR,
    FEEDER_CNF_CLOSE_TIME_MIN,
    FEEDER_CNF_CLOSE_TIME_AM_FM,
    FEEDER_CNF_DATE_DAY,
    FEEDER_CNF_DATE_MONTH,
    FEEDER_CNF_DATE_DAILY,
    FEEDER_CNFn,
}feeder_meal_config_t;


typedef enum
{
    FEEDER_DAILY_MEAL_DISABLE,
    FEEDER_DAILY_MEAL_ENABLE,
    FEEDER_DAILYn,
}feeder_daily_st_t;

typedef enum
{
    TIME_AM,
    TIME_FM,
    TIMEn,
}time_am_fm_t;

typedef enum
{
    DATE_TIME_CNF_HOUR,
    DATE_TIME_CNF_MIN,
    DATE_TIME_CNF_DAY,
    DATE_TIME_CNF_MONTH,
}date_time_config_t;

typedef struct
{
    uint8_t hour;
    uint8_t minute;
    time_am_fm_t am_fm;
} time_info_t;

typedef struct
{
    uint8_t day;
    uint8_t month;
} date_info_t;

typedef struct
{
    struct
    {
        time_info_t open;
        time_info_t close;
    } time;
    date_info_t date;

}feeder_meal_data_t;

typedef struct
{
    feeder_meal_data_t config[FEEDER_MEALn];
    feeder_daily_st_t daily_st;
}feeder_drawer_data_t;

typedef struct
{
    struct
    {
        feeder_drawer_data_t no_1;
        feeder_drawer_data_t no_2;
        feeder_drawer_data_t no_3;
        feeder_drawer_data_t no_4;
    }drawer;

}feeder_config_t;


typedef enum
{
    EVT_EXT_feeder_INVALID = 0,
    EVT_EXT_feeder_CONFIG_DATE_TIME,
    EVT_EXT_feeder_CONFIG_FEEDING_TIME,
    EVT_EXT_feeder_LAST
}feeder_ev_ext_name_t;

typedef struct
{
    time_info_t time;
    date_info_t date;
}feeder_date_time_config_t;

typedef struct
{    
    feeder_date_time_config_t config_rtc;
    struct
    {
        uint8_t drawer_no;
        feeder_drawer_data_t *config;
    }config_feeding_time;
}feeder_ev_ext_data_t;

typedef struct
{
    feeder_ev_ext_name_t name;
    feeder_ev_ext_data_t data;
}feeder_ev_ext_t;

extern const char *am_fm_str[TIMEn]; 

typedef struct feeder_fsm_t* feeder_handle_t;

feeder_handle_t feeder_fsm_get(void);
feeder_config_t *feeder_fsm_get_info(void);

void feeder_fsm_init(feeder_handle_t handle);
void feeder_fsm_run(feeder_handle_t handle);
void feeder_fsm_time_update(feeder_handle_t handle);
void feeder_fsm_write_event(feeder_handle_t handle, event_t *event);

#endif /*__feeder_H__*/
