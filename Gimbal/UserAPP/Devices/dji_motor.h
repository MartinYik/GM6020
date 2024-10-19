/**
 * @file dvc_motor.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief �޸���USTC��CAN������������
 * @version 0.2
 * @date 2022-08-03
 * @date 2024-10-17 0.2 �޸���һЩ�������
 * @date 2024-10-19 0.3 ������MPU6050�ٶȻ�����
 *
 * @copyright USTC-RoboWalker (c) 2022
 *
 */

#ifndef DVC_MOTOR_H
#define DVC_MOTOR_H

/* Includes ------------------------------------------------------------------*/

#include "alg_pid.h"
#include "drv_can.h"

/* Exported macros -----------------------------------------------------------*/

// RPM���㵽rad/s ����
#define RPM_TO_RADPS (2.0f * PI / 60.0f)
#define RADPS_TO_RPM (60.0f / (2.0f * PI))
/* Exported types ------------------------------------------------------------*/

/**
 * @brief ���״̬
 *
 */
enum Enum_CAN_Motor_Status
{
    CAN_Motor_Status_DISABLE = 0,
    CAN_Motor_Status_ENABLE,
};

/**
 * @brief CAN�����IDö������
 *
 */
enum Enum_CAN_Motor_ID
{
    CAN_Motor_ID_UNDEFINED = 0,
    CAN_Motor_ID_0x201,
    CAN_Motor_ID_0x202,
    CAN_Motor_ID_0x203,
    CAN_Motor_ID_0x204,
    CAN_Motor_ID_0x205,
    CAN_Motor_ID_0x206,
    CAN_Motor_ID_0x207,
    CAN_Motor_ID_0x208,
    CAN_Motor_ID_0x209,
    CAN_Motor_ID_0x20A,
    CAN_Motor_ID_0x20B,
};

/**
 * @brief CAN�����ID�������
 *
 */
enum Enum_CAN_Motor_ID_Status
{
    CAN_Motor_ID_Status_FREE = 0,
    CAN_Motor_ID_Status_ALLOCATED,
};

/**
 * @brief ������Ʒ�ʽ
 *
 */
enum Enum_Control_Method
{
    Control_Method_OPENLOOP = 0,
    Control_Method_TORQUE,
    Control_Method_RPM,
    Control_Method_ANGLE,
};

/**
 * @brief GM6020��ˢ���, ��Ƭ�����������ѹ
 *
 */
class Class_Motor_GM6020
{
public:
    // PID�ǶȻ�����
    Class_PID PID_Angle;
    // PID���ٶȻ�����
    Class_PID PID_Rpm;
    // PIDŤ�ػ�����
    Class_PID PID_Torque;
    // PIDMPU���ٶȻ�����
    Class_PID PID_MPU_Rpm;

    void Init(CAN_HandleTypeDef *__hcan, Enum_CAN_Motor_ID __CAN_ID, Enum_Control_Method __Control_Method = Control_Method_ANGLE, int32_t __Encoder_Offset = 0, float __Rpm_Max = 320.0f);

    uint16_t Get_Output_Max();
    Enum_CAN_Motor_Status Get_CAN_Motor_Status();
    float Get_Now_Angle();
    float Get_Now_Rpm();
    float Get_Now_MPU_Rpm();
    float Get_Now_Torque();
    uint8_t Get_Now_Temperature();
    Enum_Control_Method Get_Control_Method();
    float Get_Target_Angle();
    float Get_Target_Rpm();
    float Get_Target_Torque();
    float Get_Out();

    void Set_Control_Method(Enum_Control_Method __Control_Method);
    void Set_Target_Angle(float __Target_Angle);
    void Set_Target_Rpm(float __Target_Rpm);
    void Set_Target_Torque(float __Target_Torque);
    void Set_Out(float __Out);
    void Set_MPU_Rpm(float __MPU_Rads);
    void Set_Zero();

