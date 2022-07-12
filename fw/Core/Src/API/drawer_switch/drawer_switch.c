

#include "drawer_switch.h"
#include "printf_dbg.h"

#define SWITCH_DEBOUNCE_TIME_MS (30)


drawer_switches_t drawer_switches;


void drawer_switch_init(drawer_switches_t *ds)
{
    ds->d1.behind.gpio.pin  = S2_Pin;
    ds->d1.behind.gpio.port = S2_GPIO_Port;
    ds->d1.front.gpio.pin  = S1_Pin;
    ds->d1.front.gpio.port = S1_GPIO_Port;

    ds->d2.behind.gpio.pin  = S4_Pin;
    ds->d2.behind.gpio.port = S4_GPIO_Port;
    ds->d2.front.gpio.pin  = S3_Pin;
    ds->d2.front.gpio.port = S3_GPIO_Port;

    ds->d3.behind.gpio.pin  = S6_Pin;
    ds->d3.behind.gpio.port = S6_GPIO_Port;
    ds->d3.front.gpio.pin  = S5_Pin;
    ds->d3.front.gpio.port = S5_GPIO_Port;

    ds->d4.behind.gpio.pin  = S8_Pin;
    ds->d4.behind.gpio.port = S8_GPIO_Port;
    ds->d4.front.gpio.pin  = S7_Pin;
    ds->d4.front.gpio.port = S7_GPIO_Port;

    uint8_t switch_cnt = sizeof(drawer_switches_t)/sizeof(drawer_switch_t);
    drawer_switch_t *dws_ptr = &ds->d1;

    for (size_t i = 0; i < switch_cnt; i++)
    {
        time_event_stop(&dws_ptr->behind.debounce);
        time_event_stop(&dws_ptr->front.debounce);
        dws_ptr->behind.pressed = false;
        dws_ptr->front.pressed = false;
        dws_ptr->st = DRAWER_SWITCH_ST_INVALID;
        dws_ptr++;
    }

}

static void poll_switch_state_changes(drawer_switches_t *ds)
{
    /*update debounce time for all switches */
    uint8_t switch_cnt = sizeof(drawer_switches_t)/sizeof(drawer_switch_t);
    drawer_switch_t *dws_ptr = &ds->d1;
    static GPIO_PinState pin_st_front[4]  = {GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET};
    static GPIO_PinState pin_st_behind[4] = {GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET};

    for (size_t i = 0; i < switch_cnt; i++)
    {

        if (HAL_GPIO_ReadPin(dws_ptr->behind.gpio.port, dws_ptr->front.gpio.pin) != pin_st_front[i])
        {
            pin_st_front[i] = HAL_GPIO_ReadPin(dws_ptr->behind.gpio.port, dws_ptr->front.gpio.pin);
            if(!time_event_is_active(&dws_ptr->behind.debounce))
                time_event_start(&dws_ptr->behind.debounce, SWITCH_DEBOUNCE_TIME_MS);
        }

        if (HAL_GPIO_ReadPin(dws_ptr->front.gpio.port, dws_ptr->front.gpio.pin) != pin_st_front[i])
        {
            pin_st_front[i] = HAL_GPIO_ReadPin(dws_ptr->front.gpio.port, dws_ptr->front.gpio.pin);
            if(!time_event_is_active(&dws_ptr->front.debounce))
                time_event_start(&dws_ptr->front.debounce, SWITCH_DEBOUNCE_TIME_MS);
        }

        dws_ptr++;
    }
}

void switch_debounce_update(drawer_switches_t *ds)
{
    uint8_t switch_cnt = sizeof(drawer_switches_t)/sizeof(drawer_switch_t);
    drawer_switch_t *dws_ptr = &ds->d1;

    for (size_t i = 0; i < switch_cnt; i++)
    {
        time_event_update(&dws_ptr->behind.debounce);
        time_event_update(&dws_ptr->front.debounce);

        if(time_event_is_raised(&dws_ptr->behind.debounce))
        {
            if(HAL_GPIO_ReadPin(dws_ptr->behind.gpio.port, dws_ptr->behind.gpio.pin) == GPIO_PIN_RESET)
                dws_ptr->behind.pressed= true;
            
            time_event_stop(&dws_ptr->behind.debounce);
        }

        if(time_event_is_raised(&dws_ptr->front.debounce))
        {
            if(HAL_GPIO_ReadPin(dws_ptr->front.gpio.port, dws_ptr->front.gpio.pin) == GPIO_PIN_RESET)
                dws_ptr->front.pressed = true;
            
            time_event_stop(&dws_ptr->front.debounce);
        }
        dws_ptr++;
    }
}

void switch_state_update(drawer_switches_t *ds)
{
    uint8_t switch_cnt = sizeof(drawer_switches_t)/sizeof(drawer_switch_t);
    drawer_switch_t *dws_ptr = &ds->d1;

    for (size_t i = 0; i < switch_cnt; i++)
    {
        if(dws_ptr->behind.pressed == false && dws_ptr->front.pressed == true)
        {
            dws_ptr->st = DRAWER_SWITCH_ST_OPEN;
            printf("drawer [%d] OPEN\r\n", i + 1);
        }

        if(dws_ptr->behind.pressed == true && dws_ptr->front.pressed == false)
        {
            dws_ptr->st = DRAWER_SWITCH_ST_CLOSE;
            printf("drawer [%d] CLOSED\r\n", i + 1);
        }

        if(dws_ptr->behind.pressed == true && dws_ptr->front.pressed == true)
        {
            dws_ptr->st = DRAWER_SWITCH_ST_INVALID;
            printf("drawer switch HIGH, movement in progress..\r\n");
        }

        dws_ptr++;
    }
}

void switch_pressed_clear(drawer_switches_t *ds)
{
    uint8_t switch_cnt = sizeof(drawer_switches_t)/sizeof(drawer_switch_t);
    drawer_switch_t *dws_ptr = &ds->d1;

    for (size_t i = 0; i < switch_cnt; i++)
    {
        dws_ptr->behind.pressed = false;
        dws_ptr->front.pressed = false;
        dws_ptr++;
    }
}



void drawer_switch_run(drawer_switches_t *ds)
{
    /*Poll Switch changes */
    poll_switch_state_changes(ds);

    /*update debounce time for all switches */
    switch_debounce_update(ds);

    /*update switch state*/
    switch_state_update(ds);

    /*clear activated switch*/
    switch_pressed_clear(ds);
}


bool is_drawer_open(uint8_t drawer_no)
{
    drawer_switch_t *dws_ptr = &drawer_switches.d1;

    for (size_t i = 0; i < drawer_no; i++)
    {
        dws_ptr++;
    } 

    if(dws_ptr->st == DRAWER_SWITCH_ST_OPEN)
        return  true;
}

bool is_drawer_close(uint8_t drawer_no)
{
    drawer_switch_t *dws_ptr = &drawer_switches.d1;

    for (size_t i = 0; i < drawer_no; i++)
    {
        dws_ptr++;
    }

    if(dws_ptr->st == DRAWER_SWITCH_ST_CLOSE)
        return  true;
}

