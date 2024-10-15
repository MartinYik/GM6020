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
#include "System_Config.h"
#include "System_DataPool.h"
/* Service */
#include "Service_Devices.h"
#include "Service_Communication.h"
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

  Service_Devices_Init();
  // Service_Communication_Init();
  /* Applications Init ----------------*/
	
}




/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/

