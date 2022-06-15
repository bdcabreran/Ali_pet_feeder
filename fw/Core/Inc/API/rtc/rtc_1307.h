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




#endif /*__DS1307_H__*/
