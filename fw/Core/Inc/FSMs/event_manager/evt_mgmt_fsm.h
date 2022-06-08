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

#ifndef EVT_MGMT_FSM_H
#define EVT_MGMT_FSM_H

typedef struct evt_mgmt_fsm_t* evt_mgmt_handle_t;

#include "event_queue.h"

evt_mgmt_handle_t evt_mgmt_fsm_get(void);

void evt_mgmt_fsm_init(evt_mgmt_handle_t handle);
void evt_mgmt_fsm_run(evt_mgmt_handle_t handle);

uint8_t evt_mgmt_write(evt_mgmt_handle_t handle, event_t *event);
uint8_t evt_mgmt_fetch(evt_mgmt_handle_t handle, event_t *event);

#endif 
