/**
  ******************************************************************************
  * Copyright (c) 2019 - ~, SCUT-RobotLab Development Team
  * @file    drv_can.c
  * @author  Lv Junyu 13668997406@163.com
  * @brief   Code for CAN driver in STM32 series MCU, supported packaged:
  *          - STM32Cube_FW_F4_V1.24.0.
  *          - STM32Cube_FW_F1_V1.8.0.
  *          - STM32Cube_FW_H7_V1.5.0.
  * @date    2020-07-15
  * @version 1.2
  * @par Change Log：
  * <table>
  * <tr><th>Date        <th>Version  <th>Author    		    <th>Description
  * <tr><td>2019-06-12  <td> 1.0     <td>Mentos Seetoo    <td>Creator
  * <tr><td>2019-11-03  <td> 1.1     <td>Lv Junyu         <td>Remove the precompiled macro \n
  *                                                           Add new function interfaces.
  * <tr><td>2020-07-15  <td> 1.2     <td>Mentos Seetoo    <td>Merge callback_register into init.
  * </table>
  *
  ==============================================================================
							How to use this driver
  ==============================================================================
	@note
	  -# 调用`CAN_Init()`，初始化CAN,设置CAN接收处理函数的指针。
	  -# 如需接收can中的消息，需调用CAN_Filter_Mask_Config配置滤波器（需注意配置格式）
		  示例如下：`CAN_Filter_Mask_Config(&hcan1,CanFilter_1|CanFifo_0|Can_STDID|Can_DataType,0x3ff,0x3f0)`;
	  -# 在需要用到发送的部分直接调用CANx_SendData()函数

	@warning
	  -# 本模块只能保存一条来自于同一个FIFO的消息(详细见下方HAL库FIFOx中断的
		  实现),请注意及时读走消息。
	  -# 添加预编译宏`USE_FULL_ASSERT`可以启用断言检查。

  ******************************************************************************
  * @attention
  *
  * if you had modified this file, please make sure your code does not have any
  * bugs, update the version Number, write dowm your name and the date. The most
  * important thing is make sure the users will have clear and definite under-
  * standing through your new brief.
  *
  * <h2><center>&copy; Copyright (c) 2019 - ~, SCUT-RobotLab Development Team.
  * All rights reserved.</center></h2>
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "drv_can.h"

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

static void (*pCAN1_RxCpltCallback)(CAN_RxBuffer *);
static void (*pCAN2_RxCpltCallback)(CAN_RxBuffer *);

// CAN通信发送缓冲区
uint8_t CAN1_0x1ff_Tx_Data[8];
uint8_t CAN1_0x200_Tx_Data[8];
uint8_t CAN1_0x2ff_Tx_Data[8];

uint8_t CAN2_0x1ff_Tx_Data[8];
uint8_t CAN2_0x200_Tx_Data[8];
uint8_t CAN2_0x2ff_Tx_Data[8];

uint8_t CAN1_0x220_Tx_Data[8];
/* Private type --------------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/

/* function prototypes -------------------------------------------------------*/

/**
 * @brief  Initialize CAN Bus
 * @param  hcan: CANx created by CubeMX.
 * @return None.
 */
