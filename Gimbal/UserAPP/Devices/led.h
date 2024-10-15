#ifndef _LED_H
#define _LED_H

#include "gpio.h"
#ifdef __cplusplus
extern "C" {
	
class LED_Classdef
{
	private:
		GPIO_TypeDef* Led_GPIO_Port = GPIOC;
		uint16_t Led_Pin = GPIO_PIN_13;
	public:
//		LED_Classdef();
		void LedToggle();
};

#endif
#ifdef  __cplusplus
}
#endif 

#endif
