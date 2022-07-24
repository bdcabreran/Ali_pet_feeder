/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "init_periph.h"
#include "event_manager_fsm.h"
#include "target_version.h"
#include "string.h"
#include "ui_draw.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


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

static void refresh_lcd_brightness(void);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /*Init peripherals */
  init_peripherals();

 #if 1
  /*startup info */
  print_startup_msg();

  /*Init navigation button control */
  btn_debounce_init(&navigation_btn);

  /*Init drawer switches control */
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

  /*Init Petcall */
//  petcall_handle_t petcall_fsm = petcall_fsm_get();
//  petcall_fsm_init(petcall_fsm);

  /*Init Drawer Controller */
  drawer_ctrl_handle_t drawer_fsm = drawer_ctrl_fsm_get();
  drawer_ctrl_fsm_init(drawer_fsm);

  /*Init Power Ctrl*/
  power_handle_t power_fsm = power_fsm_get();
  power_fsm_init(power_fsm);

  /*System clock Speed */
  printf("System CLK speed [%d]Hz\r\n", HAL_RCC_GetHCLKFreq());
  #endif 

  /* Infinite loop */
  while (1)
  {
    #if 1
    /* run FSMs : */
    ui_fsm_run(ui_fsm);
    temp_ctrl_fsm_run(temp_fsm);
    event_manager_fsm_run(evm_fsm);
    feeder_fsm_run(feeder_fsm);                                          
    drawer_ctrl_fsm_run(drawer_fsm);
//    petcall_fsm_run(petcall_fsm);
    power_fsm_run(power_fsm);

    /* update FSMs */
    time_events_poll_update();
    #endif 
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