    void CAN_RxCpltCallback(uint8_t *Rx_Data);
    void TIM_Alive_PeriodElapsedCallback();
    void TIM_PID_PeriodElapsedCallback();

protected:
    // ��ʼ����ر���

    // �󶨵�CAN
    Struct_CAN_Manage_Object *CAN_Manage_Object;
    // �����ݰ󶨵�CAN ID, C6ϵ��0x201~0x208, GMϵ��0x205~0x20b
    Enum_CAN_Motor_ID CAN_ID;
    // ���ͻ�����
    uint8_t *CAN_Tx_Data;
    // ������ƫ��
    uint32_t Encoder_Offset;
    // ����ٶ�, ����ݲ�ͬ���ز�����ֵ, Ҳ�Ϳ�������õõ�, �����Ҹо�Ӧ��û������ϲ���������������
    float Rpm_Max;

    // ����

    // һȦ�������̶�
    uint16_t Encoder_Num_Per_Round = 8192;
    // ��������ѹ
    uint16_t Output_Max = 30000;

    // �ڲ�����

    // ��ǰʱ�̵ĵ������flag
    uint32_t Flag = 0;
    // ǰһʱ�̵ĵ������flag
    uint32_t Pre_Flag = 0;

    // ���յı�����λ��, 0~8191
    uint16_t Rx_Encoder = 0;
    // ���յ��ٶ�, rpm
    int16_t Rx_Rpm = 0;
    // ��MPU6050���ղ�ת���Ľ��ٶ�, rpm
    int16_t Now_MPU_Rpm = 0;
    // ���յ�Ť��, Ŀ���Ť��, -30000~30000
    int16_t Rx_Torque = 0;
    // ���յ��¶�, ���϶�
    uint16_t Rx_Temperature = 0;

    // ֮ǰ�ı�����λ��
    uint16_t Pre_Encoder = 0;
    // �ܱ�����λ��
    int32_t Total_Encoder = 0;
    // ��Ȧ��
    int32_t Total_Round = 0;

    // ������

    // ���״̬
    Enum_CAN_Motor_Status CAN_Motor_Status = CAN_Motor_Status_DISABLE;

    // ��ǰ�ĽǶ�, rad
    float Now_Angle = 0.0f;
    // ��ǰ���ٶ�, rad/s
    float Now_Rpm = 0.0f;
    // ��ǰ��Ť��, ֱ�Ӳ��÷���ֵ
    float Now_Torque = 0.0f;
    // ��ǰ���¶�, ���϶�
    uint8_t Now_Temperature = 0;

    // д����

    // ��д����

    // ������Ʒ�ʽ
    Enum_Control_Method Control_Method = Control_Method_ANGLE;
    // Ŀ��ĽǶ�, rad
    float Target_Angle = 0.0f;
    // Ŀ����ٶ�, rad/s
    float Target_Rpm = 0.0f;
    // Ŀ���Ť��, ֱ�Ӳ��÷���ֵ
    float Target_Torque = 0.0f;
    // �����
    float Out = 0.0f;

    // �ڲ�����

    void Output();
};

/**
 * @brief C610��ˢ���, �Դ�Ť�ػ�, ��Ƭ���������Ť��
 *
 */
class Class_Motor_C610
{
public:
    // PID�ǶȻ�����
    Class_PID PID_Angle;
    // PID���ٶȻ�����
    Class_PID PID_Rpm;

    void Init(CAN_HandleTypeDef *__hcan, Enum_CAN_Motor_ID __CAN_ID, Enum_Control_Method __Control_Method = Control_Method_RPM, float __Gearbox_Rate = 36.0f, float __Torque_Max = 10000.0f);

    uint16_t Get_Output_Max();
    Enum_CAN_Motor_Status Get_CAN_Motor_Status();
    float Get_Now_Angle();
    float Get_Now_Rpm();
    float Get_Now_Torque();
    uint8_t Get_Now_Temperature();
    Enum_Control_Method Get_Control_Method();
    float Get_Target_Angle();
    float Get_Target_Rpm();
    float Get_Target_Torque();
    float Get_Out();

