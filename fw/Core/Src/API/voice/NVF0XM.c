/**
 * @file NVF0XM.c
 * @author Mauro R (emauriciorg@gmail.com)
 * @brief Voice ic driver
 * @version 0.1
 * @date 2022-06-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "NVF0XM.h"
#include "stm32f1xx_hal.h"


void NVF04_init(void) {
	/*TODO : test if we need them as input or ouput high to avoid triggering*/
	GPIO_InitTypeDef	gpinit;
	gpinit.Mode = GPIO_MODE_OUTPUT_OD;
	gpinit.Pull = GPIO_NOPULL;
	gpinit.Speed = GPIO_SPEED_FREQ_HIGH;
	gpinit.Pin = NV04_RECORDING_STOP_PIN;
	
	HAL_GPIO_Init(NV04_GPIO,&gpinit);
	gpinit.Pin = NV04_PLAY_PIN;
	HAL_GPIO_Init(NV04_GPIO,&gpinit);

}

void NVF04_play_recording(void) {
	NV04_GPIO->BRR |=NV04_RECORDING_STOP_PIN;
	HAL_Delay(NV04_RECORDING_PRESS_TIME_MS);
	NV04_GPIO->BSRR |=NV04_RECORDING_STOP_PIN;
}

void NVF04_stop_recording(void) {
	NV04_GPIO->BRR |=NV04_PLAY_PIN;
	HAL_Delay(NV04_PLAY_PRESS_TIME_MS);
	NV04_GPIO->BSRR |=NV04_PLAY_PIN;
}

void NVF04_delete_recording(void) {

	/*TODO: to be implemented */
}