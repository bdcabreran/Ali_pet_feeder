/**
 * @file feeder_fsm.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-06-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef FEEDER_FSM_H
#define FEEDER_FSM_H

typedef enum
{
    FEEDER_MEAL_BREAKFAST,
    FEEDER_MEAL_SNACK_1,
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



#endif