#ifndef INC_DRIVER_LED_H_
#define INC_DRIVER_LED_H_

#include <inttypes.h>

extern const uint32_t dma_buffer_address;
extern const uint32_t dma_buffer_size;

void Driver_LED_Init();
void Driver_LED_SetColor(uint8_t x, uint8_t y, uint32_t color);
void GPDMA1_Channel0_Handler(uint8_t offset);

#endif /* INC_DRIVER_LED_H_ */
