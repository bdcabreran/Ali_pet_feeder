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

#include "stdint.h"
#include "stdbool.h"

/* Update This values Accordingly */
#define TEMP_SAMPLING_PERIOD_MS             (5000)
#define TEMP_CTRL_DEFAULT_CELSIUS_VALUE       (20)
#define TEMP_CTRL_DEFAULT_FAHRENHEIT_VALUE    (68)
#define TEMP_CTRL_HYSTERESIS_VALUE_CELSIUS    (5) /*5 degrees */ 
#define TEMP_CTRL_HYSTERESIS_VALUE_FAHRENHEIT (9) /*9 degrees */

#define IS_TEMP_CELSIUS_TOO_COLD(ctrl, sensed) (sensed < (ctrl - TEMP_CTRL_HYSTERESIS_VALUE_CELSIUS))
#define IS_TEMP_CELSIUS_TOO_HOT(ctrl, sensed)  (sensed > (ctrl + TEMP_CTRL_HYSTERESIS_VALUE_CELSIUS))
#define IS_TEMP_CELSIUS_OUT_OF_RANGE(ctrl, sensed) (IS_TEMP_CELSIUS_TOO_HOT(ctrl, sensed) || IS_TEMP_CELSIUS_TOO_COLD(ctrl, sensed))

#define IS_TEMP_FAHRENHEIT_TOO_COLD(ctrl, sensed) (sensed < (ctrl - TEMP_CTRL_HYSTERESIS_VALUE_FAHRENHEIT))
#define IS_TEMP_FAHRENHEIT_TOO_HOT(ctrl, sensed)  (sensed > (ctrl + TEMP_CTRL_HYSTERESIS_VALUE_FAHRENHEIT))
#define IS_TEMP_FAHRENHEIT_OUT_OF_RANGE(ctrl, sensed) (IS_TEMP_FAHRENHEIT_TOO_HOT(ctrl, sensed) || IS_TEMP_FAHRENHEIT_TOO_COLD(ctrl, sensed))

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


typedef struct
{
    struct
    {
        uint8_t sensed;
        uint8_t control;
    }value;
    temperature_unit_t unit;
}thermostat_info_t;

typedef struct temp_ctrl_fsm_t* temp_ctrl_handle_t;


temp_ctrl_handle_t temp_ctrl_fsm_get(void);
thermostat_info_t *temp_ctrl_get_info(void);
void temp_ctrl_fsm_init(temp_ctrl_handle_t handle);
void temp_ctrl_fsm_run(temp_ctrl_handle_t handle);
void temp_ctrl_time_update(temp_ctrl_handle_t handle);






#endif