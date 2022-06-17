/**
 * @file feeder_scheduler.h
 * @author Mauro r (emauriciorg@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-06-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __FEEDER_SCHEDULER_H__
#define __FEEDER_SCHEDULER_H__

#include "event_manager_fsm.h"


typedef struct feeder_scheduler_fsm_t* feeder_scheduler_handle_t;


feeder_scheduler_handle_t feeder_scheduler_fsm_get(void);

void feeder_scheduler_fsm_init(feeder_scheduler_handle_t handle);
void feeder_scheduler_fsm_run(feeder_scheduler_handle_t handle);
void feeder_scheduler_fsm_time_update(feeder_scheduler_handle_t handle);
void feeder_scheduler_fsm_set_ext_event(feeder_scheduler_handle_t handle, event_t *event);

#endif /*__FEEDER_SCHEDULER_H__*/