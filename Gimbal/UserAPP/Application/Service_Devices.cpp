/* Includes ------------------------------------------------------------------*/
#include "Service_Devices.h"
/* Private define ------------------------------------------------------------*/
TaskHandle_t Gimbal_Control_Handle;
TaskHandle_t DR16_Handle;
TaskHandle_t MotorSet_Handle;
TaskHandle_t LedSet_Handle;
TaskHandle_t SerialTransmit_Handle;
/* Private variables ---------------------------------------------------------*/
float Yaw_Target_Rpm = 0;
float Yaw_Now_Rpm = 0;
float Yaw_Target_Angle = 0;
float Yaw_Now_Angle = 0;
float Left_Fric_Target_Rpm = 0;
float Left_Fric_Now_Rpm = 0;
float Right_Fric_Target_Rpm = 0;
float Right_Fric_Now_Rpm = 0;
bool dr16_flag = true;
/* Private function declarations ---------------------------------------------*/
void Gimbal_Control(void *arg);
void DR16_Test(void *arg);
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
	xTaskCreate(Gimbal_Control, "Gimbal_Control", Normal_Stack_Size, NULL, PrioritySuperHigh, &Gimbal_Control_Handle);
	xTaskCreate(DR16_Test, "DR16_Test", Normal_Stack_Size, NULL, PriorityHigh, &DR16_Handle);
	xTaskCreate(Motor_Set, "Motor_set", Normal_Stack_Size, NULL, PriorityNormal, &MotorSet_Handle);
	xTaskCreate(Led_Set, "Led_set", Small_Stack_Size, NULL, PriorityLow, &LedSet_Handle);
	xTaskCreate(Serial_Transmit, "Serial_send", Small_Stack_Size, NULL, PriorityNormal, &SerialTransmit_Handle);
}

void Gimbal_Control(void *arg)
{
	for (;;)
	{
		osDelay(1);
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

void Motor_Set(void *arg)
{
	/* Cache for Task */

	/* Pre-Load for task */

	/* Infinite loop */

	for (;;)
	{

		DR16.TIM_1ms_Calculate_PeriodElapsedCallback();

		Left_Fric.Set_Target_Rpm(500.f);
		Left_Fric_Target_Rpm = Left_Fric.Get_Target_Rpm();
		Left_Fric_Now_Rpm = Left_Fric.Get_Now_Rpm();

		Right_Fric.Set_Target_Rpm(500.f);
		Right_Fric_Target_Rpm = Right_Fric.Get_Target_Rpm();
		Right_Fric_Now_Rpm = Right_Fric.Get_Now_Rpm();

		Left_Fric.TIM_PID_PeriodElapsedCallback();
		Right_Fric.TIM_PID_PeriodElapsedCallback();

		// Yaw_Motor.Set_Target_Rpm(DR16.Get_Right_X() * 320.f);
		// Yaw_Target_Rpm = Yaw_Motor.Get_Target_Rpm();
		// Yaw_Now_Rpm = Yaw_Motor.Get_Now_Rpm();
		// Yaw_Motor.TIM_PID_PeriodElapsedCallback();

		TIM_CAN_PeriodElapsedCallback();

		osDelay(5);
	}
}

void DR16_Test(void *arg)
{
	for (;;)
	{
		DR16.TIM_100ms_Alive_PeriodElapsedCallback();
		osDelay(100);
	}
}

void Serial_Transmit(void *arg)
{
	for (;;)
	{
		HAL_UART_Transmit(&huart5, (uint8_t *)&Left_Fric_Now_Rpm, sizeof(float), HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart5, (uint8_t *)&Left_Fric_Target_Rpm, sizeof(float), HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart5, (uint8_t *)&Right_Fric_Now_Rpm, sizeof(float), HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart5, (uint8_t *)&Right_Fric_Target_Rpm, sizeof(float), HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart5, tail, 4, HAL_MAX_DELAY);
		osDelay(5);
	}
}
/* User Code End Here ---------------------------------*/
