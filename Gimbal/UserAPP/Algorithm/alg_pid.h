/**
 * @file alg_pid.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief PID�㷨
 * @version 0.1
 * @date 2022-05-03
 *
 * @copyright USTC-RoboWalker (c) 2022
 *
 */

#ifndef ALG_PID_H
#define ALG_PID_H

/* Includes ------------------------------------------------------------------*/

#include "drv_math.h"

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
 * @brief ΢������
 *
 */
enum Enum_PID_D_First
{
    PID_D_First_DISABLE = 0,
    PID_D_First_ENABLE,
};

/**
 * @brief Reusable, PID�㷨
 *
 */
class Class_PID
{
public:
    void Init(float __K_P, float __K_I, float __K_D, float __K_F = 0.0f, float __I_Out_Max = 0.0f, float __Out_Max = 0.0f, float __D_T = 0.001f, float __Dead_Zone = 0.0f, float __I_Variable_Speed_A = 0.0f, float __I_Variable_Speed_B = 0.0f, float __I_Separate_Threshold = 0.0f, Enum_PID_D_First __D_First = PID_D_First_DISABLE);

    float Get_Integral_Error();
    float Get_Out();

    void Set_K_P(float __K_P);
    void Set_K_I(float __K_I);
    void Set_K_D(float __K_D);
    void Set_K_F(float __K_F);
    void Set_I_Out_Max(float __I_Out_Max);
    void Set_Out_Max(float __Out_Max);
    void Set_I_Variable_Speed_A(float __Variable_Speed_I_A);
    void Set_I_Variable_Speed_B(float __Variable_Speed_I_B);
    void Set_I_Separate_Threshold(float __I_Separate_Threshold);
    void Set_Target(float __Target);
    void Set_Now(float __Now);
    void Set_Integral_Error(float __Integral_Error);

    void TIM_Adjust_PeriodElapsedCallback();

protected:
    // ��ʼ����س���

    // PID��ʱ������, s
    float D_T;
    // ����, Error�������ֵ�ڲ����
    float Dead_Zone;
    // ΢������
    Enum_PID_D_First D_First;

    // ����

    // �ڲ�����

    // ֮ǰ�ĵ�ǰֵ
    float Pre_Now = 0.0f;
    // ֮ǰ��Ŀ��ֵ
    float Pre_Target = 0.0f;
    // ֮ǰ�����ֵ
    float Pre_Out = 0.0f;
    // ǰ�����
    float Pre_Error = 0.0f;

    // ������

    // ���ֵ
    float Out = 0.0f;

    // д����

    // PID��P
    float K_P = 0.0f;
    // PID��I
    float K_I = 0.0f;
    // PID��D
    float K_D = 0.0f;
    // ǰ��
    float K_F = 0.0f;

    // �����޷�, 0Ϊ������
    float I_Out_Max = 0;
    // ����޷�, 0Ϊ������
    float Out_Max = 0;

    // ���ٻ��ֶ����ڶ���ֵ, 0Ϊ������
    float I_Variable_Speed_A = 0.0f;
    // ���ٻ��ֱ�������, 0Ϊ������
    float I_Variable_Speed_B = 0.0f;
    // ���ַ�����ֵ����Ϊ����, 0Ϊ������
    float I_Separate_Threshold = 0.0f;

    // Ŀ��ֵ
    float Target = 0.0f;
    // ��ǰֵ
    float Now = 0.0f;

    // ��д����

    // ����ֵ
    float Integral_Error = 0.0f;

    // �ڲ�����
};

/* Exported variables --------------------------------------------------------*/

/* Exported function declarations --------------------------------------------*/

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
