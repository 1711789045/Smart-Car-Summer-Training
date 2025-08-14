/*********************************************************************************************************************
* MM32F327X-G8P Opensourec Library 即（MM32F327X-G8P 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
* 
* 本文件是 MM32F327X-G8P 开源库的一部分
* 
* MM32F327X-G8P 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
* 
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
* 
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
* 
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
* 
* 文件名称          main
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          IAR 8.32.4 or MDK 5.37
* 适用平台          MM32F327X_G8P
* 店铺链接          https://seekfree.taobao.com/
* 
* 修改记录
* 日期              作者                备注
* 2022-08-10        Teternal            first version
********************************************************************************************************************/

#include "zf_common_headfile.h"
#include "auto_menu.h"
#include "motor.h"
#include "encoder.h"
#include "image.h"
#include "servo.h"
#include "beep.h"
#include "imu.h"
#include "control.h"
// 打开新的工程或者工程移动了位置务必执行以下操作
// 第一步 关闭上面所有打开的文件
// 第二步 project->clean  等待下方进度条走完

// 本例程是开源库移植用空工程

// **************************** 代码区域 ****************************
#define PIT6                             (TIM6_PIT )                             // 使用的周期中断编号 如果修改 需要同步对应修改周期中断编号与 isr.c 中的调用
#define PIT6_PRIORITY                    (TIM6_IRQn)                             // 对应周期中断的中断编号 在 mm32f3277gx.h 头文件中查看 IRQn_Type 枚举体
#define PIT7                             (TIM7_PIT )                             // 使用的周期中断编号 如果修改 需要同步对应修改周期中断编号与 isr.c 中的调用
#define PIT7_PRIORITY                    (TIM7_IRQn)                             // 对应周期中断的中断编号 在 mm32f3277gx.h 头文件中查看 IRQn_Type 枚举体

uint32 count_10ms = 0;



int main(void)
{
    clock_init(SYSTEM_CLOCK_120M);                                              // 初始化芯片时钟 工作频率为 120MHz
    debug_init();
                                                               // 初始化默认 Debug UART
	menu_init();
	
	motor_init();
	
	mt9v03x_init();
	
	encoder_init();
	
	servo_init();

	beep_init();
	
	imu963ra_init();
	
	wireless_uart_init();
	
	 
	pit_ms_init(PIT6, 100);                                                      // 初始化 PIT 为周期中断 100ms 周期
    interrupt_set_priority(PIT6_PRIORITY, 0); 
	pit_ms_init(PIT7, 10);                                                      // 初始化 PIT 为周期中断 10ms 周期
    interrupt_set_priority(PIT7_PRIORITY, 0); 
 
    // 此处编写用户代码 例如外设初始化代码等
    
    // 此处编写用户代码 例如外设初始化代码等
	
	
//	ips200_show_string(0,160,"encoder_l:");
//	ips200_show_string(0,176,"encoder_r:");
//	ips200_show_string(0,192,"speed:");
//	ips200_show_string(0,208,"circle_flag:");
//	ips200_show_string(136,208,"midmode:");

//	ips200_show_string(0,224,"left:");
//	ips200_show_string(0,240,"right:");

	
    while(1)
    {	
        // 此处编写需要循环执行的代码
		if(count_10ms){
			show_process(NULL);
			image_process(188,120,0);
		}
//		motor_set_pid(kp,ki,kd1);
		
		all_control();
		
//		motor_process();
//		
//		servo_process();
		
//		printf("%f\n", filtering_angle);	//发送到vofa（调参用）
//		printf("%d,%d,%d\n", gx,gy,gz);	//发送到vofa（调参用）
//		printf("%d,%d,%d\n", speed, encoder_data_l, encoder_data_r);	//发送到vofa（调参用）


//		ips200_show_int(96,160,encoder_data_l,4);
//		ips200_show_int(96,176,encoder_data_r,4);
//		ips200_show_int(96,192,go_flag,4);
//		ips200_show_int(96,208,block_time,4);
//		ips200_show_int(96,192,final_mid_line-MT9V03X_W/2.0,4);
//		ips200_show_float(96,208,angle,3,2);
//		ips200_show_int(200,208,mid_mode,4);
		
        // 此处编写需要循环执行的代码
		system_delay_ms(5);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PIT 的周期中断处理函数 这个函数将在 PIT 对应的定时器中断调用 详见 isr.c
// 参数说明     void
// 返回参数     void
// 使用示例     pit_handler();
//-------------------------------------------------------------------------------------------------------------------
void pit6_handler (void)
{
	stop_time++;    
	start_time++;
	if(beep_flag){
		beep_on();
		beep_flag -= 0.5;
	}
	else
		beep_off();
	
}

void pit7_handler (void)
{
	count_10ms++;
	if(count_10ms == 2){
		//20ms
		servo_f = 1;
		circle_time++;
		
		//20ms
		count_10ms = 0;
	}
	
	
	//10ms
//	first_order_complementary_filtering();
	ramp_f = 1;
	motor_f = 1;
	encoder_data_l = encoder_get_count(ENCODER_L);                  // 获取编码器计数
    encoder_data_r = 0-encoder_get_count(ENCODER_R);                          // 获取编码器计数
	
    encoder_clear_count(ENCODER_L);                                       // 清空编码器计数
    encoder_clear_count(ENCODER_R);                               // 清空编码器计数
	
	
}
// **************************** 代码区域 ****************************
