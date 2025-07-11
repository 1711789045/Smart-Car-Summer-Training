#include "zf_common_headfile.h"
#include  "servo.h"
#include "pid.h"
#include <stdlib.h>
#include <math.h>

static PID_POSITIONAL_TypeDef turn_pid = {0};

float servo_pid_kp = 0.3,servo_pid_ki = 0,servo_pid_kd = 0.35;


void servo_init(void){
	// 初始化pwm通道
    pwm_init(SERVO_MOTOR_PWM, SERVO_MOTOR_FREQ, SERVO_MOTOR_MID);

}

void servo_setangle(float angle){
	angle = func_limit(angle,SERVO_MOTOR_LIMIT);
	float servo_motor_duty = SERVO_MOTOR_MID+angle;
	
	if(SERVO_MOTOR_R_MAX <= servo_motor_duty)
		servo_motor_duty = SERVO_MOTOR_R_MAX;
	if(SERVO_MOTOR_L_MAX >= servo_motor_duty)
		servo_motor_duty = SERVO_MOTOR_L_MAX;
	
	pwm_set_duty(SERVO_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY(servo_motor_duty));

}

void servo_set_pid(float kp,float ki,float kd){
	servo_pid_kp = kp;
	servo_pid_ki = ki;
	servo_pid_kd = kd;
}

void servo_control(uint8 mid_line){
	float angle = 0;
	float err = mid_line-MT9V03X_W/2.0;
	float k = (exp(-fabs(err))-1)/(exp(-fabs(err))+1);
	float kp = (fabs(k)/2+0.5) * servo_pid_kp;
	angle = pid_positional(&turn_pid,0,mid_line-MT9V03X_W/2.0,SERVO_MOTOR_LIMIT,
							servo_pid_kp,servo_pid_ki,servo_pid_kd);
	
	ips200_show_float(96,288,kp,1,3);
	
	servo_setangle(angle);
}

