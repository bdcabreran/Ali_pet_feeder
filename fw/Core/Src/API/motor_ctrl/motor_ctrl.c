#include "motor_ctrl.h"
#include "init_periph.h"


void control_motor(int drawer, int fwd, int bwd)
{
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

#if 0

void main()
{
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

  while(1)
  {
    TIM1->CCR4 = 300;  // Timer channel 4 output value
    control_motor(1, 360, 0); // Forward drawer 1
    control_motor(2, 0, 360); // Backward drawer 2
    control_motor(3, 360, 0); // Forward drawer 3
    control_motor(4, 0, 360); // Backward drawer 4
  }
}


#endif 