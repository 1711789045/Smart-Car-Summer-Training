#include "zf_common_headfile.h"
#include  "servo.h"


//占空比最大值是10000

void servo_init(void){
	// 初始化pwm通道
	pwm_init(SERVO_PWM_CH1, 50, SERVO_PWMMID);

}

void servo_setangle(int16 angle){
	// 初始化pwm通道
	angle = func_limit(angle,SERVO_LIMIT);
	pwm_set_duty(SERVO_PWM_CH1, SERVO_PWMMID+angle);

}

