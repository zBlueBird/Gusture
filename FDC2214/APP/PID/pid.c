#include"pid.h"

u16 PWM_Control(u16 input, u16 dianya_now, u16 Limit_high, u16 Limit_low, double Kp, double Ki,
                double Kd)
{
    double pwm_increment, err;
    static double err_last, err_next;
    static u16 pwm_now = 1900;
    err = dianya_now - input;
    pwm_increment = Kp * (err - err_next) + Ki * err + Kd * (err - 2 * err_next + err_last);
    pwm_now += pwm_increment;
    err_last = err_next;
    err_next = err;
    if (pwm_now < Limit_low) { pwm_now = Limit_low; }
    else if (pwm_now > Limit_high) { pwm_now = Limit_high; }
    else { pwm_now = (u16)pwm_now; }
    return pwm_now;
}























u16 PID_realize(u16 target, u16 Now_AD, double Kp, double Kd)
{
    static double lastError;
    double dError, Error;
    double PWM;
    Error = target - Now_AD;
    dError = Error - lastError;
    lastError = Error;
    PWM = Kp * Error + Kd * dError;
    if (PWM < 900) { PWM = 900; }
    else if (PWM > 2000) { PWM = 2000; }
    else { PWM = (double)PWM; }
    return PWM;
}


