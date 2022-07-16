#include "cooler.h"

void turn_on_cooler(void)
{
    HAL_GPIO_WritePin(POWER_CONTROL_GPIO_Port, POWER_CONTROL_Pin, GPIO_PIN_SET);
}

void turn_off_cooler(void)
{
    HAL_GPIO_WritePin(POWER_CONTROL_GPIO_Port, POWER_CONTROL_Pin, GPIO_PIN_RESET);
}
