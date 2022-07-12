/**
 * @file drawer_switch.h
 * @author Bayron Cabrera (bdcabreran@unal.edu.co)
 * @brief Drawer Switch position library to identify whether a drawer is open or closed
 * @version 0.1
 * @date 2022-07-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#ifndef DRAWER_SWITCH_H
#define DRAWER_SWITCH_H

#include "stdint.h"
#include "time_event.h"
#include "init_periph.h"

typedef enum
{
    DRAWER_SWITCH_ST_INVALID,
    DRAWER_SWITCH_ST_OPEN,
    DRAWER_SWITCH_ST_CLOSE,
    DRAWER_SWITCH_ST_LAST,
}drawer_switch_st_t;

typedef struct
{
    gpio_port_pin_t gpio;
    time_event_t    debounce;
    bool            pressed;
}switch_t;

typedef struct 
{
    drawer_switch_st_t st;
    switch_t front;
    switch_t behind;
}drawer_switch_t;

typedef struct 
{
    drawer_switch_t d1;
    drawer_switch_t d2;
    drawer_switch_t d3;
    drawer_switch_t d4;
}drawer_switches_t;



extern drawer_switches_t drawer_switches;

void drawer_switch_init(drawer_switches_t *ds);
void drawer_switch_run(drawer_switches_t *ds);
bool is_drawer_open(uint8_t drawer_no);
bool is_drawer_close(uint8_t drawer_no);

#endif


