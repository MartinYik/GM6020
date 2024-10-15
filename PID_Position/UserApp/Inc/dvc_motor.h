#ifndef _DVC_MOTOR_H
#define _DVC_MOTOR_H

#ifdef __cplusplus
extern "C"
{
#endif
/* Includes ------------------------------------------------------------------*/
#if defined(USE_HAL_DRIVER)
#if defined(STM32F405xx) || defined(STM32F407xx)
#include <stm32f4xx_hal.h>
#endif
#if defined(STM32F103xx)
#include <stm32f1xx_hal.h>
#endif
#if defined(STM32H750xx)
#include <stm32h7xx_hal.h>
#endif
#endif

	/* Private macros ------------------------------------------------------------*/

	/* Private type --------------------------------------------------------------*/
	/* Exported macros -----------------------------------------------------------*/
	typedef struct PID_Controller
	{
		float Kp, Ki, Kd; // 系数
		float p_out, d_out;
		float i_max;
		float i_out;
		float err, out;
		float last_err; // 上次误差
	} PID_Controller;

	typedef struct GM6020TypeDef
	{
		int16_t Rx_Encoder, Rx_RPM, Rx_Torque, Rx_Temp;
		// 之前的编码器位置
		int16_t Pre_Encoder;
		// 总编码器位置
		int16_t Total_Encoder;
		// 总圈数
		int16_t Total_Round;
	} Motor_GM6020;

#define PI (3.14159265f)
#define I_MAX 20000
#define OUT_MAX 25000
	/* Exported types ------------------------------------------------------------*/
	/* Exported variables ---------------------------------------------------------*/
	extern Motor_GM6020 motor;
	/* Exported function declarations ---------------------------------------------*/
	float PID_Calc(PID_Controller *PID, float Current, float Target);
	float limit(float value, float min, float max);
	void Motor_Call_Back(uint8_t *RxData);
#ifdef __cplusplus
}
#endif

#endif
