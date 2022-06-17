/**
 * @file temp_ctrl_fsm.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-06-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef TEMP_CTRL_H
#define TEMP_CTRL_H

typedef enum
{
    TEMP_CTRL_ENABLE,
    TEMP_CTRL_DISABLE,
}temperature_ctrl_st_t;

typedef enum
{
    TEMP_UNITS_CELSIUS,
    TEMP_UNITS_FAHRENHEIT,
    TEMP_UNITSn,
}temperature_unit_t;

typedef enum
{
    THERM_SET_TEMPERATURE,
    THERM_SET_UNIT,
    THERM_ENABLE_CTRL,
}thermostat_config_list_t;

#endif