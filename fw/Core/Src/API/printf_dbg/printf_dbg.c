#include "printf_dbg.h"
#include "stdio.h"

const char *str_color[E_LAST] = 
{
    "\x1B[31m",
    "\x1B[32m",
    "\x1B[33m",
    "\x1B[34m",
    "\x1B[35m",
    "\x1B[36m",
    "\x1B[37m",
    "\x1B[0m"
};

int _write(int file, char *ptr, int len)
{
#if USE_PRINTF_ITM
    /*write transmit function associated to a peripheral where you want to remap printf */
    return itm_write(ptr, len);
#endif

#if USE_PRINTF_UART2
    HAL_UART_Transmit(&huart2, ptr, len, HAL_MAX_DELAY);
    return len;
#endif

}

void print_buffer(uint8_t format, uint8_t *buff, uint8_t len)
{
	printf("buffer : \r\n");
	for (size_t i = 0; i < len; i++)
	{
		switch (format)
		{
		    case 'C': printf( "[%c] ", buff[i])  ; break;
		    case 'X': printf( "[0x%X] ", buff[i]); break;
		    default : printf( "[0x%X] ", buff[i]); break;
		}
	}
	printf("\r\n");
}

