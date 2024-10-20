#ifndef _LED_H
#define _LED_H

#include "gpio.h"
#ifdef __cplusplus
extern "C"
{

	class LED_Classdef
	{
	private:
		GPIO_TypeDef *Led_GPIO_Port;
		uint16_t Led_Pin;

	public:
		void Init(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
		void LedToggle();
	};

#endif
#ifdef __cplusplus
}
#endif

#endif
