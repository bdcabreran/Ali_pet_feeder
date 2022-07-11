/**
 * @file power_fsm.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-06-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef POWER_FSM_H
#define POWER_FSM_H

#include "event_manager_fsm.h"

typedef enum
{
    BATT_ST_INVALID,
    BATT_ST_NO_DETECTED,
    BATT_ST_LOW_BATTERY,
    BATT_ST_CHARGING,
    BATT_ST_DISCHARGING,
    BATT_ST_LAST,
}battery_st_t;

typedef struct
{
    battery_st_t  status;
    uint8_t       charge;
}battery_info_t;

typedef enum
{
    PWR_SRC_BATTERY,
    PWR_SRC_SUPPLY,
}power_source_type_t;

typedef struct
{
    power_source_type_t power_src;
    battery_info_t battery;
}power_fsm_info_t;

typedef struct power_fsm_t* power_handle_t;


power_fsm_info_t *power_fsm_get_info(void);
power_handle_t power_fsm_get(void);
void power_fsm_init(power_handle_t handle);
void power_fsm_run(power_handle_t handle);
void power_fsm_time_update(power_handle_t handle);



#endif