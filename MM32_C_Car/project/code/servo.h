#ifndef __SERVO_H__
#define __SERVO_H__
#include "zf_common_headfile.h"

#define SERVO_PWM_CH1           (TIM2_PWM_CH1_A15)
#define SERVO_PWMMID            400                //舵机中值
#define SERVO_LIMIT              50                 //舵机最大转角
void servo_init(void);


#endif
