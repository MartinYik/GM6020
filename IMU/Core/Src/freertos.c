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
float roll, pitch, yaw;
/* USER CODE END Variables */
/* Definitions for imuTask */
osThreadId_t imuTaskHandle;
const osThreadAttr_t imuTask_attributes = {
    .name = "imuTask",
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

void ImuTask(void *argument);
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
  /* creation of imuTask */
  imuTaskHandle = osThreadNew(ImuTask, NULL, &imuTask_attributes);

  /* creation of usartTask */
  usartTaskHandle = osThreadNew(UsartTask, NULL, &usartTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */
}

/* USER CODE BEGIN Header_ImuTask */
/**
 * @brief  Function implementing the imuTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_ImuTask */
void ImuTask(void *argument)
{
  /* USER CODE BEGIN ImuTask */
  int ret = 0;
  do
  {
    ret = MPU6050_Init(GPIOB, GPIO_PIN_6, GPIO_PIN_7);
  } while (ret);
  /* Infinite loop */
  for (;;)
  {
    while (mpu_dmp_get_data(&pitch, &roll, &yaw))
      ;
    osDelay(5);
  }
  /* USER CODE END ImuTask */
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
  char str[50];
  /* Infinite loop */
  for (;;)
  {
    sprintf(str, "pitch: %f, roll: %f, yaw: %f\r\n", pitch, roll, yaw);
    HAL_UART_Transmit(&huart1, (uint8_t *)str, strlen(str), HAL_MAX_DELAY);
    osDelay(1000);
  }
  /* USER CODE END UsartTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
