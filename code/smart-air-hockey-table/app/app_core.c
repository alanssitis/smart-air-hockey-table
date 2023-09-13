#include "app_core.h"
#include "app_debug.h"
#include "driver_led.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_utils.h"

void App_Init()
{
	Debug_Init();
	Debug_Log("Init started...");

	// Configure SysTick to have 1ms time base, for LL_mDelay
	LL_RCC_ClocksTypeDef* rcc_clocks = {0};
	LL_RCC_GetSystemClocksFreq(rcc_clocks);
	LL_Init1msTick(rcc_clocks->HCLK_Frequency);

	Driver_LED_Init();

	Debug_Log("Init done!");

	// TODO: State machine loop
}
