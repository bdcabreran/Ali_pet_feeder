/**
 * @file time_irq_update.c
 * @author Bayron Cabrera(bayron.carbera@titoma.com)
 * @brief  This library is in charge of updating the time events in FSM
 * using HALSysTick 1 ms interrupt as a time base 
 * @version 0.1
 * @date 2022-05-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "time_event.h"
#include "stm32f1xx_hal.h"
#include "buttons.h"
#include "event_manager_fsm.h"

/**
 * @brief Systick Callback Function 
 * @note  This callback is executed every 1 ms
 */
void HAL_SYSTICK_Callback(void)
{
    /* update FSM time events*/
    time_event_set_pending_update(true);
}

/**
 * @brief Call this function in main thread at infinite loop
 * 
 */
void time_events_poll_update(void)
{
    if(time_event_get_pending_update() == true)
    {
        /*Update Time Events of the state machines */
        //---------------------------------------------------//
        btn_debounce_run(&navigation_btn);
        ui_fsm_time_update(ui_fsm_get());
        temp_ctrl_fsm_time_update(temp_ctrl_fsm_get());
        feeder_fsm_time_update(feeder_fsm_get());
        //---------------------------------------------------//
        time_event_set_pending_update(false); 
    }
}
