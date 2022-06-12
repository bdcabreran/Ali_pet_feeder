#ifndef UI_FSM_H
#define UI_FSM_H

#include "event_manager_fsm.h"


typedef struct ui_fsm_t* ui_handle_t;


ui_handle_t ui_fsm_get(void);

void ui_fsm_init(ui_handle_t handle);
void ui_fsm_run(ui_handle_t handle);
void ui_fsm_time_update(ui_handle_t handle);
void ui_fsm_set_ext_event(ui_handle_t handle, event_t *event);

#endif