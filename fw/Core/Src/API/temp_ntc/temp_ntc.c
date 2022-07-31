
#include "temp_ntc.h"
#include "init_periph.h"
#include "ntc_table.h"
#include "filter_sma.h"

/*
https://cxemka.com/63-podklyuchenie-ntc-termistora-k-stm32-izmerenie-temperatury.html
*/

void temp_ntc_init(void)
{
    for (size_t i = 0; i < FILTER_SMA_ORDER; i++)
        temp_ntc_read_celsius();
}


float temp_ntc_read_celsius(void)
{
    uint32_t adc_raw = 0;
    HAL_ADC_Start(&hadc2);
	HAL_ADC_PollForConversion(&hadc2, HAL_MAX_DELAY);
	adc_raw = HAL_ADC_GetValue(&hadc2);
	HAL_ADC_Stop(&hadc2);

    float tmp_raw = calc_temperature(adc_raw);
    float ntc_temp = 0.1*tmp_raw;
    // printf("temperature , adc [%d], tmp [%d], sma [%d]\r\n", adc_raw, (int)ntc_temp, (int)Filter_SMA(ntc_temp));

    return Filter_SMA(ntc_temp);
}



