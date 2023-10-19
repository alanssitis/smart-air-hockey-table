#include "driver_relay.h"

#include "stm32u575xx.h"

#define RELAY_SET	GPIO_BSRR_BS14
#define RELAY_RESET	GPIO_BSRR_BR14

void Driver_Relay_TurnOn()
{
	GPIOD->BSRR |= RELAY_SET; // turn relay on
}

void Driver_Relay_TurnOff()
{
	GPIOD->BSRR |= RELAY_RESET; // turn relay off
}
