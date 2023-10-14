#include "app_core.h"

#include "stm32u5xx_ll_utils.h"

#include "driver_led.h"
#include "driver_display.h"

static volatile uint32_t time_ms;

void App_Init()
{
	// Configure SysTick to have 1 ms time based on 160 MHz, used by LL_mDelay and App_Tick
	LL_Init1msTick(160000000);

	Driver_LED_Init();
	Driver_Display_Init();
}

void App_Tick()
{
	time_ms++;

	// TODO: State machine logic

	Driver_LED_Tick();
}

uint32_t App_GetTime()
{
	return time_ms;
}
