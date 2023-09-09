#include <stdio.h>

#include "app_main.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_tim.h"
#include "stm32f4xx_ll_dma.h"
#include "stm32f4xx_ll_bus.h"

#define LED_COUNT 30
#define LED_RESET_COUNT 4
#define LED_BITS 24
#define LED_COMPARE_RESET 0
#define LED_COMPARE_OFF 34
#define LED_COMPARE_ON 69

uint8_t curr_r = 0xFF;
uint8_t curr_g = 0;
uint8_t curr_b = 0;

volatile uint16_t dma_buffer[(LED_COUNT + LED_RESET_COUNT) * LED_BITS];

void app_main()
{
	// Configure SysTick to have 1ms time base
	LL_RCC_ClocksTypeDef* rcc_clocks = {0};
	LL_RCC_GetSystemClocksFreq(rcc_clocks);
	LL_Init1msTick(rcc_clocks->HCLK_Frequency);

	LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH4);
	LL_TIM_EnableAllOutputs(TIM3);
	LL_TIM_EnableCounter(TIM3);

//	LL_DMA_SetPeriphSize(DMA1, LL_DMA_STREAM_2, LL_DMA_PDATAALIGN_BYTE);
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
	LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_2, ((LED_COUNT + LED_RESET_COUNT) * LED_BITS));
	LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_2, (uint32_t) dma_buffer, (uint32_t) &(TIM3->CCR4), LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
//	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);
	LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_2);
	TIM3->CR1 |= TIM_CR1_URS;
//	TIM3->CR2 |= TIM_CR2_CCDS;
//	TIM3->DIER |= TIM_DIER_UIE;
	TIM3->DIER |= TIM_DIER_UDE;
//	LL_TIM_EnableDMAReq_UPDATE(TIM3);
//	LL_DMA_SetPeriphAddress(DMA1, LL_DMA_STREAM_2, (uint32_t) &(TIM3->CCR1));
//	LL_DMA_SetMemoryAddress(DMA1, LL_DMA_STREAM_2, (uint32_t) dma_buffer);

