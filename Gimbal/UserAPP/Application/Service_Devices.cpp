/* Includes ------------------------------------------------------------------*/
#include "Service_Devices.h"
/* Private define ------------------------------------------------------------*/
TaskHandle_t Gimbal_Control_Handle;
TaskHandle_t DR16_Handle;
TaskHandle_t Shoot_Control_Handle;
TaskHandle_t PitchYaw_Control_Handle;
TaskHandle_t MotorSet_Handle;
TaskHandle_t LedSet_Handle;
TaskHandle_t SerialTransmit_Handle;
SemaphoreHandle_t rc_protect_Handle = NULL;
/* Private variables ---------------------------------------------------------*/
float Left_Fric_Target_Rpm, Left_Fric_Now_Rpm;
float Right_Fric_Target_Rpm, Right_Fric_Now_Rpm;
float Dial_Target_Rpm, Dial_Now_Rpm, Dial_Target_Angle, Dial_Now_Angle;
float Yaw_Target_Rpm, Yaw_Now_Rpm, Yaw_Target_Angle, Yaw_Now_Angle;
float Pitch_Target_Rpm, Pitch_Now_Rpm, Pitch_Target_Angle, Pitch_Now_Angle;
bool dr16_flag = true;
uint8_t dial_flag;
float tmp_Target_Rpm;
float tmp_Target_Angle;
/* Private function declarations ---------------------------------------------*/
void Gimbal_Control(void *arg);
void DR16_Test(void *arg);
void Shoot_Control(void *arg);
void PitchYaw_Control(void *arg);
void Motor_Set(void *arg);
void Led_Set(void *arg);
void Serial_Transmit(void *arg); // 串口发送
/* Exported devices ----------------------------------------------------------*/
/* Motor & ESC & Other actuators*/
/* Remote control */

/* IMU & NUC & Other sensors */

/* Other boards */

/* Function prototypes -------------------------------------------------------*/
/**
 * @brief  Initialization of device management service
 * @param  None.
 * @return None.
 */
void Service_Devices_Init(void)
{
	rc_protect_Handle = xSemaphoreCreateBinary();
	xTaskCreate(Gimbal_Control, "Gimbal_Control", Normal_Stack_Size, NULL, PrioritySuperHigh, &Gimbal_Control_Handle);
	xTaskCreate(DR16_Test, "DR16_Test", Normal_Stack_Size, NULL, PriorityHigh, &DR16_Handle);
	xTaskCreate(Shoot_Control, "Shoot_Control", Normal_Stack_Size, NULL, PriorityNormal, &Shoot_Control_Handle);
	xTaskCreate(PitchYaw_Control, "PitchYaw_Control", Normal_Stack_Size, NULL, PriorityNormal, &PitchYaw_Control_Handle);
	xTaskCreate(Motor_Set, "Motor_Set", Normal_Stack_Size, NULL, PriorityNormal, &MotorSet_Handle);
	xTaskCreate(Serial_Transmit, "Serial_Send", Small_Stack_Size, NULL, PriorityNormal, &SerialTransmit_Handle);
	xTaskCreate(Led_Set, "Led_Set", Small_Stack_Size, NULL, PriorityLow, &LedSet_Handle);
}

void Gimbal_Control(void *arg)
{
	for (;;)
	{
		if (xSemaphoreTake(rc_protect_Handle, portMAX_DELAY) == pdTRUE)
		{
			dr16_flag = false;
			/* ------- 设置各电机输出为0 ------ */
			Left_Fric.Set_Zero();
			Right_Fric.Set_Zero();
			Dial_Motor.Set_Zero();
			Pitch_Motor.Set_Zero();
			Yaw_Motor.Set_Zero();
			/* ------- PID电机输出置0 ------ */
			// Dial_Motor.Init(&hcan1, CAN_Motor_ID_0x201, Control_Method_RPM);
			// Pitch_Motor.Init(&hcan1, CAN_Motor_ID_0x206, Control_Method_RPM);
			// Yaw_Motor.Init(&hcan2, CAN_Motor_ID_0x209, Control_Method_RPM);

			Dial_Motor.PID_Angle.Set_Integral_Error(0.0f);
			Dial_Motor.PID_Rpm.Set_Integral_Error(0.0f);
			Left_Fric.PID_Rpm.Set_Integral_Error(0.0f);
			Right_Fric.PID_Rpm.Set_Integral_Error(0.0f);
			Pitch_Motor.PID_Torque.Set_Integral_Error(0.0f);
			Pitch_Motor.PID_Angle.Set_Integral_Error(0.0f);
			Pitch_Motor.PID_Rpm.Set_Integral_Error(0.0f);
			Yaw_Motor.PID_Torque.Set_Integral_Error(0.0f);
			Yaw_Motor.PID_Angle.Set_Integral_Error(0.0f);
			Yaw_Motor.PID_Rpm.Set_Integral_Error(0.0f);

			Left_Fric.Set_Target_Rpm(0.f);
			Right_Fric.Set_Target_Rpm(0.f);
			// Dial_Motor.Set_Target_Rpm(0.f);
			// Pitch_Motor.Set_Target_Rpm(0.f);
			// Yaw_Motor.Set_Target_Rpm(0.f);

			// /* ------ 各电机的PID计算 ------ */
			// Left_Fric.TIM_PID_PeriodElapsedCallback();
			// Right_Fric.TIM_PID_PeriodElapsedCallback();
			// Dial_Motor.TIM_PID_PeriodElapsedCallback();
			// Pitch_Motor.TIM_PID_PeriodElapsedCallback();
			// Yaw_Motor.TIM_PID_PeriodElapsedCallback();

			/* ------ 各电机的CAN发送 ------ */
			TIM_CAN_PeriodElapsedCallback();
		}
		else
		{
			dr16_flag = true;
		}

		// osDelay(1);
	}
}

