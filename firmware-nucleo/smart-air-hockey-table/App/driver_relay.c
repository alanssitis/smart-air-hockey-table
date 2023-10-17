#include "driver_relay.h"

void Driver_Relay_TurnOn()
{
	GPIOD->BSRR |= RELAY_SET; // turn relay on
}

void Driver_Relay_TurnOff()
{
	GPIOD->BSRR |= RELAY_RESET; // turn relay off
}
