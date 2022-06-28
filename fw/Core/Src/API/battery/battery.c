/**
 * @file battery.c
 * @author Mauro Rios (emauriciorg@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-06-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */

//TODO : add dma readings , right now is not crucial

#include <stdint.h>
#include "battery.h"
#include "stm32f1xx_hal.h"

extern ADC_HandleTypeDef hadc1;

void read_adc(uint32_t *adc_sample_array);

static uint32_t convert_adc_to_millivolts(uint32_t adc_value)
{
	return (ADC_MAX_VOLTAGE_VDD * adc_value)/ 4096;
}


void battery_init(void){

	// just for standardization
	#ifdef ADC_CALIBRATION_ENABLE
	#warning "ADC calibration activated"
	ADC1->CR |= ADC_CR_ADCAL;
	while (	((ADC1->CR & (ADC_CR_ADCAL)) != 0));
	#endif

}

/**
 * @brief 
 * 
 * @return uint16_t millivolts , max value is 11100 , equivalent to 11.1v or BATTERY_VOLTAGE_MV_FULL 
 */
uint16_t battery_read_voltage(void) {
	uint32_t adc_array[2] = {0}; 
	uint16_t adc_mv;
	read_adc(adc_array);
	adc_mv = convert_adc_to_millivolts(adc_array[0]);
	return BATERY_VOLTAGE_CALC(adc_mv);
}





void read_adc(uint32_t *adc_sample_array)
{
	
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1,ADC_TIMEOUT);
	*adc_sample_array = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Stop(&hadc1);
}

