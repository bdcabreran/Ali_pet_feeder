/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "init_periph.h"
#include "event_manager_fsm.h"
#include "buttons.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  init_peripherals();

  /*Init navigation button control */
  btn_debounce_init(&navigation_btn);

  /*Init Event Manager*/
  event_manager_handle_t evm_fsm = event_manager_fsm_get();
  event_manager_fsm_init(evm_fsm);

  /*Init UI */
  ui_handle_t ui_fsm = ui_fsm_get();
  ui_fsm_init(ui_fsm);

  printf("Ali Pet Feeder Project\r\n");

  /* Infinite loop */
  while (1)
  {
    ui_fsm_run(ui_fsm);
    event_manager_fsm_run(evm_fsm);
    time_events_poll_update();
  }
}



/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
}
#endif /* USE_FULL_ASSERT */
