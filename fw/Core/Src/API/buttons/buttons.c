#include "buttons.h"
#include "printf_dbg.h"
#include "event_manager_fsm.h"

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

static btn_ev_ext_t btn_key_enter_combination(navigation_btn_t *btn)
{
    if (btn->down.pressed)
    {
        if(HAL_GPIO_ReadPin(btn->enter.gpio.port, btn->enter.gpio.pin) == GPIO_PIN_RESET)
        {
            if(time_event_is_active(&btn->enter.debounce))
                time_event_stop(&btn->enter.debounce);

            printf("btn down + enter pressed\r\n");
            return EVT_EXT_BTN_DOWN_AND_ENTER_PRESSED;
        }
        else
        {
            printf("btn down pressed\r\n");
            return EVT_EXT_BTN_DOWN_PRESSED;
        }
    }

    if (btn->up.pressed)
    {
        if (HAL_GPIO_ReadPin(btn->enter.gpio.port, btn->enter.gpio.pin) == GPIO_PIN_RESET)
        {
            if (time_event_is_active(&btn->enter.debounce))
                time_event_stop(&btn->enter.debounce);

            printf("btn up + enter pressed\r\n");
            return EVT_EXT_BTN_UP_AND_ENTER_PRESSED;
        }
        else
        {
            printf("btn up pressed\r\n");
            return EVT_EXT_BTN_UP_PRESSED;
        }
    }

    if (btn->left.pressed)
    {
        if(HAL_GPIO_ReadPin(btn->enter.gpio.port, btn->enter.gpio.pin) == GPIO_PIN_RESET)
        {
            if (time_event_is_active(&btn->enter.debounce))
                time_event_stop(&btn->enter.debounce);
            
            printf("btn left + enter pressed\r\n");
            return EVT_EXT_BTN_LEFT_AND_ENTER_PRESSED;
        }
        else 
        {
            printf("btn left pressed\r\n");
            return EVT_EXT_BTN_LEFT_PRESSED;
        }
    }

    if (btn->right.pressed)
    {
        if(HAL_GPIO_ReadPin(btn->enter.gpio.port, btn->enter.gpio.pin) == GPIO_PIN_RESET)
        {
            if (time_event_is_active(&btn->enter.debounce))
                time_event_stop(&btn->enter.debounce);
            
            printf("btn right + enter pressed\r\n");
            return EVT_EXT_BTN_RIGHT_AND_ENTER_PRESSED;
        }
        else
        {
            printf("btn right pressed\r\n");
            return EVT_EXT_BTN_RIGHT_PRESSED;
        }
    }

    return EVT_EXT_BTN_INVALID;
}


static btn_ev_ext_t btn_enter_key_combination(navigation_btn_t *btn)
{
    if (btn->enter.pressed)
    {
        if (HAL_GPIO_ReadPin(btn->down.gpio.port, btn->down.gpio.pin) == GPIO_PIN_RESET)
        {
            if (time_event_is_active(&btn->down.debounce))
                time_event_stop(&btn->down.debounce);
            printf("btn enter + down pressed\r\n");
            return EVT_EXT_BTN_DOWN_AND_ENTER_PRESSED;
        }

        else if (HAL_GPIO_ReadPin(btn->up.gpio.port, btn->up.gpio.pin) == GPIO_PIN_RESET)
        {
            if (time_event_is_active(&btn->up.debounce))
                time_event_stop(&btn->up.debounce);
            printf("btn enter + up pressed\r\n");
            return EVT_EXT_BTN_UP_AND_ENTER_PRESSED;
        }

        else if (HAL_GPIO_ReadPin(btn->left.gpio.port, btn->left.gpio.pin) == GPIO_PIN_RESET)
        {
            if (time_event_is_active(&btn->left.debounce))
                time_event_stop(&btn->left.debounce);
            printf("btn enter + left pressed\r\n");
            return EVT_EXT_BTN_LEFT_AND_ENTER_PRESSED;
        }

        else if (HAL_GPIO_ReadPin(btn->right.gpio.port, btn->right.gpio.pin) == GPIO_PIN_RESET)
        {
            if (time_event_is_active(&btn->right.debounce))
                time_event_stop(&btn->right.debounce);
            printf("btn enter + right pressed\r\n");
            return EVT_EXT_BTN_RIGHT_AND_ENTER_PRESSED;
        }
        else
        {
            printf("btn enter pressed\r\n");
            return EVT_EXT_BTN_ENTER_PRESSED;
        }
    }
    return EVT_EXT_BTN_INVALID;
}

static void poll_down_key_pressed(navigation_btn_t *btn)
{
    static GPIO_PinState pin_st = GPIO_PIN_SET;
    if (HAL_GPIO_ReadPin(DOWN_GPIO_Port, DOWN_Pin) != pin_st)
    {
        pin_st = HAL_GPIO_ReadPin(DOWN_GPIO_Port, DOWN_Pin);
        if(!time_event_is_active(&btn->down.debounce))
            time_event_start(&navigation_btn.down.debounce, DEBOUNCE_TIME_MS);
    }
}

void btn_debounce_run(navigation_btn_t *btn)
{
    /*down button cannot be set IT mode, we have to do polling*/
    poll_down_key_pressed(btn);

    /*update debounce time for all buttons */
    uint8_t btn_cnt = sizeof(navigation_btn_t)/sizeof(push_button_t);
    push_button_t *btn_ptr = &btn->up;

    for (size_t i = 0; i < btn_cnt; i++)
    {
        time_event_update(&btn_ptr->debounce);
        if(time_event_is_raised(&btn_ptr->debounce))
        {
            if(HAL_GPIO_ReadPin(btn_ptr->gpio.port, btn_ptr->gpio.pin) == GPIO_PIN_RESET)
                btn_ptr->pressed = true;
            
            time_event_stop(&btn_ptr->debounce);
        }
        btn_ptr++;
    }

    /*Notify events*/
    event_t event;
    event.name = EVT_EXT_BTN_INVALID;
    event.fsm.src = BTN_FSM;
    event.fsm.dst = UI_FSM;
    event.data.len = 0;

    event.name = btn_key_enter_combination(btn);
    if(event.name != EVT_EXT_BTN_INVALID)
        event_manager_write(event_manager_fsm_get(), &event);

    event.name = btn_enter_key_combination(btn);
    if(event.name != EVT_EXT_BTN_INVALID)
        event_manager_write(event_manager_fsm_get(), &event);

    btn_ptr = &btn->up;
    for (size_t i = 0; i < btn_cnt; i++)
    {
        btn_ptr->pressed = false;
        btn_ptr++;
    }
}

