#include "led.h"

void LED_Classdef::Init(GPIO_TypeDef *_GPIOx, uint16_t _GPIO_Pin)
{
	Led_GPIO_Port = _GPIOx;
	Led_Pin = _GPIO_Pin;
}
void LED_Classdef::LedToggle()
{
	HAL_GPIO_TogglePin(Led_GPIO_Port, Led_Pin);
}
