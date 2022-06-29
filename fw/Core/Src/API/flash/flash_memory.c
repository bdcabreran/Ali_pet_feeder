/**
 * @file flash_memory.c
 * @author Mauro R (emauriciorg@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-06-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdbool.h>
#include "stm32f1xx_hal.h"
#include "flash_memory.h"
#define FLASH_SECTOR_FOR_USER_DATA 63 // Needs to be modified to match page address?
#define FLASH_BASE_ADDRESS 0x08007C00  // - 0x0800 7FFF



uint32_t flash_memory_write(uint32_t *data, uint16_t numberofwords) {
	flash_Write_Data ( FLASH_BASE_ADDRESS, data, numberofwords);
	//flash_Write_Data ( GetPage(FLASH_SECTOR_FOR_USER_DATA), data, numberofwords);
}

void flash_memory_read(uint32_t *out_buffer, uint16_t numberofwords) {
	flash_read_data (FLASH_BASE_ADDRESS, out_buffer, numberofwords);
}

uint32_t GetPage(uint32_t page_number)
{
  for (int indx=0; indx<128; indx++)
  {
      if((page_number < (0x08000000 + (1024 *(indx+1))) ) && (page_number >= (0x08000000 + 1024*indx)))
      {
          return (0x08000000 + 1024*indx);
      }
  }

  return -1;
}

uint32_t flash_Write_Data (uint32_t StartSectorAddress, uint32_t *Data, uint16_t numberofwords)
{

	int word_index=0;


	HAL_FLASH_Unlock();

	FLASH_PageErase(StartSectorAddress);

	  while (word_index < numberofwords)
	   {
	     if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, StartSectorAddress, Data[word_index]) == HAL_OK)
	     {
	    	 StartSectorAddress += 4;  // use StartPageAddress += 2 for half word and 8 for double word
	    	 word_index++;
	     }
	   }

	  HAL_FLASH_Lock();

	   return 0;
}



void flash_read_data (uint32_t StartSectorAddress, uint32_t *RxBuf, uint16_t numberofwords)
{
	while (1)
	{
		*RxBuf = *(__IO uint32_t *)StartSectorAddress;
		StartSectorAddress += 4;
		RxBuf++;
		if (!(numberofwords--)) break;
	}
}
