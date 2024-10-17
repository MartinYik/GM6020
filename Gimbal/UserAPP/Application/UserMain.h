#ifndef USER_MAIN_H
#define USER_MAIN_H

#ifdef __cplusplus
/* Includes ------------------------------------------------------------------*/
#include "drv_can.h"
/* Exported function declarations --------------------------------------------*/
extern "C"
{
#endif
  void System_Tasks_Init(void);
  void System_Source_Init();
#ifdef __cplusplus
}
void CAN1_Motor_Call_Back(Struct_CAN_Rx_Buffer *Rx_Buffer);
void CAN2_Motor_Call_Back(Struct_CAN_Rx_Buffer *Rx_Buffer);
void DR16_UART2_Callback(uint8_t *Buffer, uint16_t Length);
#endif

#endif
