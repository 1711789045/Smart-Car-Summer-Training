#include "zf_common_headfile.h"
#include  "servo.h"
#include "auto_menu.h"
#include "pid.h"
#include "image.h"
#include <stdlib.h>
#include <math.h>
#include "motor.h"

static PID_POSITIONAL_TypeDef turn_pid = {0};

float angle = 0;
float servo_pid_kp = 0.35,servo_pid_ki = 0,servo_pid_kd1 = 0.56,servo_pid_kd2 = 0;
uint8 servo_f = 0;

void servo_init(void){
	// ��ʼ��pwmͨ��
    pwm_init(SERVO_MOTOR_PWM, SERVO_MOTOR_FREQ, 0);

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

void servo_set_pid(float kp,float ki,float kd1,float kd2){
	servo_pid_kp = kp;
	servo_pid_ki = ki;
	servo_pid_kd1 = kd1;
	servo_pid_kd2 = kd2;
}

void servo_control(uint8 mid_line){
	float err = mid_line-MT9V03X_W/2.0+2;
	float kp = 0;
//	float k = (exp(-fabs(err))-1)/(exp(-fabs(err))+1);
//	float kp = (fabs(k)/2 +0.5) * servo_pid_kp;
//	if(func_abs(err) <= 20){
//		kp = kp_min;
//	}
//	else{
//		kp = servo_pid_kp;
//	}
	kp = func_limit_ab(servo_pid_kp * (err*err)/1000 + kp_min,0,3);
	 
	imu963ra_get_gyro();
	angle = pid_positional(&turn_pid,0,err,SERVO_MOTOR_LIMIT,
							kp,servo_pid_ki,servo_pid_kd1,servo_pid_kd2);
	
//	ips200_show_float(96,288,kp,1,3);
	
	servo_setangle(angle);
}

void servo_process(void){
	if(servo_f){
		servo_set_pid(kp,ki,kd1,kd2);
		if(go_flag ){
			servo_control(final_mid_line);
		}
		if(stop_flag){
			if(stop_time<=25){
				servo_control(final_mid_line);	
			}
			else
				servo_init();
		}
		servo_f = 0;
	}
}