    void Set_Control_Method(Enum_Control_Method __Control_Method);
    void Set_Target_Angle(float __Target_Angle);
    void Set_Target_Rpm(float __Target_Rpm);
    void Set_Target_Torque(float __Target_Torque);
    void Set_Out(float __Out);
    void Set_Zero();

    void CAN_RxCpltCallback(uint8_t *Rx_Data);
    void TIM_Alive_PeriodElapsedCallback();
    void TIM_PID_PeriodElapsedCallback();

protected:
    // ��ʼ����س���

    // �󶨵�CAN
    Struct_CAN_Manage_Object *CAN_Manage_Object;
    // �����ݰ󶨵�CAN ID, C6ϵ��0x201~0x208, GMϵ��0x205~0x20b
    Enum_CAN_Motor_ID CAN_ID;
    // ���ͻ�����
    uint8_t *CAN_Tx_Data;
    // ���ٱ�, Ĭ�ϴ�������
    float Gearbox_Rate = 36.0f;
    // ���Ť��, ����ݲ�ͬ���ز�����ֵ, Ҳ�Ϳ�����Ť�ػ�����õõ�, �����Ҹо�Ӧ��û������ϲ���������������
    float Torque_Max = 10000.0f;

    // ����

    // һȦ�������̶�
    uint16_t Encoder_Num_Per_Round = 8192;
    // ������Ť��
    uint16_t Output_Max = 10000;

    // �ڲ�����

    // ��ǰʱ�̵ĵ������flag
    uint32_t Flag = 0;
    // ǰһʱ�̵ĵ������flag
    uint32_t Pre_Flag = 0;

    // ���յı�����λ��, 0~8191
    uint16_t Rx_Encoder = 0;
    // ���յ��ٶ�, rpm
    int16_t Rx_Rpm = 0;
    // ���յ�Ť��, Ŀ���Ť��, -30000~30000
    int16_t Rx_Torque = 0;
    // ���յ��¶�, ���϶�
    uint16_t Rx_Temperature = 0;

    // ֮ǰ�ı�����λ��
    uint16_t Pre_Encoder = 0;
    // �ܱ�����λ��
    int32_t Total_Encoder = 0;
    // ��Ȧ��
    int32_t Total_Round = 0;

    // ������

    // ���״̬
    Enum_CAN_Motor_Status CAN_Motor_Status = CAN_Motor_Status_DISABLE;

    // ��ǰ�ĽǶ�, rad
    float Now_Angle = 0.0f;
    // ��ǰ���ٶ�, rad/s
    float Now_Rpm = 0.0f;
    // ��ǰ��Ť��, ֱ�Ӳ��÷���ֵ
    float Now_Torque = 0.0f;
    // ��ǰ���¶�, ���϶�
    uint8_t Now_Temperature = 0;

    // д����

    // ��д����

    // ������Ʒ�ʽ
    Enum_Control_Method Control_Method = Control_Method_ANGLE;
    // Ŀ��ĽǶ�, rad
    float Target_Angle = 0.0f;
    // Ŀ����ٶ�, rad/s
    float Target_Rpm = 0.0f;
    // Ŀ���Ť��, ֱ�Ӳ��÷���ֵ
    float Target_Torque = 0.0f;
    // �����
    float Out = 0.0f;

    // �ڲ�����

    void Output();
};

/**
 * @brief C620��ˢ���, �Դ�Ť�ػ�, ��Ƭ���������Ť��
 *
 */
class Class_Motor_C620
{
public:
    // PID�ǶȻ�����
    Class_PID PID_Angle;
    // PID���ٶȻ�����
    Class_PID PID_Rpm;

    void Init(CAN_HandleTypeDef *__hcan, Enum_CAN_Motor_ID __CAN_ID, Enum_Control_Method __Control_Method = Control_Method_RPM, float __Gearbox_Rate = 3591.0f / 187.0f, float __Torque_Max = 16384.0f);

