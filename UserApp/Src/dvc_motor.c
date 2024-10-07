#include "dvc_motor.h"

float PID_Calc(PID_Controller *PID, float Current, float Target)
{
    float err, out;
    err = Target - Current; // 计算误差
    PID->p_out = PID->Kp * err;
    PID->i_out += PID->Ki * err;
    PID->i_out = (PID->i_out > I_MAX) ? I_MAX : PID->i_out;
    PID->d_out = PID->Kd * (err - PID->last_err);
    out = PID->p_out + PID->i_out, +PID->d_out;
    out = (out > OUT_MAX) ? OUT_MAX : out;
    return out;
}
