/**
 * @file rtc_1307.c
 * @author Mauro R (emauriciorg@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-06-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "init_periph.h"
#include "rtc_1307.h"
#include "stm32f1xx_hal.h"
#include <stdbool.h>
#include <stdint.h>

extern I2C_HandleTypeDef hi2c1;

void init_ds1307(void){

}


bool setTime(uint8_t sec, uint8_t min, uint8_t hour, uint8_t weekday, uint8_t date, uint8_t month, uint16_t year);
//bool setTime(uint8_t *time, uint8_t len);
bool setSeconds(uint8_t value);
bool setMinutes(uint8_t value);
bool setHours(uint8_t value);
bool setWeekday(uint8_t value);
bool setDate(uint8_t value);
bool setMonth(uint8_t value);
bool setYear(uint16_t value);
bool updateTime(); // Update the local array with the RTC registers




	// Values in RTC are stored in Binary Coded Decimal. These functions convert to/from Decimal
	uint8_t BCDtoDEC(uint8_t val);
	uint8_t DECtoBCD(uint8_t val);

uint8_t readRegister(uint8_t addr){
	const uint8_t single_byte_read  = 1;
	uint8_t out_data = 0;
 	HAL_I2C_Mem_Read(&hi2c1, DS1307_ADDR, addr, single_byte_read, &out_data, single_byte_read, 100);
	return out_data;
}

bool writeRegister(uint8_t addr, uint8_t val){
	const uint8_t single_byte_read  = 1;
	uint8_t out_data = 0;
 	HAL_I2C_Mem_Write(&hi2c1, DS1307_ADDR, addr, single_byte_read, &val, single_byte_read, 100);
	return out_data;

}

bool readMultipleRegisters(uint8_t addr, uint8_t *dest, uint8_t len)
{
//	 	HAL_I2C_Mem_Write(&hi2c1, DS1307_ADDR, addr, single_byte_read, &val, single_byte_read, 100);

}

bool writeMultipleRegisters(uint8_t addr, uint8_t *values, uint8_t len)
{
// 	HAL_I2C_Mem_Write(&hi2c1, DS1307_ADDR, addr, single_byte_read, &val, single_byte_read, 100);


}

bool writeConfigEEPROM_RAMmirror(uint8_t eepromaddr, uint8_t val);
uint8_t readConfigEEPROM_RAMmirror(uint8_t eepromaddr);
bool waitforEEPROM();
void reset();

void setBit(uint8_t reg_addr, uint8_t bit_num);
void clearBit(uint8_t reg_addr, uint8_t bit_num);
bool readBit(uint8_t reg_addr, uint8_t bit_num);



