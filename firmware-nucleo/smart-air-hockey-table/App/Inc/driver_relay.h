#ifndef INC_DRIVER_RELAY_H_
#define INC_DRIVER_RELAY_H_

#include "stm32u575xx.h"

#define RELAY_SET	GPIO_BSRR_BS14
#define RELAY_RESET	GPIO_BSRR_BR14

void Driver_Relay_TurnOn();
void Driver_Relay_TurnOff();

#endif /* INC_DRIVER_RELAY_H_ */