    uint16_t Get_Output_Max();
    Enum_CAN_Motor_Status Get_CAN_Motor_Status();
    float Get_Now_Angle();
    float Get_Now_Rpm();
    float Get_Now_Torque();
    uint8_t Get_Now_Temperature();
    Enum_Control_Method Get_Control_Method();
    float Get_Target_Angle();
    float Get_Target_Rpm();
    float Get_Target_Torque();
    float Get_Out();

    void Set_Control_Method(Enum_Control_Method __Control_Method);
    void Set_Target_Angle(float __Target_Angle);
    void Set_Target_Rpm(float __Target_Rpm);
    void Set_Target_Torque(float __Target_Torque);
    void Set_Out(float __Out);
    void Set_Zero();

    void CAN_RxCpltCallback(uint8_t *Rx_Data);
    void TIM_Alive_PeriodElapsedCallback();
    void TIM_PID_PeriodElapsedCallback();

protected:
    // ��ʼ����ر���

    // �󶨵�CAN
    Struct_CAN_Manage_Object *CAN_Manage_Object;
    // �����ݰ󶨵�CAN ID, C6ϵ��0x201~0x208, GMϵ��0x205~0x20b
    Enum_CAN_Motor_ID CAN_ID;
    // ���ͻ�����
    uint8_t *CAN_Tx_Data;
    // ���ٱ�, Ĭ�ϴ�������
    // float Gearbox_Rate = 3591.0f / 187.0f;
    float Gearbox_Rate = 1.0f; // Ħ���ֲ��������
    // ���Ť��, ����ݲ�ͬ���ز�����ֵ, Ҳ�Ϳ�����Ť�ػ�����õõ�, �����Ҹо�Ӧ��û������ϲ���������������
    float Torque_Max = 16384.0f;

    // ����

    // һȦ�������̶�
    uint16_t Encoder_Num_Per_Round = 8192;
    // ������Ť��
    uint16_t Output_Max = 16384;

    // �ڲ�����

    // ��ǰʱ�̵ĵ������flag
    uint32_t Flag = 0;
    // ǰһʱ�̵ĵ������flag
    uint32_t Pre_Flag = 0;

    // ���յı�����λ��, 0~8191
    uint16_t Rx_Encoder = 0;
    // ���յ��ٶ�, rpm
    int16_t Rx_Rpm = 0;
    // ���յ�Ť��, Ŀ���Ť��, -30000~30000
    int16_t Rx_Torque = 0;
    // ���յ��¶�, ���϶�
    uint16_t Rx_Temperature = 0;

    // ֮ǰ�ı�����λ��
    uint16_t Pre_Encoder = 0;
    // �ܱ�����λ��
    int32_t Total_Encoder = 0;
    // ��Ȧ��
    int32_t Total_Round = 0;

    // ������

    // ���״̬
    Enum_CAN_Motor_Status CAN_Motor_Status = CAN_Motor_Status_DISABLE;

    // ��ǰ�ĽǶ�, rad
    float Now_Angle = 0.0f;
    // ��ǰ���ٶ�, rad/s
    float Now_Rpm = 0.0f;
    // ��ǰ��Ť��, ֱ�Ӳ��÷���ֵ
    float Now_Torque = 0.0f;
    // ��ǰ���¶�, ���϶�
    uint8_t Now_Temperature = 0;

    // д����

    // ��д����

    // ������Ʒ�ʽ
    Enum_Control_Method Control_Method = Control_Method_ANGLE;
    // Ŀ��ĽǶ�, rad
    float Target_Angle = 0.0f;
    // Ŀ����ٶ�, rad/s
    float Target_Rpm = 0.0f;
    // Ŀ���Ť��, ֱ�Ӳ��÷���ֵ
    float Target_Torque = 0.0f;
    // �����
    float Out = 0.0f;

    // �ڲ�����

    void Output();
};

/* Exported variables --------------------------------------------------------*/

/* Exported function declarations --------------------------------------------*/

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
