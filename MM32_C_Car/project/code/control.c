#include "zf_common_headfile.h"
#include "control.h"
#include "motor.h"
#include "servo.h"
#include "auto_menu.h"
#include "imu.h"
#include "beep.h"
#include "image.h"

int speed = 0;
uint8 ramp_flag = 0;

void ramp_analysis(void){
	if(filtering_angle < -600){
		ramp_flag = 1;
		beep_flag = 1;
	}
	if(filtering_angle>-400){
		ramp_flag = 0;
	}
}

void all_control(void){
	speed = basic_speed;
	if(speed_policy_flag){
		
		if(prospect >= 114)
			speed = basic_speed+300;
		else if(prospect >=110)
			speed = basic_speed+140;
		else if(prospect >= 109)
			speed = basic_speed+100;
		else if(prospect >= 108)
			speed = basic_speed+80;
		else if(prospect >= 107)
			speed = basic_speed+60;
		else if(prospect >= 106)
			speed = basic_speed+20;
		
		
		
	}
//	ramp_analysis();
//	if(ramp_flag)
//		speed = 150;
	
	servo_process();
	motor_process();
}


