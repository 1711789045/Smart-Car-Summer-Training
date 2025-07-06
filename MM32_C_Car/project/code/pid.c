#include "zf_common_headfile.h"

typedef struct {
    float last_error;      // 上一次误差
    float prev_error;      // 上上次误差（用于微分）
    float output;          // 当前输出值
} PID_TypeDef;

//增量式pid
float pid_increment(PID_TypeDef *pid, float target, float current, 
                    float limit, float kp, float ki, float kd) 
{
    float error = target - current;
    
    float p_term = kp * (error - pid->last_error);
    float i_term = ki * error;       
    float d_term = kd * (error - 2*pid->last_error + pid->prev_error);
    
    float increment = p_term + i_term + d_term;
    pid->output += increment;

    // 更新误差历史
    pid->prev_error = pid->last_error;
    pid->last_error = error;

    // 输出限幅
    if(pid->output > limit) pid->output = limit;
    else if(pid->output < -limit) pid->output = -limit;
    
    return pid->output;
}
