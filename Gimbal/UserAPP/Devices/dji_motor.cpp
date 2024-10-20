/**
 * @file dvc_motor.cpp
 * @author yssickjgd (yssickjgd@mail.ustc.edu.cn)
 * @brief �޸���USTC��CAN������������
 * @version 0.3
 * @date 2022-08-03
 * @date 2024-10-17 0.2 �޸���һЩ�������
 * @date 2024-10-19 0.3 ������mpu6050�ٶȻ�����
 *
 * @copyright USTC-RoboWalker (c) 2022
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "dji_motor.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief ����CAN���ͻ�����
 *
 * @param hcan CAN���
 * @param __CAN_ID CAN ID
 * @return uint8_t* ������ָ��
 */
uint8_t *allocate_tx_data(CAN_HandleTypeDef *hcan, Enum_CAN_Motor_ID __CAN_ID)
{
	uint8_t *tmp_tx_data_ptr;
	if (hcan == &hcan1)
	{
		switch (__CAN_ID)
		{
		case (CAN_Motor_ID_0x201):
		{
			tmp_tx_data_ptr = &(CAN1_0x200_Tx_Data[0]);
		}
		break;
		case (CAN_Motor_ID_0x202):
		{
			tmp_tx_data_ptr = &(CAN1_0x200_Tx_Data[2]);
		}
		break;
		case (CAN_Motor_ID_0x203):
		{
			tmp_tx_data_ptr = &(CAN1_0x200_Tx_Data[4]);
		}
		break;
		case (CAN_Motor_ID_0x204):
		{
			tmp_tx_data_ptr = &(CAN1_0x200_Tx_Data[6]);
		}
		break;
		case (CAN_Motor_ID_0x205):
		{
			tmp_tx_data_ptr = &(CAN1_0x1ff_Tx_Data[0]);
		}
		break;
		case (CAN_Motor_ID_0x206):
		{
			tmp_tx_data_ptr = &(CAN1_0x1ff_Tx_Data[2]);
		}
		break;
		case (CAN_Motor_ID_0x207):
		{
			tmp_tx_data_ptr = &(CAN1_0x1ff_Tx_Data[4]);
		}
		break;
		case (CAN_Motor_ID_0x208):
		{
			tmp_tx_data_ptr = &(CAN1_0x1ff_Tx_Data[6]);
		}
		break;
		case (CAN_Motor_ID_0x209):
		{
			tmp_tx_data_ptr = &(CAN1_0x2ff_Tx_Data[0]);
		}
		break;
		case (CAN_Motor_ID_0x20A):
		{
			tmp_tx_data_ptr = &(CAN1_0x2ff_Tx_Data[2]);
		}
		break;
		case (CAN_Motor_ID_0x20B):
		{
			tmp_tx_data_ptr = &(CAN1_0x2ff_Tx_Data[4]);
		}
		break;
		}
	}
	else if (hcan == &hcan2)
	{
		switch (__CAN_ID)
		{
		case (CAN_Motor_ID_0x201):
		{
			tmp_tx_data_ptr = &(CAN2_0x200_Tx_Data[0]);
		}
		break;
		case (CAN_Motor_ID_0x202):
		{
			tmp_tx_data_ptr = &(CAN2_0x200_Tx_Data[2]);
		}
		break;
		case (CAN_Motor_ID_0x203):
		{
			tmp_tx_data_ptr = &(CAN2_0x200_Tx_Data[4]);
		}
		break;
		case (CAN_Motor_ID_0x204):
		{
			tmp_tx_data_ptr = &(CAN2_0x200_Tx_Data[6]);
		}
		break;
		case (CAN_Motor_ID_0x205):
		{
			tmp_tx_data_ptr = &(CAN2_0x1ff_Tx_Data[0]);
		}
		break;
		case (CAN_Motor_ID_0x206):
		{
			tmp_tx_data_ptr = &(CAN2_0x1ff_Tx_Data[2]);
		}
		break;
		case (CAN_Motor_ID_0x207):
		{
			tmp_tx_data_ptr = &(CAN2_0x1ff_Tx_Data[4]);
		}
		break;
		case (CAN_Motor_ID_0x208):
		{
			tmp_tx_data_ptr = &(CAN2_0x1ff_Tx_Data[6]);
		}
		break;
		case (CAN_Motor_ID_0x209):
		{
			tmp_tx_data_ptr = &(CAN2_0x2ff_Tx_Data[0]);
		}
		break;
		case (CAN_Motor_ID_0x20A):
		{
			tmp_tx_data_ptr = &(CAN2_0x2ff_Tx_Data[2]);
		}
		break;
		case (CAN_Motor_ID_0x20B):
		{
			tmp_tx_data_ptr = &(CAN2_0x2ff_Tx_Data[4]);
		}
		break;
		}
	}
	return (tmp_tx_data_ptr);
}

