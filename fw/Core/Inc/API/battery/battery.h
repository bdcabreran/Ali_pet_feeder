/**
 * @file battery.h
 * @author Mauro Rios (emauriciorg@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-06-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdint.h>

#define ADC_MAX_VOLTAGE_VDD 3300
#define ADC_TIMEOUT 100
//#define ADC_CALIBRATION_ENABLE
#define BATTERY_VOLTAGE_MV_FULL 11100
#define BATTERY_R1 910000
#define BATTERY_R2 240000
#define BATTERY_RT (BATTERY_R1+BATTERY_R2)
#define BATTERY_R2_MAX_MV_VOLTAGE 2316
#define BATERY_VOLTAGE_CALC(voltage_r2) (voltage_r2*(BATTERY_RT/BATTERY_R2))


void battery_init(void);
uint16_t battery_read_voltage(void);
uint8_t battery_read_voltage_percent(void);
