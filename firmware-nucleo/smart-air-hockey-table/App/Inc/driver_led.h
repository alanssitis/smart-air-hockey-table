#ifndef INC_DRIVER_LED_H_
#define INC_DRIVER_LED_H_

#include <inttypes.h>

void Driver_LED_Init();
void Driver_LED_SetColor(uint8_t x, uint8_t y, uint32_t color);
void Driver_LED_Clear();
void Driver_LED_Tick();
void GPDMA1_Channel0_Handler();

#endif /* INC_DRIVER_LED_H_ */
