#include "buttons.h"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch (GPIO_Pin)
    {
    /* Navigation Buttons */
    // up (B0)
    case GPIO_PIN_0:
    {
        if (!time_event_is_active(&navigation_btn.up.debounce))
            time_event_start(&navigation_btn.up.debounce, DEBOUNCE_TIME_MS);
    }
    break;

    // down (B1)
    case GPIO_PIN_1:
    {
        if (!time_event_is_active(&navigation_btn.down.debounce))
            time_event_start(&navigation_btn.down.debounce, DEBOUNCE_TIME_MS);
    }

    // enter (B2)
    case GPIO_PIN_2:
    {
        if (!time_event_is_active(&navigation_btn.enter.debounce))
            time_event_start(&navigation_btn.enter.debounce, DEBOUNCE_TIME_MS);
    }
    break;


    // left (C4)
    case GPIO_PIN_4:
    {
        if (!time_event_is_active(&navigation_btn.left.debounce))
            time_event_start(&navigation_btn.left.debounce, DEBOUNCE_TIME_MS);
    }
    break;

    // right (C5)
    case GPIO_PIN_5:
    {
        if (!time_event_is_active(&navigation_btn.right.debounce))
            time_event_start(&navigation_btn.right.debounce, DEBOUNCE_TIME_MS);
    }
    break;

    default:
        break;
    }

}


void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);   // up -> B0
}

void EXTI1_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);   // down -> B1
}

void EXTI2_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);   // enter -> B2
}

void EXTI4_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);   // left -> C4
}

void EXTI9_5_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);   // right -> C5
}
