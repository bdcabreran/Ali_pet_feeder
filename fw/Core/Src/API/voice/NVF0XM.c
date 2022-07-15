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
#include <stdint.h>

typedef struct nv_pins_st {
	uint32_t pin;
	GPIO_TypeDef  *gpio; 
}nvf_pins_t;

enum nvf_action{
	NV_PLAY,
	NV_DELETE,
	NV_RECORD,
};

nvf_pins_t nvf_pins[NV04_PINS_USED]={
	{NV04_PLAY_PIN           , NV04_PLAY_GPIO},
	{NV04_DELETE_PIN         , NV04_DELETE_GPIO},
	{NV04_RECORDING_STOP_PIN , NV04_RECORDING_STOP_GPIO}
};

void NVF04_init(void) {
	/*TODO : test if we need them as input or ouput high to avoid triggering*/
	GPIO_InitTypeDef	gpinit;
	gpinit.Mode = GPIO_MODE_OUTPUT_OD;
	gpinit.Pull = GPIO_NOPULL;
	gpinit.Speed = GPIO_SPEED_FREQ_HIGH;
	
	for (uint8_t pins =0; pins<NV04_PINS_USED; pins++){
		gpinit.Pin = nvf_pins[pins].pin;
		HAL_GPIO_Init(nvf_pins[pins].gpio,&gpinit);
	}
	


}

void NVF04_play_recording(void) {
	NV04_PLAY_GPIO->BRR |=NV04_PLAY_PIN;
	HAL_Delay(NV04_RECORDING_PRESS_TIME_MS);
	NV04_PLAY_GPIO->BSRR |=NV04_PLAY_PIN;
}

void NVF04_record(void) {
	NV04_RECORDING_STOP_GPIO->BRR |=NV04_RECORDING_STOP_PIN;
	HAL_Delay(NV04_RECORD_PRESS_TIME_MS);
	NV04_RECORDING_STOP_GPIO->BSRR |=NV04_RECORDING_STOP_PIN;
}

void NVF04_delete_recording(void) {

	NV04_DELETE_GPIO->BRR |=NV04_DELETE_PIN;
	HAL_Delay(NV04_DELETE_PRESS_TIME_MS);
	NV04_DELETE_GPIO->BSRR |=NV04_DELETE_PIN;
}
