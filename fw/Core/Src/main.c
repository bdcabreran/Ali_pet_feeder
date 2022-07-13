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
#include "target_version.h"
#include "string.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

static void print_startup_msg(void)
{
  printf("#########################################\r\n");
  printf("Author :\t Embedded-Tech\r\n");
  printf("Project:\t pet-feeder v%s\r\n", FW_VERSION);
  printf("Updated:\t %s\r\n", __DATE__);
  printf("#########################################\r\n");
}

uint8_t GUI_Initialized = 0;
extern void MainTask(void);


/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /*Init peripherals */
  init_peripherals();

  /*startup info */
  print_startup_msg();

  /*Init navigation button control */
  btn_debounce_init(&navigation_btn);
  drawer_switch_init(&drawer_switches);

  /*Init Event Manager*/
  event_manager_handle_t evm_fsm = event_manager_fsm_get();
  event_manager_fsm_init(evm_fsm);

  /*Init UI */
  ui_handle_t ui_fsm = ui_fsm_get();
  ui_fsm_init(ui_fsm);

  /*Init Temperature Control */
  temp_ctrl_handle_t temp_fsm = temp_ctrl_fsm_get();
  temp_ctrl_fsm_init(temp_fsm);

  /*Init Feeder Scheduler */
  feeder_handle_t feeder_fsm = feeder_fsm_get();
  feeder_fsm_init(feeder_fsm);

  /*Init Drawer Controller */
  drawer_ctrl_handle_t drawer_fsm = drawer_ctrl_fsm_get();
  drawer_ctrl_fsm_init(drawer_fsm);

  /*Init Petcall */
  petcall_handle_t petcall_fsm = petcall_fsm_get();
  petcall_fsm_init(petcall_fsm);

  /*Init Power Ctrl*/
  power_handle_t power_fsm = power_fsm_get();
  power_fsm_init(power_fsm);

  /*System clock Speed */
  printf("System CLK speed [%ld]Hz\r\n", HAL_RCC_GetHCLKFreq());

  /* flash test */
  char data_in[] = "This is a message that i want to put into flash device";
  uint8_t data_in_len = strlen(data_in) / sizeof(uint32_t);
  printf("data len in words = [%d]\r\n", data_in_len);

  flash_memory_write(data_in, data_in_len);

  char data_out[100];
  flash_memory_read(data_out, data_in_len);
  uint8_t data_out_len = strlen(data_out) / sizeof(uint32_t);
  
  GUI_Init();

  GUI_Initialized = 1;
//  MainTask();



  /* Infinite loop */
  while (1)
  {
    /* run FSMs : */
    ui_fsm_run(ui_fsm);
    temp_ctrl_fsm_run(temp_fsm);
    event_manager_fsm_run(evm_fsm);
    feeder_fsm_run(feeder_fsm);                                          
    drawer_ctrl_fsm_run(drawer_fsm);
    petcall_fsm_run(petcall_fsm);
    power_fsm_run(power_fsm);

    /* update FSMs */
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
