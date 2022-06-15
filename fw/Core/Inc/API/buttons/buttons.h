#ifndef BUTTONS_H
#define BUTTONS_H

#include "init_periph.h"
#include "time_event.h"

#define DEBOUNCE_TIME_MS (50)
#define DEBOUNCE_TIME_DOWN_KEY_MS (200)

/**
 * @brief Internal Events 
 */
typedef enum
{
    EVT_EXT_BTN_INVALID = 0,
    
    EVT_EXT_BTN_UP_PRESSED,
    EVT_EXT_BTN_DOWN_PRESSED,
    EVT_EXT_BTN_LEFT_PRESSED,
    EVT_EXT_BTN_RIGHT_PRESSED,
    EVT_EXT_BTN_ENTER_PRESSED,

    EVT_EXT_BTN_UP_AND_ENTER_PRESSED,
    EVT_EXT_BTN_DOWN_AND_ENTER_PRESSED,
    EVT_EXT_BTN_LEFT_AND_ENTER_PRESSED,
    EVT_EXT_BTN_RIGHT_AND_ENTER_PRESSED,

    EVT_EXT_BTN_LAST
}btn_ev_ext_t;

typedef struct
{
    gpio_port_pin_t gpio;
    time_event_t    debounce;
    bool            pressed;
}push_button_t;

typedef struct
{
    push_button_t up;
    push_button_t down;
    push_button_t left;
    push_button_t right;
    push_button_t enter;
}navigation_btn_t;

#define IS_BTN_EVT_EXT(ev) (ev > EVT_EXT_BTN_INVALID && ev < EVT_EXT_BTN_LAST)

extern navigation_btn_t navigation_btn;

void btn_debounce_init(navigation_btn_t *btn);
void btn_debounce_run(navigation_btn_t *btn);


#endif
