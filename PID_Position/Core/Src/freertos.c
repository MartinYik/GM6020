/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

float Target_RPM = 200;
float Now_RPM;
float Target_Angle = 180;
float Now_Angle;
/* USER CODE END Variables */
/* Definitions for motorTask */
osThreadId_t motorTaskHandle;
const osThreadAttr_t motorTask_attributes = {
    .name = "motorTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};
/* Definitions for usartTask */
osThreadId_t usartTaskHandle;
const osThreadAttr_t usartTask_attributes = {
    .name = "usartTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void MotorTask(void *argument);
void UsartTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void)
{
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of motorTask */
  motorTaskHandle = osThreadNew(MotorTask, NULL, &motorTask_attributes);

  /* creation of usartTask */
  usartTaskHandle = osThreadNew(UsartTask, NULL, &usartTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */
}

/* USER CODE BEGIN Header_MotorTask */
/**
 * @brief  Function implementing the motorTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_MotorTask */
void MotorTask(void *argument)
{
  /* USER CODE BEGIN MotorTask */
  int16_t Torque;
  float Angle_PID_Out;
  PID_Controller PID_Speed = {45, 15, 0};
  PID_Controller PID_Position = {4,0.15,1};
  PID_Speed.i_out = 0;
  PID_Speed.i_max = 15000;
  PID_Position.i_out = 0;
  PID_Position.i_max = 5;
  /* Infinite loop */
  for (;;)
  {
    Now_Angle = Tx_Data[0] * 360.0f / 8192.0f;
    Now_RPM = Tx_Data[1];
    Angle_PID_Out = PID_Calc(&PID_Position, Now_Angle, Target_Angle);
    Torque = PID_Calc(&PID_Speed, Now_RPM, Angle_PID_Out);
    // Torque = PID_Calc(&PID_Speed, Now_RPM, Target_RPM);
    CAN1_0x1ff_Tx_Data[4] = Torque >> 8;
    CAN1_0x1ff_Tx_Data[5] = Torque;
    CANx_SendData(&hcan1, 0x1ff, CAN1_0x1ff_Tx_Data, 8);
    osDelay(5);
  }
  /* USER CODE END MotorTask */
}

/* USER CODE BEGIN Header_UsartTask */
/**
 * @brief Function implementing the usartTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_UsartTask */
void UsartTask(void *argument)
{
  /* USER CODE BEGIN UsartTask */
  /* Infinite loop */
  for (;;)
  {
    HAL_UART_Transmit(&huart1, (uint8_t *)&Target_Angle, sizeof(float), HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1, (uint8_t *)&Now_Angle, sizeof(float), HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1, tail, 4, 100);
    osDelay(5);
  }
  /* USER CODE END UsartTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
