#ifndef __MOTOR_H__
#define __MOTOR_H__
#include "zf_common_headfile.h"

#define MOTOR_L    0
#define MOTOR_R    1

#define MOTOR_L_PWM_CH4                 (TIM5_PWM_CH4_A3)
#define MOTOR_R_PWM_CH2                 (TIM5_PWM_CH2_A1)

#define MOTOR_L_DIR             (A2)
#define MOTOR_R_DIR             (A0)

#define SPEED_LIMIT             3000           //速度上限


void motor_init(void);
void motor_set_pid(float kp,float ki,float kd);
void motor_setpwm(uint8 motor,int16 speed);
void motor_setspeed(int16 target,float current_l,float current_r);
#endif
