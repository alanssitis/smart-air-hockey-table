#include <stdio.h>

#include "app_main.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_tim.h"

#define LED_COUNT 8
#define LED_RESET_COUNT 4
#define LED_BITS 24
#define LED_COMPARE_RESET 0
#define LED_COMPARE_OFF 34
#define LED_COMPARE_ON 69

volatile uint8_t dma_buffer[(LED_COUNT + LED_RESET_COUNT) * LED_BITS];

void app_main()
{
	// Configure SysTick to have 1ms time base
	LL_RCC_ClocksTypeDef* rcc_clocks = {0};
	LL_RCC_GetSystemClocksFreq(rcc_clocks);
	LL_Init1msTick(rcc_clocks->HCLK_Frequency);

	LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH1);
	LL_TIM_EnableAllOutputs(TIM3);
	LL_TIM_EnableCounter(TIM3);

	for (uint32_t i = 0; i < LED_COUNT * LED_BITS; i++)
	{
		dma_buffer[i + LED_RESET_COUNT * LED_BITS] = LED_COMPARE_ON;
	}

	uint32_t time = 0;
	while (1)
	{
		LL_mDelay(500);
		time += 500;
		printf("Time: %.3f s\r\n", time / 1000.0f);
	}
}