/**
 * @brief �����ʼ��
 *
 * @param hcan �󶨵�CAN����
 * @param __CAN_ID �󶨵�CAN ID
 * @param __Control_Method ������Ʒ�ʽ, Ĭ�ϽǶ�
 * @param __Encoder_Offset ������ƫ��, Ĭ��0
 * @param __Rpm_Max ����ٶ�, ����ݲ�ͬ���ز�����ֵ, Ҳ�Ϳ�������õõ�, �����Ҹо�Ӧ��û������ϲ���������������
 */
void Class_Motor_GM6020::Init(CAN_HandleTypeDef *hcan, Enum_CAN_Motor_ID __CAN_ID, Enum_Control_Method __Control_Method, int32_t __Encoder_Offset, float __Rpm_Max)
{
	if (hcan->Instance == CAN1)
	{
		CAN_Manage_Object = &CAN1_Manage_Object;
	}
	else if (hcan->Instance == CAN2)
	{
		CAN_Manage_Object = &CAN2_Manage_Object;
	}
	CAN_ID = __CAN_ID;
	Control_Method = __Control_Method;
	Encoder_Offset = __Encoder_Offset;
	Rpm_Max = __Rpm_Max;
	CAN_Tx_Data = allocate_tx_data(hcan, __CAN_ID);
}

/**
 * @brief ������������CAN���߷��ͻ�����
 *
 */
void Class_Motor_GM6020::Output()
{
	CAN_Tx_Data[0] = (int16_t)Out >> 8;
	CAN_Tx_Data[1] = (int16_t)Out;
}

/**
 * @brief ��ȡ��������ѹ
 *
 * @return uint16_t ��������ѹ
 */
uint16_t Class_Motor_GM6020::Get_Output_Max()
{
	return (Output_Max);
}

/**
 * @brief ��ȡ���״̬
 *
 * @return Enum_CAN_Motor_Status ���״̬
 */
Enum_CAN_Motor_Status Class_Motor_GM6020::Get_CAN_Motor_Status()
{
	return (CAN_Motor_Status);
}

/**
 * @brief ��ȡ��ǰ�ĽǶ�, ��
 *
 * @return float ��ǰ�ĽǶ�, ��
 */
float Class_Motor_GM6020::Get_Now_Angle()
{
	return (Now_Angle);
}

/**
 * @brief ��ȡ��ǰ���ٶ�, rpm
 *
 * @return float ��ǰ���ٶ�, rpm
 */
float Class_Motor_GM6020::Get_Now_Rpm()
{
	return (Now_Rpm);
}

/**
 * @brief ��ȡ��ǰ��MPU�ٶ�, rpm
 *
 * @return ��ǰ��MPU�ٶ�, rpm
 */
float Class_Motor_GM6020::Get_Now_MPU_Rpm()
{
	return (Now_MPU_Rpm);
}

/**
 * @brief ��ȡ��ǰ��Ť��, ֱ�Ӳ��÷���ֵ
 *
 * @return ��ǰ��Ť��, ֱ�Ӳ��÷���ֵ
 */
float Class_Motor_GM6020::Get_Now_Torque()
{
	return (Now_Torque);
}

/**
 * @brief ��ȡ��ǰ���¶�, ���϶�
 *
 * @return uint8_t ��ǰ���¶�, ���϶�
 */
uint8_t Class_Motor_GM6020::Get_Now_Temperature()
{
	return (Now_Temperature);
}

/**
 * @brief ��ȡ������Ʒ�ʽ
 *
 * @return Enum_Control_Method ������Ʒ�ʽ
 */
Enum_Control_Method Class_Motor_GM6020::Get_Control_Method()
{
	return (Control_Method);
}

/**
 * @brief ��ȡĿ��ĽǶ�, ��
 *
 * @return float Ŀ��ĽǶ�, ��
 */
float Class_Motor_GM6020::Get_Target_Angle()
{
	return (Target_Angle);
}