void DR16_Test(void *arg)
{
	for (;;)
	{
		DR16.TIM_100ms_Alive_PeriodElapsedCallback();
		if (DR16.Get_Status() == DR16_Status_DISABLE)
		{
			xSemaphoreGive(rc_protect_Handle);
		}
		else
		{
			dr16_flag = true;
		}

		osDelay(100);
	}
}

void Shoot_Control(void *arg)
{
	/* Cache for Task */

	/* Pre-Load for task */

	/* Infinite loop */

	for (;;)
	{
		if (dr16_flag)
		{
			/* ------ DR16数据处理 ------ */
			DR16.TIM_1ms_Calculate_PeriodElapsedCallback();

			/* ------ 摩擦轮设置 ------ */
			if (DR16.Get_Left_Switch() == 0)
			{
				Left_Fric.Set_Target_Rpm(6000.f);
				Right_Fric.Set_Target_Rpm(-6000.f);
			}
			else
			{
				Left_Fric.Set_Target_Rpm(0.f);
				Right_Fric.Set_Target_Rpm(0.f);
			}
			Left_Fric_Target_Rpm = Left_Fric.Get_Target_Rpm();
			Left_Fric_Now_Rpm = Left_Fric.Get_Now_Rpm();
			Right_Fric_Target_Rpm = Right_Fric.Get_Target_Rpm();
			Right_Fric_Now_Rpm = Right_Fric.Get_Now_Rpm();

			/* ------ 拨盘设置 ------ */
			if (DR16.Get_Right_Switch() == 0)
			{
				// 单发模式
				if (dial_flag != 1)
				{
					dial_flag = 1;
					Dial_Target_Angle = Dial_Motor.Get_Now_Angle();
					Dial_Target_Angle += 40.f;
					Dial_Motor.Set_Target_Angle(Dial_Target_Angle);
					Dial_Now_Angle = Dial_Motor.Get_Now_Angle();
					Dial_Target_Angle = Dial_Motor.Get_Now_Angle();
					Dial_Target_Rpm = Dial_Motor.Get_Target_Rpm();
					Dial_Now_Rpm = Dial_Motor.Get_Now_Rpm();
					
//					Dial_Motor.Set_Target_Angle(tmp_Target_Angle);
//					Dial_Target_Angle = Dial_Motor.Get_Target_Angle();
//					Dial_Now_Angle = Dial_Motor.Get_Now_Angle();					
//					Dial_Target_Rpm = Dial_Motor.Get_Target_Rpm();
//					Dial_Now_Rpm = Dial_Motor.Get_Now_Rpm();
				}
			}
			else if (DR16.Get_Right_Switch() == 6)
			{
				// 连发模式
				dial_flag = 2;					
				Dial_Target_Angle = Dial_Motor.Get_Now_Angle();
				Dial_Target_Angle += 40.f;
				Dial_Motor.Set_Target_Angle(Dial_Target_Angle);
				Dial_Target_Angle = Dial_Motor.Get_Now_Angle();
				Dial_Now_Angle = Dial_Motor.Get_Now_Angle();
				Dial_Target_Rpm = Dial_Motor.Get_Target_Rpm();
				Dial_Now_Rpm = Dial_Motor.Get_Now_Rpm();
			}
			else
			{
				dial_flag = 0;
				Dial_Target_Angle = Dial_Motor.Get_Now_Angle();
				Dial_Now_Angle = Dial_Motor.Get_Now_Angle();
				Dial_Motor.Set_Zero();
				Dial_Motor.Set_Target_Rpm(0.f);
				Dial_Target_Rpm = Dial_Motor.Get_Target_Rpm();
				Dial_Now_Rpm = Dial_Motor.Get_Now_Rpm();
			}
		}
		osDelay(50);
	}
}

