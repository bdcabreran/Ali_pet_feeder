#include "motor_ctrl.h"
#include "init_periph.h"


void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
void Error_Handler(void);

/*  PWM signal with 50KHz and variable duty cycle from 0-19
  F = Timer CLK / ( Prescaler + 1 )( Period +1 )
  F = 80MHz / (79 + 1) (19 + 1) = 50KHz
*/

#define MOTOR_PWM_PRESCALER (79)
#define MOTOR_PWM_PERIOD    (19)

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim1;
motor_ctrl_t motor_handle[MOTORn];


void tim1_init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = MOTOR_PWM_PRESCALER;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = MOTOR_PWM_PERIOD;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /*Motor Configuration : 
  /*
  FWD3 : TIM1_CH1
  BWD3 : TIM1_CH2
  FWD4 : TIM1_CH3
  BWD4 : TIM1_CH4 */ 

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
    Error_Handler();
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
    Error_Handler();
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
    Error_Handler();
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
    Error_Handler();

  HAL_TIM_MspPostInit(&htim1);
}

/*Enable PWM in CH1, CH2, CH3, CH4*/ 
void tim2_init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = MOTOR_PWM_PRESCALER;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = MOTOR_PWM_PERIOD;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /*Motor Configuration : 
  FWD1 : TIM2_CH1
  BWD1 : TIM2_CH2
  FWD2 : TIM2_CH3
  BWD2 : TIM2_CH4 
  */ 
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
    Error_Handler();
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
    Error_Handler();
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
    Error_Handler();
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
    Error_Handler();

  HAL_TIM_MspPostInit(&htim2);
}

void dc_motor_init(void)
{
  motor_handle[MOTOR_NO_1].pwm.ch_fwd = TIM_CHANNEL_1;
  motor_handle[MOTOR_NO_1].pwm.ch_bwd = TIM_CHANNEL_2;
  motor_handle[MOTOR_NO_1].pwm.tim = &htim2;

  motor_handle[MOTOR_NO_2].pwm.ch_fwd = TIM_CHANNEL_3;
  motor_handle[MOTOR_NO_2].pwm.ch_bwd = TIM_CHANNEL_4;
  motor_handle[MOTOR_NO_2].pwm.tim = &htim2;

  motor_handle[MOTOR_NO_3].pwm.ch_fwd = TIM_CHANNEL_1;
  motor_handle[MOTOR_NO_3].pwm.ch_bwd = TIM_CHANNEL_2;
  motor_handle[MOTOR_NO_3].pwm.tim = &htim1;

  motor_handle[MOTOR_NO_4].pwm.ch_fwd = TIM_CHANNEL_3;
  motor_handle[MOTOR_NO_4].pwm.ch_bwd = TIM_CHANNEL_4;
  motor_handle[MOTOR_NO_4].pwm.tim = &htim1;

  tim1_init();
  tim2_init();
}

void dc_motor_move_forward(motor_no_t no, motor_speed_t speed )
{
  motor_ctrl_t *motor_ptr = &motor_handle[no];
  HAL_TIM_PWM_Stop(motor_ptr->pwm.tim, motor_ptr->pwm.ch_bwd);
  __HAL_TIM_SET_COMPARE(motor_ptr->pwm.tim, motor_ptr->pwm.ch_fwd, speed);
  HAL_TIM_PWM_Start(motor_ptr->pwm.tim, motor_ptr->pwm.ch_fwd);
}

void dc_motor_move_backward(motor_no_t no, motor_speed_t speed )
{
  motor_ctrl_t *motor_ptr = &motor_handle[no];
  HAL_TIM_PWM_Stop(motor_ptr->pwm.tim, motor_ptr->pwm.ch_fwd);
  __HAL_TIM_SET_COMPARE(motor_ptr->pwm.tim, motor_ptr->pwm.ch_bwd, speed);
  HAL_TIM_PWM_Start(motor_ptr->pwm.tim, motor_ptr->pwm.ch_bwd);
}

void dc_motor_move_direction(motor_no_t no, motor_speed_t speed,  motor_direction_t dir)
{
  if(dir == MOTOR_MOVE_FORWARD)
  {
    dc_motor_move_forward(no, speed);
  }
  else if (dir == MOTOR_MOVE_BACKWARD)
  {
    dc_motor_move_backward(no, speed);
  }
}

void dc_motor_stop(motor_no_t no)
{
  motor_ctrl_t *motor_ptr = &motor_handle[no];
  HAL_TIM_PWM_Stop(motor_ptr->pwm.tim, motor_ptr->pwm.ch_bwd);
  HAL_TIM_PWM_Stop(motor_ptr->pwm.tim, motor_ptr->pwm.ch_fwd);
}