//	printf("%d\r\n", sizeof(dma_buffer));
	while(1)
	{
		for (uint32_t i = 0; i < LED_COUNT; i++)
		{
			for (uint8_t red = 0; red < 8; red++)
			{
				dma_buffer[(i * LED_BITS) + 8 + red + LED_RESET_COUNT * LED_BITS] = (curr_r & (0x80 >> red)) ? LED_COMPARE_ON : LED_COMPARE_OFF;
			}

			for (uint8_t green = 0; green < 8; green++)
			{
				dma_buffer[(i * LED_BITS) + 0 + green + LED_RESET_COUNT * LED_BITS] = (curr_g & (0x80 >> green)) ? LED_COMPARE_ON : LED_COMPARE_OFF;
			}

			for (uint8_t blue = 0; blue < 8; blue++)
			{
				dma_buffer[(i * LED_BITS) + 16 + blue + LED_RESET_COUNT * LED_BITS] = (curr_b & (0x80 >> blue)) ? LED_COMPARE_ON : LED_COMPARE_OFF;
			}

//			for (uint32_t j = 0; j < LED_BITS; j++)
//			{
//				dma_buffer[(i * LED_BITS) + 0 + LED_RESET_COUNT * LED_BITS] = (curr_g & 0x80) ? LED_COMPARE_ON : LED_COMPARE_OFF;
//				dma_buffer[(i * LED_BITS) + 1 + LED_RESET_COUNT * LED_BITS] = (curr_g & 0x40) ? LED_COMPARE_ON : LED_COMPARE_OFF;
//				dma_buffer[(i * LED_BITS) + 2 + LED_RESET_COUNT * LED_BITS] = (curr_g & 0x80) ? LED_COMPARE_ON : LED_COMPARE_OFF;
//				dma_buffer[(i * LED_BITS) + 3 + LED_RESET_COUNT * LED_BITS] = (curr_g & 0x40) ? LED_COMPARE_ON : LED_COMPARE_OFF;
//				dma_buffer[(i * LED_BITS) + 4 + LED_RESET_COUNT * LED_BITS] = (curr_g & 0x80) ? LED_COMPARE_ON : LED_COMPARE_OFF;
//				dma_buffer[(i * LED_BITS) + 5 + LED_RESET_COUNT * LED_BITS] = (curr_g & 0x40) ? LED_COMPARE_ON : LED_COMPARE_OFF;
//				dma_buffer[(i * LED_BITS) + 6 + LED_RESET_COUNT * LED_BITS] = (curr_g & 0x80) ? LED_COMPARE_ON : LED_COMPARE_OFF;
//				dma_buffer[(i * LED_BITS) + 7 + LED_RESET_COUNT * LED_BITS] = (curr_g & 0x40) ? LED_COMPARE_ON : LED_COMPARE_OFF;
//
//				dma_buffer[(i * LED_BITS) + j + LED_RESET_COUNT * LED_BITS] = LED_COMPARE_OFF;
////				if (curr_led == i) dma_buffer[(i * LED_BITS) + j + LED_RESET_COUNT * LED_BITS] = LED_COMPARE_ON;
//
////				dma_buffer[(i * LED_BITS) + 0 + LED_RESET_COUNT * LED_BITS] = (curr_g & 0x80) ? LED_COMPARE_ON : LED_COMPARE_OFF;
////				dma_buffer[(i * LED_BITS) + 1 + LED_RESET_COUNT * LED_BITS] = (curr_g & 0x40) ? LED_COMPARE_ON : LED_COMPARE_OFF;
//
//				if (j < 8)
//				{
//					// green bits
//					if (curr_g & (0x80 >> (j % 8))) dma_buffer[(i * LED_BITS) + j + LED_RESET_COUNT * LED_BITS] = LED_COMPARE_ON;
//				}
//				else if (j < 16)
//				{
//					// red bits
//					if (curr_r & (0x80 >> (j % 8))) dma_buffer[(i * LED_BITS) + j + LED_RESET_COUNT * LED_BITS] = LED_COMPARE_ON;
//				}
//				else
//				{
//					// blue bits
//					if (curr_b & (0x80 >> (j % 8))) dma_buffer[(i * LED_BITS) + j + LED_RESET_COUNT * LED_BITS] = LED_COMPARE_ON;
//				}
	//			if (i % 2) dma_buffer[(i * LED_BITS) + j + LED_RESET_COUNT * LED_BITS] = LED_COMPARE_ON;
	//			else dma_buffer[(i * LED_BITS) + j + LED_RESET_COUNT * LED_BITS] = LED_COMPARE_OFF;
//			}
	//		printf("%lu\r\n", (i + LED_RESET_COUNT * LED_BITS));
		}
		if (curr_r == 0xFF && curr_g < 0xFF && curr_b == 0x00)
		{
			curr_g += 0x01;
		}
		else if (curr_r > 0x00 && curr_g == 0xFF && curr_b == 0x00)
		{
			curr_r -= 0x01;
		}
		else if (curr_r == 0x00 && curr_g == 0xFF && curr_b < 0xFF)
		{
			curr_b += 0x01;
		}
		else if (curr_r == 0x00 && curr_b == 0xFF && curr_g > 0x00)
		{
			curr_g -= 0x01;
		}
		else if (curr_r < 0xFF && curr_g == 0x00 && curr_b == 0xFF)
		{
			curr_r += 0x01;
		}
		else
		{
			curr_b -= 0x01;
		}
//		printf("%x %x %x\r\n", curr_r, curr_g, curr_b);
		LL_mDelay(1000 / 510);
	}

	uint32_t time = 0;
	while (1)
	{
		LL_mDelay(500);
		time += 500;
		printf("Time: %.3f %lu s\r\n", time / 1000.0f, TIM3->CCR4);
	}
}
