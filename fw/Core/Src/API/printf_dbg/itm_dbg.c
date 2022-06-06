
#include "itm_dbg.h"
#include "stdio.h"

void itm_enable(void)
{
	//Enable TRCENA
	DEMCR |= ( 1 << 24);

	//enable stimulus port 0
	ITM->TER |= ( 1 << 0);
}

void itm_send_char(uint8_t ch)
{
	// read FIFO status in bit [0]:
	while(!(ITM_STIMULUS_PORT0 & 1));

	//Write to ITM stimulus port0
	ITM_STIMULUS_PORT0 = ch;
}

int itm_write(char *ptr, int len)
{
	int DataIdx;
	for(DataIdx = 0 ; DataIdx < len; DataIdx++)
	{
		ITM_SendChar(*ptr++);
	}
	return len;
}