/**
 * @brief ��ȡĿ����ٶ�, rpm
 *
 * @return float Ŀ����ٶ�, rpm
 */
float Class_Motor_GM6020::Get_Target_Rpm()
{
	return (Target_Rpm);
}

/**
 * @brief ��ȡĿ���Ť��, ֱ�Ӳ��÷���ֵ
 *
 * @return float Ŀ���Ť��, ֱ�Ӳ��÷���ֵ
 */
float Class_Motor_GM6020::Get_Target_Torque()
{
	return (Target_Torque);
}

/**
 * @brief ��ȡ�����
 *
 * @return float �����
 */
float Class_Motor_GM6020::Get_Out()
{
	return (Out);
}

/**
 * @brief �趨������Ʒ�ʽ
 *
 * @param __Control_Method ������Ʒ�ʽ
 */
void Class_Motor_GM6020::Set_Control_Method(Enum_Control_Method __Control_Method)
{
	Control_Method = __Control_Method;
}

/**
 * @brief �趨Ŀ��ĽǶ�, ��
 *
 * @param __Target_Angle Ŀ��ĽǶ�, ��
 */
void Class_Motor_GM6020::Set_Target_Angle(float __Target_Angle)
{
	Target_Angle = __Target_Angle;
}

/**
 * @brief �趨Ŀ����ٶ�, rpm
 *
 * @param __Target_Rpm Ŀ����ٶ�, rpm
 */
void Class_Motor_GM6020::Set_Target_Rpm(float __Target_Rpm)
{
	Target_Rpm = __Target_Rpm;
}

/**
 * @brief �趨Ŀ���Ť��, ֱ�Ӳ��÷���ֵ
 *
 * @param __Target_Torque Ŀ���Ť��, ֱ�Ӳ��÷���ֵ
 */
void Class_Motor_GM6020::Set_Target_Torque(float __Target_Torque)
{
	Target_Torque = __Target_Torque;
}

/**
 * @brief �趨�����
 *
 * @param __Output_Voltage �����
 */
void Class_Motor_GM6020::Set_Out(float __Out)
{
	Out = __Out;
}

/**
 * @brief �趨MPU��ȡ��ת�٣�rpm
 *
 * @param __MPU_Rpm ��MPU��ȡ��ת�٣�rad/s
 * @return * void
 */
void Class_Motor_GM6020::Set_MPU_Rpm(float __MPU_Rads)
{
	// Now_MPU_Rpm = __MPU_Rads * RADPS_TO_RPM;
	Now_MPU_Rpm = __MPU_Rads / 16.4 * 60 / 360;
}

/**
 * @brief ����������
 *
 * @param none
 */
void Class_Motor_GM6020::Set_Zero()
{
	Out = 0.f;
	Output();
}

/**
 * @brief CANͨ�Ž��ջص�����
 *
 * @param Rx_Data ���յ�����
 */
void Class_Motor_GM6020::CAN_RxCpltCallback(uint8_t *Rx_Data)
{
	int16_t delta_encoder;

	Flag += 1;

	Pre_Encoder = Rx_Encoder;

	Rx_Encoder = (Rx_Data[0] << 8) | Rx_Data[1];
	Rx_Rpm = (Rx_Data[2] << 8) | Rx_Data[3];
	Rx_Torque = (Rx_Data[4] << 8) | Rx_Data[5];
	Rx_Temperature = Rx_Data[6];

	delta_encoder = Rx_Encoder - Pre_Encoder;
	if (delta_encoder < -4096)
	{
		// ������ת����һȦ
		Total_Round++;
	}
	else if (delta_encoder > 4096)
	{
		// ������ת����һȦ
		Total_Round--;
	}
	Total_Encoder = Total_Round * Encoder_Num_Per_Round + Rx_Encoder + Encoder_Offset;

	Now_Angle = (float)Total_Encoder / (float)Encoder_Num_Per_Round * 360.f;
	Now_Rpm = (float)Rx_Rpm;
	Now_Torque = Rx_Torque;
	Now_Temperature = Rx_Temperature;
}

/**
 * @brief TIM��ʱ���ж϶��ڼ�����Ƿ���
 *
 */
