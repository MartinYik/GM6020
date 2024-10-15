#include "led.h"


void LED_Classdef::LedToggle()
{
	HAL_GPIO_TogglePin(Led_GPIO_Port, Led_Pin);
}
