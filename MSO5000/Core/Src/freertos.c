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
#include "tim.h"
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
/* Definitions for canTask */
osThreadId_t canTaskHandle;
const osThreadAttr_t canTask_attributes = {
    .name = "canTask",
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
/* Definitions for pwmTask */
osThreadId_t pwmTaskHandle;
const osThreadAttr_t pwmTask_attributes = {
    .name = "pwmTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void MotorTask(void *argument);
void UsartTask(void *argument);
void PwmTask(void *argument);

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
  /* creation of canTask */
  canTaskHandle = osThreadNew(MotorTask, NULL, &canTask_attributes);

  /* creation of usartTask */
  usartTaskHandle = osThreadNew(UsartTask, NULL, &usartTask_attributes);

  /* creation of pwmTask */
  pwmTaskHandle = osThreadNew(PwmTask, NULL, &pwmTask_attributes);

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
  int16_t Torque = 0;
  Torque = 5000;
  CAN1_0x1ff_Tx_Data[0] = Torque >> 8;
  CAN1_0x1ff_Tx_Data[1] = Torque;
  /* Infinite loop */
  for (;;)
  {

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
    char str[5] = "YKJ\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t *)str, strlen(str), HAL_MAX_DELAY);
    osDelay(1000);
  }
  /* USER CODE END UsartTask */
}

/* USER CODE BEGIN Header_PwmTask */
/**
 * @brief Function implementing the pwmTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_PwmTask */
void PwmTask(void *argument)
{
  /* USER CODE BEGIN PwmTask */
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  /* Infinite loop */
  for (;;)
  {
    for (int i = 0; i < 100; i++)
    {
      TIM2->CCR2 = i;
      osDelay(5);
    }
    for (int i = 99; i >= 0; i--)
    {
      TIM2->CCR2 = i;
      osDelay(5);
    }
  }
  /* USER CODE END PwmTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
