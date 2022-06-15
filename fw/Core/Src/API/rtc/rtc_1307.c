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

#define DS1307_WRITE_REG(...)

#define DS1307_READ_REG(...)

uint8_t second;
uint8_t minute;
uint8_t hour;
uint8_t dayOfWeek;// day of week, 1 = Monday
uint8_t dayOfMonth;
uint8_t month;
uint16_t year;


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



uint8_t decToBcd(uint8_t val) {
    return ((val / 10 * 16) + (val % 10));
}

//Convert binary coded decimal to normal decimal numbers
uint8_t bcdToDec(uint8_t val) {
    return ((val / 16 * 10) + (val % 16));
}


/*Function: The clock timing will start */
void startClock(void) {      // set the ClockHalt bit low to start the rtc
    DS1307_WRITE_REG((uint8_t)0x00);                      // Register 0x00 holds the oscillator start/stop bit
    DS1307_READ_REG(1);
//    second = Wire.read() & 0x7f;       // save actual seconds and AND sec with bit 7 (sart/stop bit) = clock started
    DS1307_WRITE_REG((uint8_t)0x00,seconds);
    
}

/*Function: The clock timing will stop */
void stopClock(void) {       // set the ClockHalt bit high to stop the rtc
    DS1307_WRITE_REG((uint8_t)0x00);                      // Register 0x00 holds the oscillator start/stop bit
    //second = DS1307_READ_REG( 1) | 0x80;
    DS1307_WRITE_REG((uint8_t)0x00, seconds);
}
/****************************************************************/
/*Function: Read time and date from RTC */
void getTime() {
    // Reset the register pointer
    DS1307_WRITE_REG((uint8_t)0x00);
    DS1307_READ_REG( 7);
    // A few of these need masks because certain bits are control bits
#if 0
	/** TODO:convert to multiple reg read*/
    second     = bcdToDec(Wire.read() & 0x7f);
    minute     = bcdToDec(Wire.read());
    hour       = bcdToDec(Wire.read() & 0x3f);// Need to change this if 12 hour am/pm
    dayOfWeek  = bcdToDec(Wire.read());
    dayOfMonth = bcdToDec(Wire.read());
    month      = bcdToDec(Wire.read());
    year       = bcdToDec(Wire.read());
#endif
}
/*******************************************************************/
/*Frunction: Write the time that includes the date to the RTC chip */
void set_time() {
    DS1307_WRITE_REG((uint8_t)0x00);
    DS1307_WRITE_REG(decToBcd(second));// 0 to bit 7 starts the clock
    DS1307_WRITE_REG(decToBcd(minute));
    DS1307_WRITE_REG(decToBcd(hour));  // If you want 12 hour am/pm you need to set bit 6
    DS1307_WRITE_REG(decToBcd(dayOfWeek));
    DS1307_WRITE_REG(decToBcd(dayOfMonth));
    DS1307_WRITE_REG(decToBcd(month));
    DS1307_WRITE_REG(decToBcd(year));
}
void fillByHMS(uint8_t _hour, uint8_t _minute, uint8_t _second) {
    // assign variables
    hour   = _hour;
    minute = _minute;
    second = _second;
}
void fillByYMD(uint16_t _year, uint8_t _month, uint8_t _day) {
    year = _year - 2000;
    month = _month;
    dayOfMonth = _day;
}
void fillDayOfWeek(uint8_t _dow) {
    dayOfWeek = _dow;
}