void Class_Motor_GM6020::TIM_Alive_PeriodElapsedCallback()
{
	// �жϸ�ʱ������Ƿ���չ��������
	if (Flag == Pre_Flag)
	{
		// ����Ͽ�����
		CAN_Motor_Status = CAN_Motor_Status_DISABLE;
		PID_Angle.Set_Integral_Error(0.0f);
		PID_Rpm.Set_Integral_Error(0.0f);
		PID_Torque.Set_Integral_Error(0.0f);
	}
	else
	{
		// �����������
		CAN_Motor_Status = CAN_Motor_Status_ENABLE;
	}
	Pre_Flag = Flag;
}

/**
 * @brief TIM��ʱ���жϼ���ص�����
 *
 */
void Class_Motor_GM6020::TIM_PID_PeriodElapsedCallback()
{
	switch (Control_Method)
	{
	case (Control_Method_OPENLOOP):
	{
		// Ĭ�Ͽ����ٶȿ���
		Set_Out(Target_Rpm / Rpm_Max * Output_Max);
	}
	break;
	case (Control_Method_TORQUE):
	{
		PID_Torque.Set_Target(Target_Torque);
		PID_Torque.Set_Now(Now_Torque);
		PID_Torque.TIM_Adjust_PeriodElapsedCallback();

		Set_Out(PID_Torque.Get_Out());
	}
	break;
	case (Control_Method_RPM):
	{
		PID_Rpm.Set_Target(Target_Rpm);
		PID_Rpm.Set_Now(Now_Rpm);
		PID_Rpm.TIM_Adjust_PeriodElapsedCallback();

		Target_Torque = PID_Rpm.Get_Out();

		PID_Torque.Set_Target(Target_Torque);
		PID_Torque.Set_Now(Now_Torque);
		PID_Torque.TIM_Adjust_PeriodElapsedCallback();

		Set_Out(PID_Torque.Get_Out());
	}
	break;
	case (Control_Method_ANGLE):
	{
		PID_Angle.Set_Target(Target_Angle);
		PID_Angle.Set_Now(Now_Angle);
		PID_Angle.TIM_Adjust_PeriodElapsedCallback();

		Target_Rpm = PID_Angle.Get_Out();

		// ʹ�õ����������
		// PID_Rpm.Set_Target(Target_Rpm);
		// PID_Rpm.Set_Now(Now_Rpm);
		// PID_Rpm.TIM_Adjust_PeriodElapsedCallback();

		PID_MPU_Rpm.Set_Target(Target_Rpm);
		PID_MPU_Rpm.Set_Now(Now_MPU_Rpm);
		PID_MPU_Rpm.TIM_Adjust_PeriodElapsedCallback();

		//        Target_Torque = PID_Rpm.Get_Out();

		//        PID_Torque.Set_Target(Target_Torque);
		//        PID_Torque.Set_Now(Now_Torque);
		//        PID_Torque.TIM_Adjust_PeriodElapsedCallback();

		//        Set_Out(PID_Torque.Get_Out());

		// Set_Out(PID_Rpm.Get_Out());
		Set_Out(PID_MPU_Rpm.Get_Out());
	}
	break;
	default:
	{
		Set_Out(0.0f);
	}
	break;
	}
	Output();
}

/**
 * @brief �����ʼ��
 *
 * @param hcan CAN���
 * @param __CAN_ID CAN ID
 * @param __Control_Method ������Ʒ�ʽ, Ĭ�ϽǶ�
 * @param __Gearbox_Rate ��������ٱ�, Ĭ��Ϊԭװ������, ���ȥ���������ֵ��Ϊ1
 * @param __Torque_Max ���Ť��, ����ݲ�ͬ���ز�����ֵ, Ҳ�Ϳ�����Ť�ػ�����õõ�, �����Ҹо�Ӧ��û������ϲ���������������
 */
void Class_Motor_C610::Init(CAN_HandleTypeDef *hcan, Enum_CAN_Motor_ID __CAN_ID, Enum_Control_Method __Control_Method, float __Gearbox_Rate, float __Torque_Max)
{
	if (hcan->Instance == CAN1)
	{
		CAN_Manage_Object = &CAN1_Manage_Object;
	}
	else if (hcan->Instance == CAN2)
	{
		CAN_Manage_Object = &CAN2_Manage_Object;
	}
	CAN_ID = __CAN_ID;
	Control_Method = __Control_Method;
	Gearbox_Rate = __Gearbox_Rate;
	Torque_Max = __Torque_Max;
	CAN_Tx_Data = allocate_tx_data(hcan, __CAN_ID);
}

/**
 * @brief ������������CAN���߷��ͻ�����
 *
 */
