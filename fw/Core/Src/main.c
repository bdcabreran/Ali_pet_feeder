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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


void control_motor(int drawer, int fwd, int bwd){
    if(drawer == 1){
      TIM2->CCR1 = fwd;
      TIM2->CCR2 = bwd;
    }

    if(drawer == 2){
      TIM2->CCR3 = fwd;
      TIM2->CCR4 = bwd;
    }

    if(drawer == 3){
      TIM1->CCR1 = fwd;
      TIM1->CCR2 = bwd;
    }

    if(drawer == 4){
      TIM1->CCR3 = fwd;
      TIM1->CCR4 = bwd;
    }    
}



/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* Initialize all configured peripherals */
  init_peripherals();
 


  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); // Init channel 1 PWM
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2); // Init channel 2 PWM
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3); // Init channel 3 PWM
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4); // Init channel 4 PWM

  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); // Init channel 1 PWM
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2); // Init channel 2 PWM
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3); // Init channel 3 PWM
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4); // Init channel 4 PWM

  //TIM2->CCR1 = arr_max; // Timer channel 1 output value
  //TIM2->CCR2 = arr_min; // Timer channel 2 output value
  //TIM2->CCR3 = 10000; // Timer channel 3 output value
  //TIM2->CCR4 = 5000;  // Timer channel 4 output value
  

  /*Init UI */
  //ui_handle_t ui_fsm = ui_fsm_get();
  //ui_fsm_init(ui_fsm);

  /* Infinite loop */
  while (1)  {
    
    //ui_fsm_run(ui_fsm);
    TIM1->CCR4 = 300;  // Timer channel 4 output value
    control_motor(1, 360, 0); // Forward drawer 1
    control_motor(2, 0, 360); // Backward drawer 2
    control_motor(3, 360, 0); // Forward drawer 3
    //control_motor(4, 0, 360); // Backward drawer 4


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
