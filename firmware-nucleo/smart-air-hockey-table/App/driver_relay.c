#include "driver_relay.h"

#include "stm32u5xx_ll_gpio.h"

void Driver_Relay_SetPowered(bool is_powered)
{
	if (is_powered) LL_GPIO_SetOutputPin(GPIOD, LL_GPIO_PIN_14);
	else LL_GPIO_ResetOutputPin(GPIOD, LL_GPIO_PIN_14);
}
