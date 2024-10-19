#ifndef _COM_INC_H_
#define _COM_INC_H_

/* Includes ------------------------------------------------------------------*/
/* Middlewares & Drivers Support */
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>
#include <stm32f4xx.h>

#include "main.h"
#include "cmsis_os2.h"


#include "drv_can.h"
#include "drv_i2c.h"
#include "led.h"
#include "dr16.h"
#include "dji_motor.h"
#include "mpu6050.h"
#include "mpu6050_config.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
/* Macro Definitions ---------------------------------------------------------*/
#define Tiny_Stack_Size 64
#define Small_Stack_Size 128
#define Normal_Stack_Size 256
#define Large_Stack_Size 512
#define Huge_Stack_Size 1024
#define PriorityVeryLow 1
#define PriorityLow 2
#define PriorityBelowNormal 3
#define PriorityNormal 4
#define PriorityAboveNormal 5
#define PriorityHigh 6
#define PrioritySuperHigh 7
#define PriorityRealtime 8

#define IMU_IIC_SCL_Pin GPIO_PIN_6
#define IMU_IIC_SCL_GPIO_Port GPIOB
#define IMU_IIC_SDA_Pin GPIO_PIN_7
#define IMU_IIC_SDA_GPIO_Port GPIOB

/* HAL Handlers --------------------------------------------------------------*/
extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim2;

extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart6;

/* RTOS Resources ------------------------------------------------------------*/
/* Queues */
extern QueueHandle_t USART_RxPort;
extern QueueHandle_t USART_TxPort;
extern QueueHandle_t CAN1_TxPort;
extern QueueHandle_t CAN2_TxPort;
extern QueueHandle_t CAN1_RxPort;
extern QueueHandle_t CAN2_RxPort;

/* Semaphores */
/* Mutexes */
/* Notifications */
/* Other Resources -----------------------------------------------------------*/
#define USART1_RX_BUFFER_SIZE 32
#define USART2_RX_BUFFER_SIZE 64
#define USART3_RX_BUFFER_SIZE 128
#define USART4_RX_BUFFER_SIZE 256
#define USART5_RX_BUFFER_SIZE 512
#define USART6_RX_BUFFER_SIZE 256

extern uint8_t Uart1_Rx_Buff[USART1_RX_BUFFER_SIZE];
extern uint8_t Uart2_Rx_Buff[USART2_RX_BUFFER_SIZE];
extern uint8_t Uart3_Rx_Buff[USART3_RX_BUFFER_SIZE];
extern uint8_t Uart4_Rx_Buff[USART4_RX_BUFFER_SIZE];
extern uint8_t Uart5_Rx_Buff[USART5_RX_BUFFER_SIZE];
extern uint8_t Uart6_Rx_Buff[USART6_RX_BUFFER_SIZE];

extern Class_DR16 DR16;
extern LED_Classdef led13;
extern Class_Motor_GM6020 Pitch_Motor;
extern Class_Motor_GM6020 Yaw_Motor;
extern Class_Motor_C620 Left_Fric;
extern Class_Motor_C620 Right_Fric;
extern Class_Motor_C610 Dial_Motor;
#endif