void PitchYaw_Control(void *arg)
{
	Yaw_Target_Angle = 68.f;
	Pitch_Target_Angle = 28.f;
	for (;;)
	{
		if (dr16_flag)
		{
			/* ------ DR16数据处理 ------ */
			DR16.TIM_1ms_Calculate_PeriodElapsedCallback();
			/* ------ PitchYaw设置 ------ */

			Pitch_Target_Angle -= DR16.Get_Right_Y();
			if (Pitch_Target_Angle > 53)
				Pitch_Target_Angle = 53;
			if (Pitch_Target_Angle < 3)
				Pitch_Target_Angle = 3;
			Pitch_Motor.Set_Target_Angle(Pitch_Target_Angle);
			Pitch_Now_Angle = Pitch_Motor.Get_Now_Angle();
			Pitch_Target_Rpm = Pitch_Motor.Get_Target_Rpm();
			Pitch_Now_Rpm = Pitch_Motor.Get_Now_Rpm();

			Yaw_Target_Angle -= DR16.Get_Right_X();
			Yaw_Motor.Set_Target_Angle(Yaw_Target_Angle);
			Yaw_Now_Angle = Yaw_Motor.Get_Now_Angle();
			Yaw_Target_Rpm = Yaw_Motor.Get_Target_Rpm();
			Yaw_Now_Rpm = Yaw_Motor.Get_Now_Rpm();
		}

		osDelay(5);
	}
}

void Motor_Set(void *arg)
{
	for (;;)
	{
		if (dr16_flag)
		{
			/* ------ 各电机的PID计算 ------ */
			Left_Fric.TIM_PID_PeriodElapsedCallback();
			Right_Fric.TIM_PID_PeriodElapsedCallback();
			Dial_Motor.TIM_PID_PeriodElapsedCallback();
			Pitch_Motor.TIM_PID_PeriodElapsedCallback();
			Yaw_Motor.TIM_PID_PeriodElapsedCallback();
		}

		/* ------ 各电机的CAN发送 ------ */
		TIM_CAN_PeriodElapsedCallback();
		osDelay(1);
	}
}

void Serial_Transmit(void *arg)
{
	for (;;)
	{
		// 打印各参数, 用于调试
		// HAL_UART_Transmit(&huart5, (uint8_t *)&Left_Fric_Now_Rpm, sizeof(float), HAL_MAX_DELAY);
		// HAL_UART_Transmit(&huart5, (uint8_t *)&Left_Fric_Target_Rpm, sizeof(float), HAL_MAX_DELAY);
		// HAL_UART_Transmit(&huart5, (uint8_t *)&Right_Fric_Now_Rpm, sizeof(float), HAL_MAX_DELAY);
		// HAL_UART_Transmit(&huart5, (uint8_t *)&Right_Fric_Target_Rpm, sizeof(float), HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart5, (uint8_t *)&Dial_Now_Rpm, sizeof(float), HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart5, (uint8_t *)&Dial_Target_Rpm, sizeof(float), HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart5, (uint8_t *)&Dial_Now_Angle, sizeof(float), HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart5, (uint8_t *)&Dial_Target_Angle, sizeof(float), HAL_MAX_DELAY);
		// HAL_UART_Transmit(&huart5, (uint8_t *)&Pitch_Now_Rpm, sizeof(float), HAL_MAX_DELAY);
		// HAL_UART_Transmit(&huart5, (uint8_t *)&Pitch_Target_Rpm, sizeof(float), HAL_MAX_DELAY);
		// HAL_UART_Transmit(&huart5, (uint8_t *)&Pitch_Now_Angle, sizeof(float), HAL_MAX_DELAY);
		// HAL_UART_Transmit(&huart5, (uint8_t *)&Pitch_Target_Angle, sizeof(float), HAL_MAX_DELAY);
		// HAL_UART_Transmit(&huart5, (uint8_t *)&Yaw_Now_Rpm, sizeof(float), HAL_MAX_DELAY);
		// HAL_UART_Transmit(&huart5, (uint8_t *)&Yaw_Target_Rpm, sizeof(float), HAL_MAX_DELAY);
		// HAL_UART_Transmit(&huart5, (uint8_t *)&Yaw_Now_Angle, sizeof(float), HAL_MAX_DELAY);
		// HAL_UART_Transmit(&huart5, (uint8_t *)&Yaw_Target_Angle, sizeof(float), HAL_MAX_DELAY);

		// justfloat帧尾标志
		HAL_UART_Transmit(&huart5, tail, 4, HAL_MAX_DELAY);
		osDelay(5);
	}
}
void Led_Set(void *arg)
{
	for (;;)
	{
		led13.LedToggle();
		osDelay(500);
	}
}
/* User Code End Here ---------------------------------*/
