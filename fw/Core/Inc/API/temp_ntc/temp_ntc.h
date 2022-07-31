/**
 * @file temp_ntc.h
 * @author Bayron Cabrera (bdcabreran@titoma.com)
 * @brief NTC library for temperature measurement via ADC 
 * @version 0.1
 * @date 2022-07-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef TEMP_NTC_H
#define TEMP_NTC_H

#include "stdint.h"
#include "stm32f1xx_hal.h"

void temp_ntc_init(void);
float temp_ntc_read_celsius(void);

#endif
