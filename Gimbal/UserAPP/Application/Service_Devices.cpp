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
TaskHandle_t DeviceDR16_Handle;
TaskHandle_t LedSet_Handle;
/* Private variables ---------------------------------------------------------*/
/* Private function declarations ---------------------------------------------*/
void Device_DR16(void *arg);
void Device_Control(void *arg);
void Mode_Set(void *arg);
void Led_Set(void *arg);
/* Exported devices ----------------------------------------------------------*/
/* Motor & ESC & Other actuators*/
//Motor_AK80_9  Test_Motor(1, 0, 0);
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
//	xTaskCreate(Device_DR16,      	"Dev.DR16", 		Small_Stack_Size,  	NULL, 	PriorityHigh,   		&DeviceDR16_Handle);
	xTaskCreate(Led_Set,			"Led_set",			Small_Stack_Size,	NULL,	PriorityHigh,			&LedSet_Handle);
}


void Led_Set(void *arg)
{
	for(;;)
	{
		led13.LedToggle();
		osDelay(500);
	}
	
}






//void Device_DR16(void *arg)
//{
//  /* Cache for Task */

//  /* Pre-Load for task */

//  /* Infinite loop */
//	DR16_DataPack_Typedef* dr16_pack;
//	TickType_t _xTicksToWait = pdMS_TO_TICKS(1);
//	for(;;)
//	{
//		
//		if(xTaskNotifyWait(0x00000000, 0xFFFFFFFF, (uint32_t *) &dr16_pack, 0) == pdTRUE)
//			{
//				/* Remote control data unpacking */
//				DR16.DataCapture(dr16_pack);
//			}
//	}
//}

/* User Code End Here ---------------------------------*/

/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/
