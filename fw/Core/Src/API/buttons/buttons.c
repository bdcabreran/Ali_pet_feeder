#include "buttons.h"
#include "printf_dbg.h"

navigation_btn_t navigation_btn;

void btn_debounce_init(navigation_btn_t *btn)
{
    btn->down.gpio.port = DOWN_GPIO_Port;
    btn->down.gpio.pin  = DOWN_Pin;
    btn->up.gpio.port = UP_GPIO_Port;
    btn->up.gpio.pin = UP_Pin;
    btn->left.gpio.port = LEFT_GPIO_Port;
    btn->left.gpio.pin = LEFT_Pin;
    btn->right.gpio.port = RIGHT_GPIO_Port;
    btn->right.gpio.pin = RIGHT_Pin;
    btn->enter.gpio.port = ENTER_GPIO_Port;
    btn->enter.gpio.pin = ENTER_Pin;

    uint8_t btn_cnt = sizeof(navigation_btn_t)/sizeof(push_button_t);
    push_button_t *btn_ptr = &btn->up;

    for (size_t i = 0; i < btn_cnt; i++)
    {
        time_event_stop(&btn_ptr->debounce);
        btn_ptr->pressed = false;
        btn_ptr++;
    }
}

void btn_debounce_run(navigation_btn_t *btn)
{
    uint8_t btn_cnt = sizeof(navigation_btn_t)/sizeof(push_button_t);
    push_button_t *btn_ptr = &btn->up;

    for (size_t i = 0; i < btn_cnt; i++)
    {
        if(time_event_is_raised(&btn_ptr->debounce))
        {
            if(HAL_GPIO_ReadPin(btn_ptr->gpio.port, btn_ptr->gpio.pin) == GPIO_PIN_RESET)
                btn_ptr->pressed = true;
            
            time_event_stop(&btn_ptr->debounce);
        }
        btn_ptr++;
    }

    /*Notify events*/
    btn_ev_ext_t btn_ext_event = EVT_EXT_BTN_INVALID;

    if (btn->down.pressed)
    {
        if (btn->enter.pressed)
        {
            printf("btn down + enter pressed\r\n");
        }
        else
        {
            printf("btn down pressed\r\n");
        }
    }

    if (btn->up.pressed)
    {
        if (btn->enter.pressed)
        {
            printf("btn up + enter pressed\r\n");
        }
        else
        {
            printf("btn up pressed\r\n");
        }
    }

    if (btn->left.pressed)
    {
        if (btn->enter.pressed)
        {
            printf("btn left + enter pressed\r\n");
        }
        else 
        {
            printf("btn left pressed\r\n");
        }
    }

    if (btn->right.pressed)
    {
        if (btn->enter.pressed)
        {
            printf("btn right + enter pressed\r\n");

        }
        else
        {
            printf("btn right pressed\r\n");
        }
    }

    if (btn->enter.pressed)
    {
        printf("btn enter pressed\r\n");
    }
}

