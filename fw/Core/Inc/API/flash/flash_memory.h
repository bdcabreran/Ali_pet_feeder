/**
 * @file flash_memory.h
 * @author Mauro R (emauriciorg@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-06-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __FLASH_MEMORY_H__
#define __FLASH_MEMORY_H__

#include <stdint.h>


/*user routines */
uint32_t flash_memory_write(uint32_t *data, uint16_t numberofwords);
void flash_memory_read(uint32_t *out_buffer, uint16_t numberofwords);

uint32_t flash_write_data (uint32_t StartSectorAddress, uint32_t *Data, uint16_t numberofwords);
void flash_read_data (uint32_t StartSectorAddress, uint32_t *RxBuf, uint16_t numberofwords);

uint32_t GetPage(uint32_t page_number);






#endif /*__FLASH_MEMORY_H__ */