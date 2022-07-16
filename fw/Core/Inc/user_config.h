/**
 * @file user_config.h
 * @author Bayron Cabrera (bdcabreran@unal.edu.co)
 * @brief This file contains the functions to save/read user configurations from flash, keep in mind
 * every time the info is updated the flash page is cleared, so you have to store all the info at once
 * @version 0.1
 * @date 2022-07-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef USER_CONFIG_H
#define USER_CONFIG_H

#include "flash_memory.h"
#include "event_manager_fsm.h"

typedef struct 
{
    bool                     data_available;
    thermostat_config_info_t thermostat_info;
    petcall_config_info_t    petcall_info;
    feeder_config_info_t     feeder_info;
}user_config_t;


void user_config_set(void);
user_config_t *user_config_get(void);


#endif
