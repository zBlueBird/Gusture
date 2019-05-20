#ifndef __PID_H
#define __PID_H
#include"stm32f10x.h"












u16 PWM_Control(u16 input, u16 dianya_now, u16 Limit_high, u16 Limit_low, double Kp, double Ki,
                double Kd);

u16 PID_realize(u16 setpoint, u16 Now_AD, double Kp, double Kd);

#endif

