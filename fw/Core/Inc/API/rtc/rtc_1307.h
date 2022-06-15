/**
 * @file rtc_1307.h
 * @author your name (you@domain.com)
 * @brief rtc low level driver
 * @version 0.1
 * @date 2022-06-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __DS1307_H__
#define __DS1307_H__

#include <stdbool.h>
#include <stdint.h>
//0x68
#define DS1307_ADDR 0xD0  /*8 BIT ADDRESS , first bit 'r/w*/

void init_ds1307(void);


// Values in RTC are stored in Binary Coded Decimal. These functions convert to/from Decimal
uint8_t BCDtoDEC(uint8_t val);
uint8_t DECtoBCD(uint8_t val);

uint8_t readRegister(uint8_t addr);
bool writeRegister(uint8_t addr, uint8_t val);
bool readMultipleRegisters(uint8_t addr, uint8_t *dest, uint8_t len);
bool writeMultipleRegisters(uint8_t addr, uint8_t *values, uint8_t len);

bool writeConfigEEPROM_RAMmirror(uint8_t eepromaddr, uint8_t val);
uint8_t readConfigEEPROM_RAMmirror(uint8_t eepromaddr);
bool waitforEEPROM();
void reset();

void setBit(uint8_t reg_addr, uint8_t bit_num);
void clearBit(uint8_t reg_addr, uint8_t bit_num);
bool readBit(uint8_t reg_addr, uint8_t bit_num);


typedef enum days_of_the_week_enum {
 	DAY_MON= 1,
 	DAY_TUE= 2,
 	DAY_WED= 3,
 	DAY_THU= 4,
 	DAY_FRI= 5,
 	DAY_SAT= 6,
 	DAY_SUN= 7
}days_of_the_week_t;

    uint8_t decToBcd(uint8_t val);
    uint8_t bcdToDec(uint8_t val);

    void startClock(void);
    void stopClock(void);
    void set_time(void);
    void getTime(void);
    void fillByHMS(uint8_t _hour, uint8_t _minute, uint8_t _second);
    void fillByYMD(uint16_t _year, uint8_t _month, uint8_t _day);
    void fillDayOfWeek(uint8_t _dow);
 

	// clock.fillByYMD(2013, 1, 19); //Jan 19,2013
    // clock.fillByHMS(15, 28, 30); //15:28 30"
    // clock.fillDayOfWeek(SAT);//Saturday
    // clock.setTime();//write time to the RTC chip

//    clock.getTime();

#endif /*__DS1307_H__*/
