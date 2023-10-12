#include "app_core.h"

#include <inttypes.h>
#include "stm32u5xx_ll_tim.h"
#include "stm32u5xx_ll_utils.h"

#include "driver_led.h"
//#include "driver_display.h"

void App_Init()
{
	// Configure SysTick to have 1ms time based on 160 MHz, for LL_mDelay
	LL_Init1msTick(160000000);

	Driver_LED_Init();
	//Driver_Display_Init();

	Driver_LED_SetColor(0, 0, 0xFF0000);
	Driver_LED_SetColor(1, 0, 0x00FF00);
	Driver_LED_SetColor(2, 0, 0x0000FF);
	Driver_LED_SetColor(3, 0, 0xFFFF00);

	// TODO: State machine loop
}
