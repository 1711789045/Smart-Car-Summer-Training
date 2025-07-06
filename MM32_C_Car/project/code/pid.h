#ifndef __PID_H__
#define __PID_H__

typedef struct {
    float last_error;      // 上一次误差
    float prev_error;      // 上上次误差（用于微分）
    float output;          // 当前输出值
} PID_TypeDef;

float pid_increment(PID_TypeDef *pid, float target, float current, 
                    float limit, float kp, float ki, float kd);
#endif
