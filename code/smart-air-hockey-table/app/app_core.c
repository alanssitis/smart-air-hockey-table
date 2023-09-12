#include "app_core.h"
#include "app_debug.h"
#include "driver_led.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_utils.h"

static void loop_test();

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
	loop_test();
}

static void loop_test()
{
	// NOTE: End goal is to remove any "while true" loops,
	// and solely rely on timer interrupts

	uint8_t offset = 0;
	while (1)
	{
		LL_mDelay(100);

		for (uint8_t i = 0; i < 30; i++)
		{
			if ((i + offset) & 4)
			{
				Driver_LED_SetColor(i, 0, ((i + offset) & 2) ? 0x001100 : 0x110000);
			}
			else
			{
				Driver_LED_SetColor(i, 0, ((i + offset) & 2) ? 0x000011 : 0x111100);
			}
		}
		if (++offset == 8)
		{
			offset = 0;
			Debug_Log("LEDs looped");
		}
	}
}
