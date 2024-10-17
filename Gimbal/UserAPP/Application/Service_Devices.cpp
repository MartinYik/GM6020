/**
  ******************************************************************************
  * @file   Service_Devices.cpp
  * @brief  Devices service running file.
  ******************************************************************************
  * @note
  *  - Before running your devices, just do what you want ~ !
  *  - More devices or using other classification is decided by yourself ~ !
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
#include "Service_Devices.h"
/* Private define ------------------------------------------------------------*/
TaskHandle_t DR16_Handle;
TaskHandle_t MotorSet_Handle;
TaskHandle_t LedSet_Handle;
/* Private variables ---------------------------------------------------------*/
float Yaw_Target_Rpm = 0;
float Yaw_Now_Rpm = 0;
float Yaw_Target_Angle = 0;
float Yaw_Now_Angle = 0;
/* Private function declarations ---------------------------------------------*/
void DR16_Test(void *arg);
void Motor_Set(void *arg);
void Led_Set(void *arg);
/* Exported devices ----------------------------------------------------------*/
/* Motor & ESC & Other actuators*/
// Motor_AK80_9  Test_Motor(1, 0, 0);
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
  xTaskCreate(DR16_Test, "DR16_Test", Normal_Stack_Size, NULL, PriorityNormal, &DR16_Handle);
  xTaskCreate(Motor_Set, "Motor_set", Normal_Stack_Size, NULL, PriorityNormal, &MotorSet_Handle);
  xTaskCreate(Led_Set, "Led_set", Small_Stack_Size, NULL, PriorityLow, &LedSet_Handle);
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
    // Yaw_Motor.Set_Target_Angle(0);
    // Yaw_Target_Angle = Yaw_Motor.Get_Target_Angle();
    // Yaw_Now_Angle = Yaw_Motor.Get_Now_Angle();
    DR16.TIM_1ms_Calculate_PeriodElapsedCallback();

    TIM_CAN_PeriodElapsedCallback();

    osDelay(1);
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
/* User Code End Here ---------------------------------*/

/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/
