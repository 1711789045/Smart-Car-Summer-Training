#include "zf_common_headfile.h"
#include "control.h"
#include "motor.h"
#include "servo.h"
#include "auto_menu.h"
#include "imu.h"
#include "beep.h"
#include "image.h"

int speed = 0;
uint8 ramp_flag = 0,ramp_f = 0;

void ramp_analysis(void){
	if(ramp_flag == 0 && filtering_angle < -400){
		ramp_flag = 1;
		beep_flag = 1;
	}
	if(ramp_flag && filtering_angle>-200){
		ramp_flag = 2;
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
	if(ramp_f){
		first_order_complementary_filtering();
		ramp_analysis();
		ramp_f = 0;
	}
	if(ramp_flag == 1)
		speed = 100;
	
	servo_process();
	motor_process();
}


