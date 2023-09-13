#ifndef INC_DRIVER_LED_H_
#define INC_DRIVER_LED_H_

#include <inttypes.h>

void Driver_LED_Init();
void Driver_LED_SetColor(uint8_t x, uint8_t y, uint32_t color);
void DMA1_Handler(uint8_t offset);

#endif /* INC_DRIVER_LED_H_ */
