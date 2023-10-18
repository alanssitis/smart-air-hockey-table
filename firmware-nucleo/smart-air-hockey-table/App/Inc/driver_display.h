#ifndef INC_DRIVER_DISPLAY_H_
#define INC_DRIVER_DISPLAY_H_

#include <inttypes.h>

typedef enum
{
	DISPLAY_0 = 0x1,
	DISPLAY_1 = 0x2,
	DISPLAY_ALL = 0xFF
} Display;

void Driver_Display_Init();
void Driver_Display_Clear(Display display);
void Driver_Display_SetPowered(Display display, bool is_powered);
void Driver_Display_Print(Display display, uint8_t line, uint8_t offset, const char* format, ...);
void Driver_Display_ShowScore(Display display, uint8_t score_a, uint8_t score_b);
void Driver_Display_FontTest(Display display);

#endif /* INC_DRIVER_DISPLAY_H_ */
