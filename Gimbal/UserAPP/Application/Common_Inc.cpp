#include "Common_Inc.h"

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

/* RTOS Resources ------------------------------------------------------------*/
/* Queues */
QueueHandle_t USART_RxPort;
QueueHandle_t USART_TxPort;
QueueHandle_t CAN1_TxPort;
QueueHandle_t CAN2_TxPort;
QueueHandle_t CAN1_RxPort;
QueueHandle_t CAN2_RxPort;

/* Semaphores */
/* Mutexes */
/* Notifications */

/* Other Resources -----------------------------------------------------------*/
uint8_t Uart1_Rx_Buff[USART1_RX_BUFFER_SIZE]; /*!< Receive buffer for Uart1 */
uint8_t Uart2_Rx_Buff[USART2_RX_BUFFER_SIZE]; /*!< Receive buffer for Uart2 */
uint8_t Uart3_Rx_Buff[USART3_RX_BUFFER_SIZE]; /*!< Receive buffer for Uart3 */
uint8_t Uart4_Rx_Buff[USART4_RX_BUFFER_SIZE]; /*!< Receive buffer for Uart4 */
uint8_t Uart5_Rx_Buff[USART5_RX_BUFFER_SIZE]; /*!< Receive buffer for Uart5 */
uint8_t Uart6_Rx_Buff[USART6_RX_BUFFER_SIZE]; /*!< Receive buffer for Uart6 */

Class_DR16 DR16;
LED_Classdef led13;
Class_Motor_GM6020 Pitch_Motor;
Class_Motor_GM6020 Yaw_Motor;
Class_Motor_C620 Left_Fric;
Class_Motor_C620 Right_Fric;
Class_Motor_C610 Dial_Motor;