void Class_Motor_C610::Output()
{
	CAN_Tx_Data[0] = (int16_t)Out >> 8;
	CAN_Tx_Data[1] = (int16_t)Out;
}

/**
 * @brief ��ȡ����������
 *
 * @return uint16_t ����������
 */
uint16_t Class_Motor_C610::Get_Output_Max()
{
	return (Output_Max);
}

/**
 * @brief ��ȡ���״̬
 *
 * @return Enum_CAN_Motor_Status ���״̬
 */
Enum_CAN_Motor_Status Class_Motor_C610::Get_CAN_Motor_Status()
{
	return (CAN_Motor_Status);
}

/**
 * @brief ��ȡ��ǰ�ĽǶ�, ��
 *
 * @return float ��ǰ�ĽǶ�, ��
 */
float Class_Motor_C610::Get_Now_Angle()
{
	return (Now_Angle);
}

/**
 * @brief ��ȡ��ǰ���ٶ�, rpm
 *
 * @return float ��ǰ���ٶ�, rpm
 */
float Class_Motor_C610::Get_Now_Rpm()
{
	return (Now_Rpm);
}

/**
 * @brief ��ȡ��ǰ��Ť��, ֱ�Ӳ��÷���ֵ
 *
 * @return ��ǰ��Ť��, ֱ�Ӳ��÷���ֵ
 */
float Class_Motor_C610::Get_Now_Torque()
{
	return (Now_Torque);
}

/**
 * @brief ��ȡ��ǰ���¶�, ���϶�
 *
 * @return uint8_t ��ǰ���¶�, ���϶�
 */
uint8_t Class_Motor_C610::Get_Now_Temperature()
{
	return (Now_Temperature);
}

/**
 * @brief ��ȡ������Ʒ�ʽ
 *
 * @return Enum_Control_Method ������Ʒ�ʽ
 */
Enum_Control_Method Class_Motor_C610::Get_Control_Method()
{
	return (Control_Method);
}

/**
 * @brief ��ȡĿ��ĽǶ�, ��
 *
 * @return float Ŀ��ĽǶ�, ��
 */
float Class_Motor_C610::Get_Target_Angle()
{
	return (Target_Angle);
}

/**
 * @brief ��ȡĿ����ٶ�, rpm
 *
 * @return float Ŀ����ٶ�, rpm
 */
float Class_Motor_C610::Get_Target_Rpm()
{
	return (Target_Rpm);
}

/**
 * @brief ��ȡĿ���Ť��, ֱ�Ӳ��÷���ֵ
 *
 * @return float Ŀ���Ť��, ֱ�Ӳ��÷���ֵ
 */
float Class_Motor_C610::Get_Target_Torque()
{
	return (Target_Torque);
}

/**
 * @brief ��ȡ�����
 *
 * @return float �����
 */
float Class_Motor_C610::Get_Out()
{
	return (Out);
}

/**
 * @brief �趨������Ʒ�ʽ
 *
 * @param __Control_Method ������Ʒ�ʽ
 */
void Class_Motor_C610::Set_Control_Method(Enum_Control_Method __Control_Method)
{
	Control_Method = __Control_Method;
}

/**
 * @brief �趨Ŀ��ĽǶ�, ��
 *
 * @param __Target_Angle Ŀ��ĽǶ�, ��
 */
void Class_Motor_C610::Set_Target_Angle(float __Target_Angle)
{
	Target_Angle = __Target_Angle;
}

/**
 * @brief �趨Ŀ����ٶ�, rpm
 *
 * @param __Target_Rpm Ŀ����ٶ�, rpm
 */
void Class_Motor_C610::Set_Target_Rpm(float __Target_Rpm)
{
	Target_Rpm = __Target_Rpm;
}

/**
 * @brief �趨Ŀ���Ť��, ֱ�Ӳ��÷���ֵ
 *
 * @param __Target_Torque Ŀ���Ť��, ֱ�Ӳ��÷���ֵ
 */
void Class_Motor_C610::Set_Target_Torque(float __Target_Torque)
{
	Target_Torque = __Target_Torque;
}

/**
 * @brief �趨�����
 *
 * @param __Output_Voltage �����
 */
void Class_Motor_C610::Set_Out(float __Out)
{
	Out = __Out;
}

/**
 * @brief ����������
 *
 * @param none
 */
void Class_Motor_C610::Set_Zero()
{
	Out = 0.f;
	Output();
}

