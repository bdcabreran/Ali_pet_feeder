#include "buttons.h"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch (GPIO_Pin)
    {
        /* Navigation Buttons */

    // enter (B0)
    case GPIO_PIN_0:
    {
        if (!time_event_is_active(&navigation_btn.enter.debounce))
            time_event_start(&navigation_btn.enter.debounce, DEBOUNCE_TIME_MS);
    }
    break;

    // up (C4)
    case GPIO_PIN_4:
    {
        if (!time_event_is_active(&navigation_btn.up.debounce))
            time_event_start(&navigation_btn.up.debounce, DEBOUNCE_TIME_MS);
    }
    break;

    // left (A5)
    case GPIO_PIN_5:
    {
        if (!time_event_is_active(&navigation_btn.left.debounce))
            time_event_start(&navigation_btn.left.debounce, DEBOUNCE_TIME_MS);
    }
    break;

    // right (A7)
    case GPIO_PIN_7:
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
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);   // enter -> B0
}

void EXTI4_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);   // up -> C4
}

void EXTI9_5_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);   //Left (A5)
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);   //Right(A7)
}
