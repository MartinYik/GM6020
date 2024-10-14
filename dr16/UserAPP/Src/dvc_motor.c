#include "dvc_motor.h"

float PID_Calc(PID_Controller *PID, float Current, float Target)
{
    PID->err = Target - Current; // 计算误差
    PID->p_out = PID->Kp * PID->err;
    PID->i_out += PID->Ki * PID->err;
    PID->i_out = (PID->i_out > PID->i_max) ? PID->i_max : PID->i_out;
    PID->d_out = PID->Kd * (PID->err - PID->last_err);
    PID->out = PID->p_out + PID->i_out + PID->d_out;
    PID->out = (PID->out > OUT_MAX) ? OUT_MAX : PID->out;
    return PID->out;
}
