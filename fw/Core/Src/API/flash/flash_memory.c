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

#define FLASH_SECTOR_USER_DATA 	(127) 
#define FLASH_SECTOR_SIZE       (0x400)

/*addr = 0x801FC00*/
#define FLASH_ADDRESS_USER_DATA ((uint32_t)FLASH_BASE + (FLASH_PAGE_SIZE * FLASH_SECTOR_USER_DATA))

uint32_t flash_memory_write(uint32_t *data, uint16_t numberofwords)
{
	flash_write_data(FLASH_ADDRESS_USER_DATA, data, numberofwords);
}

void flash_memory_read(uint32_t *out_buffer, uint16_t numberofwords)
{
	flash_read_data(FLASH_ADDRESS_USER_DATA, out_buffer, numberofwords);
}

uint32_t GetPage(uint32_t page_number)
{
	for (int indx = 0; indx < 128; indx++)
	{
		if ((page_number < (0x08000000 + (1024 * (indx + 1)))) && (page_number >= (0x08000000 + 1024 * indx)))
		{
			return (0x08000000 + 1024 * indx);
		}
	}

  return -1;
}

static uint32_t flash_erase_page(uint32_t page_address)
{
	FLASH_EraseInitTypeDef flash_erase;
    uint32_t page_status;
    flash_erase.TypeErase = FLASH_TYPEERASE_PAGES;
    flash_erase.PageAddress = FLASH_ADDRESS_USER_DATA;
    flash_erase.NbPages = 1;

    // __disable_irq();  
	HAL_FLASH_Unlock();
	if (HAL_FLASHEx_Erase(&flash_erase, &page_status) != HAL_OK)
		Error_Handler();
	HAL_FLASH_Lock();
	// __enable_irq();
}

uint32_t flash_write_data(uint32_t StartSectorAddress, uint32_t *Data, uint16_t numberofwords)
{
	int word_index = 0;
	uint32_t start_addr = StartSectorAddress;
	flash_erase_page(StartSectorAddress);
	
	HAL_FLASH_Unlock();

	while (word_index < numberofwords)
	{
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, start_addr, (uint32_t)Data[word_index]) == HAL_OK)
		{
			start_addr += 4; // use StartPageAddress += 2 for half word and 8 for double word
			word_index++;
		}
		else
		{
			Error_Handler();
		}
	}

	HAL_FLASH_Lock();

	return 0;
}

void flash_read_data(uint32_t StartSectorAddress, uint32_t *RxBuf, uint16_t numberofwords)
{
	while (1)
	{
		*RxBuf = *(__IO uint32_t *)StartSectorAddress;
		StartSectorAddress += 4;
		RxBuf++;
		if (!(numberofwords--))
			break;
	}
}
