#ifndef __ENCODER_H__
#define __ENCODER_H__

#define ENCODER_L                 (TIM3_ENCODER)                         
#define ENCODER_L_A               (TIM3_ENCODER_CH1_B4)                   
#define ENCODER_L_B               (TIM3_ENCODER_CH2_B5)                   

#define ENCODER_R		        (TIM4_ENCODER)                       
#define ENCODER_R_A               (TIM4_ENCODER_CH1_B6)                   
#define ENCODER_R_B               (TIM4_ENCODER_CH2_B7)                   

void encoder_init(void);

#endif
