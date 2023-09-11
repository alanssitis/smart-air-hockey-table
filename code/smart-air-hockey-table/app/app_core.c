#include <app_core.h>
#include <stdio.h>

#include "driver_led.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_utils.h"

void App_Init()
{
	// Configure SysTick to have 1ms time base
	LL_RCC_ClocksTypeDef* rcc_clocks = {0};
	LL_RCC_GetSystemClocksFreq(rcc_clocks);
	LL_Init1msTick(rcc_clocks->HCLK_Frequency);

	Driver_LED_Init();

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
		if (++offset == 8) offset = 0;
	}
}
