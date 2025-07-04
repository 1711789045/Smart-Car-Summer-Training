#include "zf_common_headfile.h"

#define MOTOR_L    0
#define MOTOR_R    1

#define MOTOR_L_PWM_CH2                 (TIM5_PWM_CH2_A1)
#define MOTOR_R_PWM_CH4                 (TIM5_PWM_CH4_A3)

#define MOTOR_L_DIR             (A0)
#define MOTOR_R_DIR             (A2)

#define SPEED_LIMIT             50           //速度上限

void motor_init(void){
	// 初始化pwm通道2和4
	pwm_init(MOTOR_L_PWM_CH2, 17000, 0);
	pwm_init(MOTOR_R_PWM_CH4, 17000, 0);
	
	// 初始化 LED2 输出 默认高电平 推挽输出模式
	gpio_init(MOTOR_L_DIR, GPO, GPIO_HIGH, GPO_PUSH_PULL);
	gpio_init(MOTOR_R_DIR, GPO, GPIO_HIGH, GPO_PUSH_PULL);
}

void motor_setspeed(int16 speed){
		if(speed >= 0){
			if(speed>SPEED_LIMIT)
				speed = SPEED_LIMIT;
			speed = speed *100;
			pwm_set_duty(MOTOR_L_PWM_CH2, speed);
			pwm_set_duty(MOTOR_R_PWM_CH4, speed);
			gpio_set_level(MOTOR_L_DIR, GPIO_HIGH);
			gpio_set_level(MOTOR_R_DIR, GPIO_HIGH);
		}
		else{
			speed = -speed;
			if(speed>=SPEED_LIMIT)
				speed = SPEED_LIMIT;
			speed = speed *100;
			pwm_set_duty(MOTOR_L_PWM_CH2, speed);
			pwm_set_duty(MOTOR_R_PWM_CH4, speed);
			gpio_set_level(MOTOR_L_DIR, GPIO_LOW);
			gpio_set_level(MOTOR_R_DIR, GPIO_LOW);
		}
}

