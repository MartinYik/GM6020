/**
  ******************************************************************************
  * @file   System_config.cpp
  * @brief  Deploy resources,tasks and services in this file.
  ******************************************************************************
  * @note
  *  - Before running your Task you should first include your headers and init-
  *    ialize used resources in "System_Resource_Init()". This function will be
  *    called before tasks Start.
  *
  *  - All tasks should be created in "System_Tasks_Init()", this function will
  *    be called in 'defaultTask()'.
  *
  ===============================================================================
                                    Task List
  ===============================================================================
  * <table>
  * <tr><th>Task Name     <th>Priority          <th>Frequency/Hz    <th>Stack/Byte
  * <tr><td>              <td>                  <td>                <td>
  * </table>
  *
 */
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
	init_finished = true;
	
  DR16.Init(&huart2);
  Left_Fric.PID_Rpm.Init(500.0f, 2000.0f, 0.0f, 0.0f, 25000.0f, 25000.0f);
  Right_Fric.PID_Rpm.Init(500.0f, 2000.0f, 0.0f, 0.0f, 25000.0f, 25000.0f);
  Left_Fric.Init(&hcan1, CAN_Motor_ID_0x203, Control_Method_RPM);
  Right_Fric.Init(&hcan1, CAN_Motor_ID_0x202, Control_Method_RPM);
  Yaw_Motor.PID_Torque.Init(0.8f, 100.0f, 0.0f, 0.0f, 30000.0f, 30000.0f);
  Yaw_Motor.PID_Rpm.Init(500.0f, 2000.0f, 0.0f, 0.0f, 25000.0f, 25000.0f);
  Yaw_Motor.PID_Angle.Init(12.0f, 0.0f, 0.0f, 0.0f, 720.f, 720.f);
  Yaw_Motor.Init(&hcan2, CAN_Motor_ID_0x209, Control_Method_RPM);
}

/* Callback Funtions ---------- */
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
