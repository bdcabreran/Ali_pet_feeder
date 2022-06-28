/**
 * @file NVF0XM.h
 * @author Mauro R (emauriciorg@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-06-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */

//just need to leave that as input or ouput
#include "stm32f1xx_hal.h"

#define	NV04_GPIO							GPIOC
#define	NV04_RECORDING_STOP_PIN				GPIO_PIN_2
#define	NV04_PLAY_PIN						GPIO_PIN_3

#define	NV04_RECORDING_PRESS_TIME_MS 300		
#define	NV04_PLAY_PRESS_TIME_MS	     300		


void NVF04_init(void);
void NVF04_play_recording(void);
void NVF04_stop_recording(void);
void NVF04_delete_recording(void);