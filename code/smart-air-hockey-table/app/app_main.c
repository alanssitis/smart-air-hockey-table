#include <stdio.h>

#include "app_main.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_tim.h"

void app_main()
{
	// Configure SysTick to have 1ms time base
	LL_RCC_ClocksTypeDef* rcc_clocks = {0};
	LL_RCC_GetSystemClocksFreq(rcc_clocks);
	LL_Init1msTick(rcc_clocks->HCLK_Frequency);

	LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH1);
	LL_TIM_EnableAllOutputs(TIM3);
	LL_TIM_EnableCounter(TIM3);

	uint32_t time = 0;
	uint32_t compare = 1;
	while (1)
	{
		LL_mDelay(500);
		time += 500;
		printf("Time: %.3f s\r\n", time / 1000.0f);

		LL_TIM_OC_SetCompareCH1(TIM3, 0);
		LL_mDelay(1);
		LL_TIM_OC_SetCompareCH1(TIM3, compare);
		if (compare == 1) compare = 2;
		else compare = 1;
	}
}
