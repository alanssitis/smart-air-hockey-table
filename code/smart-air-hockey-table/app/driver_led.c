#include <stddef.h>
#include "driver_led.h"
#include "stm32f4xx_ll_tim.h"
#include "stm32f4xx_ll_dma.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_utils.h"

#define LED_MATRIX_WIDTH 30
#define LED_MATRIX_HEIGHT 1
#define LED_COUNT (LED_MATRIX_WIDTH * LED_MATRIX_HEIGHT)
#define LED_RESET_COUNT 4
#define LED_BITS 24
#define LED_COMPARE_RESET 0
#define LED_COMPARE_OFF 34
#define LED_COMPARE_ON 69

static uint16_t dma_buffer[(LED_MATRIX_WIDTH * LED_MATRIX_HEIGHT + LED_RESET_COUNT) * LED_BITS];

// TODO: Remove LED_RESET_COUNT, make DMA non-circular, have DMA completion clear TIM3 CCR4, trigger DMA on demand instead

void Driver_LED_Init()
{
	// Attach DMA1 Stream 2 to TIM3
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
	LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_2, sizeof(dma_buffer) / sizeof(uint16_t));
	LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_2, (uint32_t) dma_buffer, (uint32_t) &(TIM3->CCR4), LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
	LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_2);

	// Start TIM3 Channel 4
	LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH4);
	LL_TIM_EnableDMAReq_UPDATE(TIM3);
	LL_TIM_SetUpdateSource(TIM3, LL_TIM_UPDATESOURCE_COUNTER);
	LL_TIM_EnableCounter(TIM3);
}

void Driver_LED_SetColor(uint8_t x, uint8_t y, uint32_t color)
{
	const size_t led_index = (x + y * LED_MATRIX_WIDTH) * LED_BITS;

	// Yeah there are "more clever" ways to do this, but this is nice and simple
	// WS2812 spec needs GRB msb first, but we have RGB
	dma_buffer[led_index + 0] = color & 0x008000 ? LED_COMPARE_ON : LED_COMPARE_OFF;
	dma_buffer[led_index + 1] = color & 0x004000 ? LED_COMPARE_ON : LED_COMPARE_OFF;
	dma_buffer[led_index + 2] = color & 0x002000 ? LED_COMPARE_ON : LED_COMPARE_OFF;
	dma_buffer[led_index + 3] = color & 0x001000 ? LED_COMPARE_ON : LED_COMPARE_OFF;
	dma_buffer[led_index + 4] = color & 0x000800 ? LED_COMPARE_ON : LED_COMPARE_OFF;
	dma_buffer[led_index + 5] = color & 0x000400 ? LED_COMPARE_ON : LED_COMPARE_OFF;
	dma_buffer[led_index + 6] = color & 0x000200 ? LED_COMPARE_ON : LED_COMPARE_OFF;
	dma_buffer[led_index + 7] = color & 0x000100 ? LED_COMPARE_ON : LED_COMPARE_OFF;

	dma_buffer[led_index + 8] = color & 0x800000 ? LED_COMPARE_ON : LED_COMPARE_OFF;
	dma_buffer[led_index + 9] = color & 0x400000 ? LED_COMPARE_ON : LED_COMPARE_OFF;
	dma_buffer[led_index + 10] = color & 0x200000 ? LED_COMPARE_ON : LED_COMPARE_OFF;
	dma_buffer[led_index + 11] = color & 0x100000 ? LED_COMPARE_ON : LED_COMPARE_OFF;
	dma_buffer[led_index + 12] = color & 0x080000 ? LED_COMPARE_ON : LED_COMPARE_OFF;
	dma_buffer[led_index + 13] = color & 0x040000 ? LED_COMPARE_ON : LED_COMPARE_OFF;
	dma_buffer[led_index + 14] = color & 0x020000 ? LED_COMPARE_ON : LED_COMPARE_OFF;
	dma_buffer[led_index + 15] = color & 0x010000 ? LED_COMPARE_ON : LED_COMPARE_OFF;

	dma_buffer[led_index + 16] = color & 0x000080 ? LED_COMPARE_ON : LED_COMPARE_OFF;
	dma_buffer[led_index + 17] = color & 0x000040 ? LED_COMPARE_ON : LED_COMPARE_OFF;
	dma_buffer[led_index + 18] = color & 0x000020 ? LED_COMPARE_ON : LED_COMPARE_OFF;
	dma_buffer[led_index + 19] = color & 0x000010 ? LED_COMPARE_ON : LED_COMPARE_OFF;
	dma_buffer[led_index + 20] = color & 0x000008 ? LED_COMPARE_ON : LED_COMPARE_OFF;
	dma_buffer[led_index + 21] = color & 0x000004 ? LED_COMPARE_ON : LED_COMPARE_OFF;
	dma_buffer[led_index + 22] = color & 0x000002 ? LED_COMPARE_ON : LED_COMPARE_OFF;
	dma_buffer[led_index + 23] = color & 0x000001 ? LED_COMPARE_ON : LED_COMPARE_OFF;
}
