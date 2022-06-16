/**
 * @file printf_dbg.h
 * @author Bayron Cabrera (bdcabreran@unal.edu.co)
 * @brief  Event Queue Manager, all event flow from other FSMs will have to go through this manager
 * @version 0.1
 * @date 2022-05-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef EVENT_MANAGER_FSM_H
#define EVENT_MANAGER_FSM_H

#include "event_queue.h"
#include "ui_fsm.h"
#include "drawer_fsm.h"
#include "power_fsm.h"
#include "feeder_fsm.h"
#include "temp_ctrl_fsm.h"

typedef enum
{
    UI_FSM,
    BTN_FSM,
    DRAWER_FSM,
    FEEDER_FSM,
    TEMP_CTRL_FSM,

}fsm_list_t;


typedef struct event_manager_fsm_t* event_manager_handle_t;

event_manager_handle_t event_manager_fsm_get(void);

void event_manager_fsm_init(event_manager_handle_t handle);
void event_manager_fsm_run(event_manager_handle_t handle);

uint8_t event_manager_write(event_manager_handle_t handle, event_t *event);
uint8_t event_manager_fetch(event_manager_handle_t handle, event_t *event);

#endif 