/**
 * @brief CANͨ�Ž��ջص�����
 *
 * @param Rx_Data ���յ�����
 */
void Class_Motor_C610::CAN_RxCpltCallback(uint8_t *Rx_Data)
{
	int16_t delta_encoder;

	Flag += 1;

	Pre_Encoder = Rx_Encoder;

	Rx_Encoder = (Rx_Data[0] << 8) | Rx_Data[1];
	Rx_Rpm = (Rx_Data[2] << 8) | Rx_Data[3];
	Rx_Torque = (Rx_Data[4] << 8) | Rx_Data[5];
	Rx_Temperature = Rx_Data[6];

	delta_encoder = Rx_Encoder - Pre_Encoder;
	if (delta_encoder < -4096)
	{
		// ������ת����һȦ
		Total_Round++;
	}
	else if (delta_encoder > 4096)
	{
		// ������ת����һȦ
		Total_Round--;
	}
	Total_Encoder = Total_Round * Encoder_Num_Per_Round + Rx_Encoder;

	Now_Angle = (float)Total_Encoder / (float)Encoder_Num_Per_Round * 360.0f / Gearbox_Rate;
	Now_Rpm = (float)Rx_Rpm / Gearbox_Rate;
	Now_Torque = Rx_Torque;
	Now_Temperature = Rx_Temperature;
}

/**
 * @brief TIM��ʱ���ж϶��ڼ�����Ƿ���
 *
 */
void Class_Motor_C610::TIM_Alive_PeriodElapsedCallback()
{
	// �жϸ�ʱ������Ƿ���չ��������
	if (Flag == Pre_Flag)
	{
		// ����Ͽ�����
		CAN_Motor_Status = CAN_Motor_Status_DISABLE;
		PID_Angle.Set_Integral_Error(0.0f);
		PID_Rpm.Set_Integral_Error(0.0f);
	}
	else
	{
		// �����������
		CAN_Motor_Status = CAN_Motor_Status_ENABLE;
	}
	Pre_Flag = Flag;
}

/**
 * @brief TIM��ʱ���жϼ���ص�����
 *
 */
void Class_Motor_C610::TIM_PID_PeriodElapsedCallback()
{
	switch (Control_Method)
	{
	case (Control_Method_OPENLOOP):
	{
		// Ĭ�Ͽ���Ť�ؿ���
		Set_Out(Target_Torque / Torque_Max * Output_Max);
	}
	break;
	case (Control_Method_TORQUE):
	{
		// Ĭ�ϱջ�Ť�ؿ���
		Set_Out(Target_Torque / Torque_Max * Output_Max);
	}
	break;
	case (Control_Method_RPM):
	{
		PID_Rpm.Set_Target(Target_Rpm);
		PID_Rpm.Set_Now(Now_Rpm);
		PID_Rpm.TIM_Adjust_PeriodElapsedCallback();

		Set_Out(PID_Rpm.Get_Out());
	}
	break;
	case (Control_Method_ANGLE):
	{
		PID_Angle.Set_Target(Target_Angle);
		PID_Angle.Set_Now(Now_Angle);
		PID_Angle.TIM_Adjust_PeriodElapsedCallback();

		Target_Rpm = PID_Angle.Get_Out();

		Now_Rpm = PID_Rpm.LowPass_Filter.f(Now_Rpm);

		PID_Rpm.Set_Target(Target_Rpm);
		PID_Rpm.Set_Now(Now_Rpm);
		PID_Rpm.TIM_Adjust_PeriodElapsedCallback();

		Set_Out(PID_Rpm.Get_Out());
	}
	break;
	default:
	{
		Set_Out(0.0f);
	}
	break;
	}
	Output();
}

/**
 * @brief �����ʼ��
 *
 * @param hcan CAN���
 * @param __CAN_ID CAN ID
 * @param __Control_Method ������Ʒ�ʽ, Ĭ���ٶ�
 * @param __Gearbox_Rate ��������ٱ�, Ĭ��Ϊԭװ������, ���ȥ���������ֵ��Ϊ1
 * @param __Torque_Max ���Ť��, ����ݲ�ͬ���ز�����ֵ, Ҳ�Ϳ�����Ť�ػ�����õõ�, �����Ҹо�Ӧ��û������ϲ���������������
 */
