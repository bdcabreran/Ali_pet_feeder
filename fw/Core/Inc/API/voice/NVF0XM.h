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
/*
	Press record  for 8 seconds
	Press 1 second to delete
	Press 1 second to play

*/
//just need to leave that as input or ouput
#include "stm32f1xx_hal.h"

#define NV04_PINS_USED 3
#define	NV04_RECORDING_STOP_GPIO			GPIOB
#define	NV04_RECORDING_STOP_PIN				GPIO_PIN_3
#define	NV04_PLAY_GPIO						GPIOB
#define	NV04_PLAY_PIN						GPIO_PIN_4
#define	NV04_DELETE_GPIO			        GPIOD
#define	NV04_DELETE_PIN				        GPIO_PIN_2

#define	NV04_RECORDING_PRESS_TIME_MS 300		
#define	NV04_PLAY_PRESS_TIME_MS	     1000		
#define NV04_DELETE_PRESS_TIME_MS    1000
#define NV04_RECORD_PRESS_TIME_MS    8000



void NVF04_init(void);
void NVF04_play_recording(void);
void NVF04_stop_recording(void);
void NVF04_record(void);
void NVF04_delete_recording(void);