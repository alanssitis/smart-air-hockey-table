#ifndef INC_DRIVER_LED_H_
#define INC_DRIVER_LED_H_

#include <inttypes.h>

#define LED_MATRIX_WIDTH 2
#define LED_MATRIX_HEIGHT 8

typedef struct
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
} Color;

void Driver_LED_Init();
void Driver_LED_SetColor(uint_fast8_t x, uint_fast8_t y, Color color);
Color Driver_LED_GetColor(uint_fast8_t x, uint_fast8_t y);
void Driver_LED_Clear();
void Driver_LED_Tick();
void GPDMA1_Channel0_Handler();

#endif /* INC_DRIVER_LED_H_ */