void Class_Motor_C620::Init(CAN_HandleTypeDef *hcan, Enum_CAN_Motor_ID __CAN_ID, Enum_Control_Method __Control_Method, float __Gearbox_Rate, float __Torque_Max)
{
	if (hcan->Instance == CAN1)
	{
		CAN_Manage_Object = &CAN1_Manage_Object;
	}
	else if (hcan->Instance == CAN2)
	{
		CAN_Manage_Object = &CAN2_Manage_Object;
	}
	CAN_ID = __CAN_ID;
	Control_Method = __Control_Method;
	Gearbox_Rate = __Gearbox_Rate;
	Torque_Max = __Torque_Max;
	CAN_Tx_Data = allocate_tx_data(hcan, __CAN_ID);
}

/**
 * @brief ������������CAN���߷��ͻ�����
 *
 */
void Class_Motor_C620::Output()
{
	CAN_Tx_Data[0] = (int16_t)Out >> 8;
	CAN_Tx_Data[1] = (int16_t)Out;
}

/**
 * @brief ��ȡ����������
 *
 * @return uint16_t ����������
 */
uint16_t Class_Motor_C620::Get_Output_Max()
{
	return (Output_Max);
}

/**
 * @brief ��ȡ���״̬
 *
 * @return Enum_CAN_Motor_Status ���״̬
 */
Enum_CAN_Motor_Status Class_Motor_C620::Get_CAN_Motor_Status()
{
	return (CAN_Motor_Status);
}

/**
 * @brief ��ȡ��ǰ�ĽǶ�, ��
 *
 * @return float ��ǰ�ĽǶ�, ��
 */
float Class_Motor_C620::Get_Now_Angle()
{
	return (Now_Angle);
}

/**
 * @brief ��ȡ��ǰ���ٶ�, rpm
 *
 * @return float ��ǰ���ٶ�, rpm
 */
float Class_Motor_C620::Get_Now_Rpm()
{
	return (Now_Rpm);
}

/**
 * @brief ��ȡ��ǰ��Ť��, ֱ�Ӳ��÷���ֵ
 *
 * @return ��ǰ��Ť��, ֱ�Ӳ��÷���ֵ
 */
float Class_Motor_C620::Get_Now_Torque()
{
	return (Now_Torque);
}

/**
 * @brief ��ȡ��ǰ���¶�, ���϶�
 *
 * @return uint8_t ��ǰ���¶�, ���϶�
 */
uint8_t Class_Motor_C620::Get_Now_Temperature()
{
	return (Now_Temperature);
}

/**
 * @brief ��ȡ������Ʒ�ʽ
 *
 * @return Enum_Control_Method ������Ʒ�ʽ
 */
Enum_Control_Method Class_Motor_C620::Get_Control_Method()
{
	return (Control_Method);
}

/**
 * @brief ��ȡĿ��ĽǶ�, ��
 *
 * @return float Ŀ��ĽǶ�, ��
 */
float Class_Motor_C620::Get_Target_Angle()
{
	return (Target_Angle);
}

/**
 * @brief ��ȡĿ����ٶ�, rpm
 *
 * @return float Ŀ����ٶ�, rpm
 */
float Class_Motor_C620::Get_Target_Rpm()
{
	return (Target_Rpm);
}

/**
 * @brief ��ȡĿ���Ť��, ֱ�Ӳ��÷���ֵ
 *
 * @return float Ŀ���Ť��, ֱ�Ӳ��÷���ֵ
 */
float Class_Motor_C620::Get_Target_Torque()
{
	return (Target_Torque);
}

/**
 * @brief ��ȡ�����
 *
 * @return float �����
 */
float Class_Motor_C620::Get_Out()
{
	return (Out);
}

/**
 * @brief �趨������Ʒ�ʽ
 *
 * @param __Control_Method ������Ʒ�ʽ
 */
void Class_Motor_C620::Set_Control_Method(Enum_Control_Method __Control_Method)
{
	Control_Method = __Control_Method;
}

/**
 * @brief �趨Ŀ��ĽǶ�, ��
 *
 * @param __Target_Angle Ŀ��ĽǶ�, ��
 */
void Class_Motor_C620::Set_Target_Angle(float __Target_Angle)
{
	Target_Angle = __Target_Angle;
}

/**
 * @brief �趨Ŀ����ٶ�, rpm
 *
 * @param __Target_Rpm Ŀ����ٶ�, rpm
 */
void Class_Motor_C620::Set_Target_Rpm(float __Target_Rpm)
{
	Target_Rpm = __Target_Rpm;
}

