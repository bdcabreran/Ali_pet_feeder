#include "buttons.h"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch (GPIO_Pin)
    {
        /* Navigation Buttons */

        // enter (B0)
        case GPIO_PIN_0: {
            time_event_start(&navigation_btn.enter.debounce, DEBOUNCE_TIME_MS);
        }break;

        // up (C4)
        case GPIO_PIN_4: { 
            time_event_start(&navigation_btn.up.debounce, DEBOUNCE_TIME_MS);
        }break;

        // left (A5) - down (C5)
        case GPIO_PIN_5: {
            if(HAL_GPIO_ReadPin(LEFT_GPIO_Port, LEFT_Pin) == GPIO_PIN_RESET)
                time_event_start(&navigation_btn.left.debounce, DEBOUNCE_TIME_MS);
            
            if (HAL_GPIO_ReadPin(DOWN_GPIO_Port, DOWN_Pin) == GPIO_PIN_RESET)
                  time_event_start(&navigation_btn.down.debounce, DEBOUNCE_TIME_MS);
         }break;

        // right (A7)
        case GPIO_PIN_7: {
            time_event_start(&navigation_btn.right.debounce, DEBOUNCE_TIME_MS);
         }break;
    
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
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);   //Left (A5), Down (C5)
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);   //Right(A7)
}