#include "app_core.h"

#include <inttypes.h>
#include "stm32u5xx_ll_utils.h"

#include "driver_led.h"
#include "driver_display.h"

void App_Init()
{
	// Configure SysTick to have 1ms time based on 160 MHz, for LL_mDelay
	LL_Init1msTick(160000000);

	Driver_LED_Init();
	//Driver_Display_Init();

	// TODO: State machine loop
}
