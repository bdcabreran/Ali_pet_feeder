/**
 * @file motor_ctrl.h
 * @author Bayron Cabrera (bdcabreran@unal.edu.co)
 * @brief  DC motor control driver for TMI8260SP IC, max freq = 50MHz
 * IN1: L, IN2: L = Coast, h-bridge disabled to High Z
 * IN1: L, IN2: H = Reverse (Current OUT2 -> OUT1) 
 * IN1: H, IN2: L = Forward (Current OUT1 -> OUT2) 
 * IN1: H, IN2: H = Brake, low side slow decay 
 * link data sheet : http://en.toll-semi.com/public/html/pdfjs/viewer/viewernew.html?file=https://v1.cecdn.yun300.cn/100001_2003315218/TMI8260SP_TMI8260DP_DS_EN_V1.4.pdf
 * @version 0.1
 * @date 2022-07-18
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef MOTOR_CTRL_H
#define MOTOR_CTRL_H

#include "stdint.h"
#include "stm32f1xx_hal.h"

typedef struct
{
    TIM_HandleTypeDef *tim;
    uint32_t ch_fwd;
    uint32_t ch_bwd;
} motor_pwm_t;

typedef enum
{
    MOTOR_MOVE_INVALID,
    MOTOR_MOVE_FORWARD,
    MOTOR_MOVE_BACKWARD,
    MOTOR_MOVE_LAST,
}motor_direction_t;

typedef enum
{
    MOTOR_NO_1,
    MOTOR_NO_2,
    MOTOR_NO_3,
    MOTOR_NO_4,
    MOTORn,
}motor_no_t;

/*Value between 0 to 19*/
typedef enum
{
    MOTOR_CTRL_SPEED_INVALID,
    MOTOR_CTRL_SPEED_SLOW = 5,
    MOTOR_CTRL_SPEED_MEDIUM = 10,
    MOTOR_CTRL_SPEED_FAST = 15,
    MOTOR_CTRL_SPEED_LAST,
}motor_speed_t;

typedef struct
{
    motor_pwm_t pwm;
}motor_ctrl_t;

void dc_motor_init(void);
void dc_motor_move_forward(motor_no_t no, motor_speed_t speed );
void dc_motor_move_backward(motor_no_t no, motor_speed_t speed );
void dc_motor_move_direction(motor_no_t no, motor_speed_t speed,  motor_direction_t dir);
void dc_motor_stop(motor_no_t no);

#if 0
void control_motor(int drawer, int fwd, int bwd);
#endif


#endif