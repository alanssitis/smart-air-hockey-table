#include "app_core.h"

#include "stm32u5xx_ll_tim.h"
#include "stm32u5xx_ll_utils.h"

#include "driver_led.h"
//#include "driver_display.h"

static volatile uint32_t ticks_elapsed;

void App_Init()
{
	// Configure SysTick to have 1 ms time based on 160 MHz, used by LL_mDelay
	LL_Init1msTick(160000000);

	Driver_LED_Init();
	//Driver_Display_Init();

	// Start 1000 Hz "superloop"
	LL_TIM_SetUpdateSource(TIM6, LL_TIM_UPDATESOURCE_COUNTER);
	LL_TIM_SetUpdateSource(TIM7, LL_TIM_UPDATESOURCE_COUNTER);
	LL_TIM_EnableIT_UPDATE(TIM6);
	LL_TIM_EnableIT_UPDATE(TIM7);
	LL_TIM_EnableCounter(TIM6);
	LL_TIM_EnableCounter(TIM7);
}

void TIM6_Handler()
{
	ticks_elapsed++;
}

void TIM7_Handler()
{
	// TODO: State machine logic

	Driver_LED_Tick();
}

uint32_t App_GetTicksElapsed()
{
	return ticks_elapsed;
}
