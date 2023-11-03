#ifndef INC_DRIVER_LED_H_
#define INC_DRIVER_LED_H_

#include <inttypes.h>

#define LED_MATRIX_COL_NUM 16
#define LED_MATRIX_ROW_NUM 8
#define LED_MATRIX_PIXEL_COUNT (LED_MATRIX_ROW_NUM * LED_MATRIX_COL_NUM)
#define LED_COLOR_DATA_SIZE 24

typedef struct {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} Color;

void Driver_LED_Init();
void Driver_LED_SetColor(uint_fast8_t col, uint_fast8_t row, Color color);
void Driver_LED_Clear();
void Driver_LED_Tick();
void GPDMA1_Channel0123_Handler();

#endif /* INC_DRIVER_LED_H_ */
