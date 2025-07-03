#include "zf_common_headfile.h"

#define ENCODER_L                 (TIM3_ENCODER)                         
#define ENCODER_L_A               (TIM3_ENCODER_CH1_B4)                   
#define ENCODER_L_B               (TIM3_ENCODER_CH2_B5)                   

#define ENCODER_R		        (TIM4_ENCODER)                       
#define ENCODER_R_A               (TIM4_ENCODER_CH1_B6)                   
#define ENCODER_R_B               (TIM4_ENCODER_CH2_B7)                   

void encoder_init(void){
	encoder_quad_init(ENCODER_L, ENCODER_L_A, ENCODER_L_B);
	encoder_quad_init(ENCODER_R, ENCODER_R_A, ENCODER_R_B);
}
	


