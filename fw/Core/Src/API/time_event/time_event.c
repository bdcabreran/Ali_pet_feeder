/**
 * @file time_event.c
 * @author Bayron Cabrera (bdcabreran@unal.edu.co)
 * @brief  time event file for time-driven events in FSM
 * @version 0.1
 * @date 2021-08-18
 */

#include "time_event.h"
#include <stdbool.h>
#include <assert.h>

bool volatile pending_time_update = false;

void time_event_start(time_event_t *time_event, const uint16_t time_ms)
{
    time_event->tick_cnt = time_ms;
    time_event->active = true;
    time_event->raised = false;
}   

void time_event_stop(time_event_t *time_event)
{
    time_event->active = false;
    time_event->raised = false;
    time_event->tick_cnt = 0;
}

bool time_event_is_active(time_event_t *time_event)
{
    return time_event->active;
}

bool time_event_update(time_event_t *time_event)
{
    if (time_event->active == true)
    {
        if (time_event->tick_cnt > 0)
            time_event->tick_cnt--;

        if(!time_event->tick_cnt)
            time_event->raised = true;
        
        return true;
    }
    return false;
}

bool time_event_is_raised(time_event_t *time_event)
{
    return time_event->raised;
}

void time_event_set_pending_update(bool status)
{
    pending_time_update = status;
}

bool time_event_get_pending_update(void)
{
    return pending_time_update;
}