/**
 * @brief �趨Ŀ���Ť��, ֱ�Ӳ��÷���ֵ
 *
 * @param __Target_Torque Ŀ���Ť��, ֱ�Ӳ��÷���ֵ
 */
void Class_Motor_C620::Set_Target_Torque(float __Target_Torque)
{
	Target_Torque = __Target_Torque;
}

/**
 * @brief �趨�����
 *
 * @param __Output_Voltage �����
 */
void Class_Motor_C620::Set_Out(float __Out)
{
	Out = __Out;
}

/**
 * @brief ����������
 *
 * @param none
 */
void Class_Motor_C620::Set_Zero()
{
	Out = 0.f;
	Output();
}

/**
 * @brief CANͨ�Ž��ջص�����
 *
 * @param Rx_Data ���յ�����
 */
void Class_Motor_C620::CAN_RxCpltCallback(uint8_t *Rx_Data)
{
	int16_t delta_encoder;

	Flag += 1;

	Pre_Encoder = Rx_Encoder;

	Rx_Encoder = (Rx_Data[0] << 8) | Rx_Data[1];
	Rx_Rpm = (Rx_Data[2] << 8) | Rx_Data[3];
	Rx_Torque = (Rx_Data[4] << 8) | Rx_Data[5];
	Rx_Temperature = Rx_Data[6];

	delta_encoder = Rx_Encoder - Pre_Encoder;
	if (delta_encoder < -4096)
	{
		// ������ת����һȦ
		Total_Round++;
	}
	else if (delta_encoder > 4096)
	{
		// ������ת����һȦ
		Total_Round--;
	}
	Total_Encoder = Total_Round * Encoder_Num_Per_Round + Rx_Encoder;

	Now_Angle = (float)Total_Encoder / (float)Encoder_Num_Per_Round * 360.0f / Gearbox_Rate;
	// Now_Rpm = (float)Rx_Rpm / Gearbox_Rate;
	Now_Rpm = (float)Rx_Rpm;
	Now_Torque = Rx_Torque;
	Now_Temperature = Rx_Temperature;
}

/**
 * @brief TIM��ʱ���ж϶��ڼ�����Ƿ���
 *
 */
void Class_Motor_C620::TIM_Alive_PeriodElapsedCallback()
{
	// �жϸ�ʱ������Ƿ���չ��������
	if (Flag == Pre_Flag)
	{
		// ����Ͽ�����
		CAN_Motor_Status = CAN_Motor_Status_DISABLE;
		PID_Angle.Set_Integral_Error(0.0f);
		PID_Rpm.Set_Integral_Error(0.0f);
	}
	else
	{
		// �����������
		CAN_Motor_Status = CAN_Motor_Status_ENABLE;
	}
	Pre_Flag = Flag;
}

/**
 * @brief TIM��ʱ���жϼ���ص�����
 *
 */
void Class_Motor_C620::TIM_PID_PeriodElapsedCallback()
{
	switch (Control_Method)
	{
	case (Control_Method_OPENLOOP):
	{
		// Ĭ�Ͽ���Ť�ؿ���
		Set_Out(Target_Torque / Torque_Max * Output_Max);
	}
	break;
	case (Control_Method_TORQUE):
	{
		// Ĭ�ϱջ�Ť�ؿ���
		Set_Out(Target_Torque / Torque_Max * Output_Max);
	}
	break;
	case (Control_Method_RPM):
	{
		PID_Rpm.Set_Target(Target_Rpm);
		PID_Rpm.Set_Now(Now_Rpm);
		PID_Rpm.TIM_Adjust_PeriodElapsedCallback();

		Set_Out(PID_Rpm.Get_Out());
	}
	break;
	case (Control_Method_ANGLE):
	{
		PID_Angle.Set_Target(Target_Angle);
		PID_Angle.Set_Now(Now_Angle);
		PID_Angle.TIM_Adjust_PeriodElapsedCallback();

		Target_Rpm = PID_Angle.Get_Out();

		PID_Rpm.Set_Target(Target_Rpm);
		PID_Rpm.Set_Now(Now_Rpm);
		PID_Rpm.TIM_Adjust_PeriodElapsedCallback();

		Set_Out(PID_Rpm.Get_Out());
	}
	break;
	default:
	{
		Set_Out(0.0f);
	}
	break;
	}
	Output();
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