uint8_t CAN_Init(CAN_HandleTypeDef *hcan, void (*pFunc)(CAN_RxBuffer *))
{
	/* Check the parameters */
	assert_param(hcan != NULL);

	HAL_CAN_Start(hcan);
	__HAL_CAN_ENABLE_IT(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
	__HAL_CAN_ENABLE_IT(hcan, CAN_IT_RX_FIFO1_MSG_PENDING);

	if (hcan->Instance == CAN1)
	{
		pCAN1_RxCpltCallback = pFunc;
		return SUCCESS;
	}
	else if (hcan->Instance == CAN2)
	{
		pCAN2_RxCpltCallback = pFunc;
		return SUCCESS;
	}
	else
		return ERROR;
}

/**
 * @brief 配置CAN的过滤器
 *
 * @param hcan CAN编号
 * @param Object_Para 编号 | FIFOx | ID类型 | 帧类型
 * @param ID ID
 * @param Mask_ID 屏蔽位(0x3ff, 0x1fffffff)
 */
void CAN_Filter_Mask_Config(CAN_HandleTypeDef *hcan, uint8_t Object_Para, uint32_t ID, uint32_t Mask_ID)
{
	CAN_FilterTypeDef can_filter_init_structure;

	// 检测传参是否正确
	assert_param(hcan != NULL);

	if ((Object_Para & 0x02))
	{
		// 数据帧
		// 掩码后ID的高16bit
		can_filter_init_structure.FilterIdHigh = ID << 3 << 16;
		// 掩码后ID的低16bit
		can_filter_init_structure.FilterIdLow = ID << 3 | ((Object_Para & 0x03) << 1);
		// ID掩码值高16bit
		can_filter_init_structure.FilterMaskIdHigh = Mask_ID << 3 << 16;
		// ID掩码值低16bit
		can_filter_init_structure.FilterMaskIdLow = Mask_ID << 3 | ((Object_Para & 0x03) << 1);
	}
	else
	{
		// 其他帧
		// 掩码后ID的高16bit
		can_filter_init_structure.FilterIdHigh = ID << 5;
		// 掩码后ID的低16bit
		can_filter_init_structure.FilterIdLow = ((Object_Para & 0x03) << 1);
		// ID掩码值高16bit
		can_filter_init_structure.FilterMaskIdHigh = Mask_ID << 5;
		// ID掩码值低16bit
		can_filter_init_structure.FilterMaskIdLow = ((Object_Para & 0x03) << 1);
	}
	// 滤波器序号, 0-27, 共28个滤波器, 前14个在CAN1, 后14个在CAN2
	can_filter_init_structure.FilterBank = Object_Para >> 3;
	// 滤波器绑定FIFO0
	can_filter_init_structure.FilterFIFOAssignment = (Object_Para >> 2) & 0x01;
	// 使能滤波器
	can_filter_init_structure.FilterActivation = ENABLE;
	// 滤波器模式，设置ID掩码模式
	can_filter_init_structure.FilterMode = CAN_FILTERMODE_IDMASK;
	// 32位滤波
	can_filter_init_structure.FilterScale = CAN_FILTERSCALE_32BIT;
	// 从机模式选择开始单元
	can_filter_init_structure.SlaveStartFilterBank = 14;

	HAL_CAN_ConfigFilter(hcan, &can_filter_init_structure);
}

/**
 * @brief  Send an communication frame by CAN.
 * @param  hcan  :CAN bus used to send.
 * @param  ID    :ID of frame.
 * @param  *pData:Data to send.
 * @param  Len   :Length of data.
 * @return CAN_SUCCESS:  Operation success.
 * @return CAN_LINE_BUSY:CAN line busy.
 */
uint8_t CANx_SendData(CAN_HandleTypeDef *hcan, uint16_t ID, uint8_t *pData, uint16_t Len)
{
	static CAN_TxHeaderTypeDef Tx_Header;
	uint32_t used_mailbox;
	/* Check the parameters */
	assert_param(hcan != NULL);

	Tx_Header.StdId = ID;
	Tx_Header.ExtId = 0;
	Tx_Header.IDE = 0;
	Tx_Header.RTR = 0;
	Tx_Header.DLC = Len;

	if (HAL_CAN_AddTxMessage(hcan, &Tx_Header, pData, &used_mailbox) != HAL_OK)
	{
		return CAN_LINE_BUSY;
	}
	else
	{
	}

	return CAN_SUCCESS;
}

/**
 * @brief  Send an extended communication frame by CAN.
 * @param  hcan  :CAN bus used to send.
 * @param  ID    :ID of frame.
 * @param  *pData:Data to send.
 * @param  Len   :Length of data.
 * @return CAN_SUCCESS:  Operation success.
 * @return CAN_LINE_BUSY:CAN line busy.
 */
uint8_t CANx_SendExtData(CAN_HandleTypeDef *hcan, uint16_t ID, uint8_t *pData, uint16_t Len)
{
	CAN_TxHeaderTypeDef tx_header;
	uint32_t used_mailbox;

	// 检测传参是否正确
	assert_param(hcan != NULL);

	tx_header.StdId = ID;
	tx_header.ExtId = 0;
	tx_header.IDE = 0;
	tx_header.RTR = 0;
	tx_header.DLC = Len;

	return (HAL_CAN_AddTxMessage(hcan, &tx_header, pData, &used_mailbox));
}

/*HAL库FIFO0中断*/
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	/*!< CAN receive buffer */
	static CAN_RxBuffer CAN_RxBuffer;

	/* Switch to user call back function. */
	if (hcan->Instance == CAN1)
	{
		if (HAL_CAN_GetRxMessage(hcan, CAN_FILTER_FIFO0, &CAN_RxBuffer.header, CAN_RxBuffer.data) == HAL_ERROR)
		{
		};
		pCAN1_RxCpltCallback(&CAN_RxBuffer);
	}
	else if (hcan->Instance == CAN2)
	{
		if (HAL_CAN_GetRxMessage(hcan, CAN_FILTER_FIFO0, &CAN_RxBuffer.header, CAN_RxBuffer.data) == HAL_ERROR)
		{
		};
		pCAN2_RxCpltCallback(&CAN_RxBuffer);
	}
}
/*HAL库FIFO1中断*/
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	/*!< CAN receive buffer */
	static CAN_RxBuffer CAN_RxBuffer;

	/* Switch to user call back function. */
	if (hcan->Instance == CAN1)
	{
		if (HAL_CAN_GetRxMessage(hcan, CAN_FILTER_FIFO1, &CAN_RxBuffer.header, CAN_RxBuffer.data) == HAL_ERROR)
		{
		};
		pCAN1_RxCpltCallback(&CAN_RxBuffer);
	}
	else if (hcan->Instance == CAN2)
	{
		if (HAL_CAN_GetRxMessage(hcan, CAN_FILTER_FIFO1, &CAN_RxBuffer.header, CAN_RxBuffer.data) == HAL_ERROR)
		{
		};
		pCAN2_RxCpltCallback(&CAN_RxBuffer);
	}
}

/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/
