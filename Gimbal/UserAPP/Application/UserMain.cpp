/* Includes ------------------------------------------------------------------*/
#include "UserMain.h"
#include "Common_Inc.h"
/* Service */
#include "Service_Devices.h"
/* User support package & SRML */
#include "main.h"

/* Private variables ---------------------------------------------------------*/

/*Founctions------------------------------------------------------------------*/
/**
 * @brief Load drivers ,modules, and data resources for tasks.
 * @note  Edit this function to add Init-functions and configurations.
 */

/**
 * @brief Load and start User Tasks.
 * @note  Edit this function to add tasks into the activated tasks list.
 */
void System_Tasks_Init(void)
{
	/* Syetem Service init --------------*/
	System_Source_Init();
	Service_Devices_Init();
	// Service_Communication_Init();
	/* Applications Init ----------------*/
}

void System_Source_Init()
{
	CAN_Init(&hcan1, CAN1_Motor_Call_Back);
	CAN_Init(&hcan2, CAN2_Motor_Call_Back);
	UART_Init(&huart2, DR16_UART2_Callback, 36);
	UART_Init(&huart5, NULL, 0);
	// char str[] = "Hello Gimbal\r\n";
	// HAL_UART_Transmit(&huart5, (uint8_t *)str, strlen(str), HAL_MAX_DELAY);
	init_finished = true;

	int ret = 0;
	do
	{
		ret = MPU6050_Init(GPIOB, GPIO_PIN_6, GPIO_PIN_7);
	} while (ret);

	led13.Init(GPIOC, GPIO_PIN_13);

	DR16.Init(&huart2);

	Left_Fric.PID_Rpm.Init(25.0f, 0.0f, 0.0f, 0.0f, 16384.0f, 16384.0f);
	Right_Fric.PID_Rpm.Init(25.0f, 0.0f, 0.0f, 0.0f, 16384.0f, 16384.0f);
	Left_Fric.Init(&hcan1, CAN_Motor_ID_0x203, Control_Method_RPM);
	Right_Fric.Init(&hcan1, CAN_Motor_ID_0x202, Control_Method_RPM);

	// Dial_Motor.PID_Rpm.Init(50.0f, 0.0f, 0.4f, 0.0f, 10000.0f, 10000.0f);
	// Dial_Motor.PID_Angle.Init(20.0f, 0.0f, 0.0f, 0.0f, 180.f, 180.f);
	Dial_Motor.PID_Rpm.Init(350.0f, 180.0f, 0.3f, 0.0f, 10000.0f, 10000.0f);
	Dial_Motor.PID_Angle.Init(8.5f, 0.0f, 0.0f, 1.0f, 720.f, 720.f);
	Dial_Motor.Init(&hcan1, CAN_Motor_ID_0x201, Control_Method_ANGLE);

	Pitch_Motor.PID_MPU_Rpm.Init(3000.0f, 20.0f, 0.0f, 100.0f, 25000.0f, 25000.0f);
	// Pitch_Motor.PID_Rpm.Init(30.0f, 50.0f, 0.0f, 0.0f, 25000.0f, 25000.0f);
	Pitch_Motor.PID_Angle.Init(2.2f, 0.0f, 0.0f, 0.7f, 720.f, 720.f);
	Pitch_Motor.Init(&hcan1, CAN_Motor_ID_0x206, Control_Method_ANGLE);
	Yaw_Motor.PID_MPU_Rpm.Init(1600.0f, 0.0f, 0.0f, 0.0f, 25000.0f, 25000.0f);
	// Yaw_Motor.PID_MPU_Rpm.Init(600.0f, 0.0f, 0.0f, 0.0f, 25000.0f, 25000.0f);
	// Yaw_Motor.PID_Rpm.Init(570.0f, 5.0f, 0.0f, 0.0f, 25000.0f, 25000.0f);
	Yaw_Motor.PID_Angle.Init(0.83f, 0.0f, 0.0f, 0.0f, 720.f, 720.f);
	Yaw_Motor.PID_Angle.Set_D_First(PID_D_First_ENABLE);
	Yaw_Motor.Init(&hcan2, CAN_Motor_ID_0x209, Control_Method_ANGLE);
}

/* ---------- Callback Funtions ---------- */
void CAN1_Motor_Call_Back(Struct_CAN_Rx_Buffer *Rx_Buffer)
{
	switch (Rx_Buffer->Header.StdId)
	{
	case (0x203):
	{
		Left_Fric.CAN_RxCpltCallback(Rx_Buffer->Data);
	}
	break;
	case (0x202):
	{
		Right_Fric.CAN_RxCpltCallback(Rx_Buffer->Data);
	}
	break;
	case (0x201):
	{
		Dial_Motor.CAN_RxCpltCallback(Rx_Buffer->Data);
	}
	break;
	case (0x206):
	{
		Pitch_Motor.CAN_RxCpltCallback(Rx_Buffer->Data);
	}
	break;
	}
}
void CAN2_Motor_Call_Back(Struct_CAN_Rx_Buffer *Rx_Buffer)
{
	switch (Rx_Buffer->Header.StdId)
	{
	case (0x209):
	{
		Yaw_Motor.CAN_RxCpltCallback(Rx_Buffer->Data);
	}
	break;
	}
}
void DR16_UART2_Callback(uint8_t *Buffer, uint16_t Length)
{
	DR16.UART_RxCpltCallback(Buffer, Length);
}
