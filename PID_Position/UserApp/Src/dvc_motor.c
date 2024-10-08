#include "dvc_motor.h"

float PID_Calc(PID_Controller *PID, float Current, float Target)
{
    PID->err = Target - Current;
    PID->p_out = PID->Kp * PID->err;
    PID->i_out += PID->Ki * PID->err;
    PID->i_out = limit(PID->i_out, -PID->i_max, PID->i_max);
    PID->d_out = PID->Kd * (PID->err - PID->last_err);
    PID->out = PID->p_out + PID->i_out + PID->d_out;
    PID->out = limit(PID->out, -OUT_MAX, OUT_MAX);
    PID->last_err = PID->err;
    return PID->out;
}

float limit(float value, float min, float max)
{
    if (value > max)
        return max;
    else if (value < min)
        return min;
    else
        return value;
}
