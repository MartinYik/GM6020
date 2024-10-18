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
int16_t Torque = 0;
uint8_t motor_flag;
float Target;
float Current;
extern int rc_flag;
extern int last_rc_flag;
extern const RC_ctrl_t *local_rc_ctrl;
/* USER CODE END Variables */
/* Definitions for motorTask */
osThreadId_t motorTaskHandle;
const osThreadAttr_t motorTask_attributes = {
    .name = "motorTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};
/* Definitions for rcTask */
osThreadId_t rcTaskHandle;
const osThreadAttr_t rcTask_attributes = {
    .name = "rcTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void MotorTask(void *argument);
void RCTask(void *argument);

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

    /* creation of rcTask */
    rcTaskHandle = osThreadNew(RCTask, NULL, &rcTask_attributes);

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
    PID_Controller PID_Speed = {10, 0, 0};
    PID_Speed.i_out = 0;
    PID_Speed.i_max = 15000;
    /* Infinite loop */
    for (;;)
    {
        if (motor_flag == 1)
        {
            Target = -(local_rc_ctrl->rc.ch[0] / 2);
            Current = Tx_Data[1];
            Torque = PID_Calc(&PID_Speed, Current, Target);
        }
        else
        {
            Torque = 0;
        }

        CAN2_0x2ff_Tx_Data[0] = Torque >> 8;
        CAN2_0x2ff_Tx_Data[1] = Torque;
        CANx_SendData(&hcan2, 0x2ff, CAN2_0x2ff_Tx_Data, 8);
        HAL_UART_Transmit(&huart4, (uint8_t *)&Current, sizeof(float), HAL_MAX_DELAY);
        HAL_UART_Transmit(&huart4, (uint8_t *)&Target, sizeof(float), HAL_MAX_DELAY);
        HAL_UART_Transmit(&huart4, tail, 4, 100);
        osDelay(5);
    }
    /* USER CODE END MotorTask */
}

/* USER CODE BEGIN Header_RCTask */
/**
 * @brief Function implementing the rcTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_RCTask */
void RCTask(void *argument)
{
    /* USER CODE BEGIN RCTask */
    /* Infinite loop */
    for (;;)
    {
        if (last_rc_flag == rc_flag)
        {
            motor_flag = 0;
        }
        else
        {
            motor_flag = 1;
        }
        last_rc_flag = rc_flag;
        osDelay(100);
    }
    /* USER CODE END RCTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
