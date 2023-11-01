#ifndef INC_DRIVER_LED_H_
#define INC_DRIVER_LED_H_

#include <inttypes.h>

#define LED_MATRIX_ROW_SIZE 2
#define LED_MATRIX_COL_SIZE 8
#define LED_MATRIX_PIXEL_COUNT (LED_MATRIX_ROW_SIZE * LED_MATRIX_COL_SIZE)
#define LED_COLOR_DATA_SIZE 24

void Driver_LED_Init();
void Driver_LED_SetColor(uint_fast8_t x, uint_fast8_t y, uint32_t color);
void Driver_LED_Clear();
void Driver_LED_Tick();
void GPDMA1_Channel0_Handler();

#endif /* INC_DRIVER_LED_H_ */
