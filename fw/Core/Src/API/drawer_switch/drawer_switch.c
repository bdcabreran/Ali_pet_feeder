

#include "drawer_switch.h"
#include "printf_dbg.h"

#define SWITCH_DEBOUNCE_TIME_MS (30)


drawer_switches_t drawer_switches;


void drawer_switch_init(drawer_switches_t *ds)
{
    ds->d_sw[0].behind.gpio.pin  = S2_Pin;
    ds->d_sw[0].behind.gpio.port = S2_GPIO_Port;
    ds->d_sw[0].front.gpio.pin  = S1_Pin;
    ds->d_sw[0].front.gpio.port = S1_GPIO_Port;

    ds->d_sw[1].behind.gpio.pin  = S4_Pin;
    ds->d_sw[1].behind.gpio.port = S4_GPIO_Port;
    ds->d_sw[1].front.gpio.pin  = S3_Pin;
    ds->d_sw[1].front.gpio.port = S3_GPIO_Port;

    ds->d_sw[2].behind.gpio.pin  = S6_Pin;
    ds->d_sw[2].behind.gpio.port = S6_GPIO_Port;
    ds->d_sw[2].front.gpio.pin  = S5_Pin;
    ds->d_sw[2].front.gpio.port = S5_GPIO_Port;

    ds->d_sw[3].behind.gpio.pin  = S8_Pin;
    ds->d_sw[3].behind.gpio.port = S8_GPIO_Port;
    ds->d_sw[3].front.gpio.pin  = S7_Pin;
    ds->d_sw[3].front.gpio.port = S7_GPIO_Port;

    uint8_t switch_cnt = sizeof(drawer_switches_t)/sizeof(drawer_switch_t);

    for (size_t drawer_sw_idx = 0; drawer_sw_idx < DRAWER_NO; drawer_sw_idx++)
    {
        ds->d_sw[drawer_sw_idx].behind.pressed = false;
        ds->d_sw[drawer_sw_idx].front.pressed = false;
        ds->d_sw[drawer_sw_idx].st = DRAWER_SWITCH_ST_INVALID;
    }

}

void switch_state_update(drawer_switches_t *ds)
{
    for (size_t drawer_idx = 0; drawer_idx < DRAWER_NO; drawer_idx++)
    {
        drawer_switch_t *ds_ptr = &ds->d_sw[drawer_idx];

        /* check if front switch is pressed */
        if (HAL_GPIO_ReadPin(ds_ptr->front.gpio.port, ds_ptr->front.gpio.pin) == GPIO_PIN_RESET)
        {
            ds_ptr->front.pressed = true;
        }
        else
        {
            ds_ptr->front.pressed = false;
        }
        
        /*check if behind switch is pressed */
        if (HAL_GPIO_ReadPin(ds_ptr->behind.gpio.port, ds_ptr->behind.gpio.pin) == GPIO_PIN_RESET)
        {
            ds_ptr->behind.pressed = true;
        }
        else
        {
            ds_ptr->behind.pressed = false;
        }
    
        /*update switch status*/
        if(ds_ptr->behind.pressed == false && ds_ptr->front.pressed == true)
        {
            ds_ptr->st = DRAWER_SWITCH_ST_OPEN;
            // printf("drawer [%d] OPEN\r\n", i + 1);
        }

        if(ds_ptr->behind.pressed == true && ds_ptr->front.pressed == false)
        {
            ds_ptr->st = DRAWER_SWITCH_ST_CLOSE;
            // printf("drawer [%d] CLOSED\r\n", i + 1);
        }

        if(ds_ptr->behind.pressed == false && ds_ptr->front.pressed == false)
        {
            ds_ptr->st = DRAWER_SWITCH_ST_INVALID;
            // printf("drawer switch HIGH, movement in progress..\r\n");
        }
    }
}

void drawer_switch_run(drawer_switches_t *ds)
{
    /*update switch state*/
    switch_state_update(ds);
}


bool is_drawer_open(uint8_t drawer_no)
{
    drawer_switch_t *dws_ptr = &drawer_switches.d_sw[drawer_no];
    if(dws_ptr->st == DRAWER_SWITCH_ST_OPEN)
        return  true;
    return false;
}

bool is_drawer_close(uint8_t drawer_no)
{
    drawer_switch_t *dws_ptr = &drawer_switches.d_sw[drawer_no];
    if(dws_ptr->st == DRAWER_SWITCH_ST_CLOSE)
        return  true;
    return false;
}

