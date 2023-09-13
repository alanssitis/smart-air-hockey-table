#include <stddef.h>
#include "driver_led.h"
#include "stm32f4xx_ll_tim.h"
#include "stm32f4xx_ll_dma.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_utils.h"

#define LED_MATRIX_WIDTH 30
#define LED_MATRIX_HEIGHT 1
#define LED_COUNT (LED_MATRIX_WIDTH * LED_MATRIX_HEIGHT)
#define LED_RESET_COUNT 3
#define LED_BITS 24
#define LED_COMPARE_RESET 0
#define LED_COMPARE_OFF 34
#define LED_COMPARE_ON 69
#define DMA_BUFFER_LENGTH (2 * LED_BITS)

static volatile uint32_t led_state[LED_MATRIX_WIDTH * LED_MATRIX_HEIGHT];
static volatile uint16_t dma_buffer[DMA_BUFFER_LENGTH];
static volatile size_t led_index = 0;

void Driver_LED_Init()
{
	// Attach DMA1 Stream 2 to TIM3
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
	LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_2, DMA_BUFFER_LENGTH);
	LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_2, (uint32_t) dma_buffer, (uint32_t) &(TIM3->CCR4), LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
	LL_DMA_EnableIT_HT(DMA1, LL_DMA_STREAM_2);
	LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_2);
	LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_2);

	// Start TIM3 Channel 4
	LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH4);
	LL_TIM_EnableDMAReq_UPDATE(TIM3);
	LL_TIM_SetUpdateSource(TIM3, LL_TIM_UPDATESOURCE_COUNTER);
	LL_TIM_EnableCounter(TIM3);
}

void Driver_LED_SetColor(uint8_t x, uint8_t y, uint32_t color)
{
	if (x >= LED_MATRIX_WIDTH || y >= LED_MATRIX_HEIGHT) return;

	// Remaps XY coords to physical LED index
	led_state[x + y * LED_MATRIX_WIDTH] = color;
}

static void write_buffer(uint32_t color, uint8_t offset)
{
	offset *= LED_BITS;
	dma_buffer[offset + 0] = color & 0x008000 ? LED_COMPARE_ON : LED_COMPARE_OFF;
	dma_buffer[offset + 1] = color & 0x004000 ? LED_COMPARE_ON : LED_COMPARE_OFF;
	dma_buffer[offset + 2] = color & 0x002000 ? LED_COMPARE_ON : LED_COMPARE_OFF;
	dma_buffer[offset + 3] = color & 0x001000 ? LED_COMPARE_ON : LED_COMPARE_OFF;
	dma_buffer[offset + 4] = color & 0x000800 ? LED_COMPARE_ON : LED_COMPARE_OFF;
	dma_buffer[offset + 5] = color & 0x000400 ? LED_COMPARE_ON : LED_COMPARE_OFF;
	dma_buffer[offset + 6] = color & 0x000200 ? LED_COMPARE_ON : LED_COMPARE_OFF;
	dma_buffer[offset + 7] = color & 0x000100 ? LED_COMPARE_ON : LED_COMPARE_OFF;

	dma_buffer[offset + 8] = color & 0x800000 ? LED_COMPARE_ON : LED_COMPARE_OFF;
	dma_buffer[offset + 9] = color & 0x400000 ? LED_COMPARE_ON : LED_COMPARE_OFF;
	dma_buffer[offset + 10] = color & 0x200000 ? LED_COMPARE_ON : LED_COMPARE_OFF;
	dma_buffer[offset + 11] = color & 0x100000 ? LED_COMPARE_ON : LED_COMPARE_OFF;
	dma_buffer[offset + 12] = color & 0x080000 ? LED_COMPARE_ON : LED_COMPARE_OFF;
	dma_buffer[offset + 13] = color & 0x040000 ? LED_COMPARE_ON : LED_COMPARE_OFF;
	dma_buffer[offset + 14] = color & 0x020000 ? LED_COMPARE_ON : LED_COMPARE_OFF;
	dma_buffer[offset + 15] = color & 0x010000 ? LED_COMPARE_ON : LED_COMPARE_OFF;

	dma_buffer[offset + 16] = color & 0x000080 ? LED_COMPARE_ON : LED_COMPARE_OFF;
	dma_buffer[offset + 17] = color & 0x000040 ? LED_COMPARE_ON : LED_COMPARE_OFF;
	dma_buffer[offset + 18] = color & 0x000020 ? LED_COMPARE_ON : LED_COMPARE_OFF;
	dma_buffer[offset + 19] = color & 0x000010 ? LED_COMPARE_ON : LED_COMPARE_OFF;
	dma_buffer[offset + 20] = color & 0x000008 ? LED_COMPARE_ON : LED_COMPARE_OFF;
	dma_buffer[offset + 21] = color & 0x000004 ? LED_COMPARE_ON : LED_COMPARE_OFF;
	dma_buffer[offset + 22] = color & 0x000002 ? LED_COMPARE_ON : LED_COMPARE_OFF;
	dma_buffer[offset + 23] = color & 0x000001 ? LED_COMPARE_ON : LED_COMPARE_OFF;
}

static void clear_buffer(uint8_t offset)
{
	offset *= LED_BITS;
	for (uint8_t i = 0; i < LED_BITS; i++)
	{
		dma_buffer[offset + i] = LED_COMPARE_RESET;
	}
}

void DMA1_Handler(uint8_t offset)
{
	if (led_index < LED_COUNT) write_buffer(led_state[led_index], offset);
	else clear_buffer(offset);

	// Loop back around
	if (++led_index == LED_COUNT + LED_RESET_COUNT) led_index = 0;
}
