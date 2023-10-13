#include "driver_led.h"

#include <stddef.h>
#include "stm32u5xx_ll_dma.h"
#include "stm32u5xx_ll_tim.h"

#define LED_MATRIX_WIDTH 4
#define LED_MATRIX_HEIGHT 1
#define LED_COUNT (LED_MATRIX_WIDTH * LED_MATRIX_HEIGHT)
#define LED_RESET_COUNT 3
#define LED_BITS 24
#define LED_COMPARE_RESET 0
#define LED_COMPARE_OFF 3
#define LED_COMPARE_ON 6
#define DMA_BUFFER_LENGTH (2 * LED_BITS)
#define COLOR_8BIT_R 23
#define COLOR_8BIT_G 15
#define COLOR_8BIT_B 7

static volatile uint32_t dma_buffer[DMA_BUFFER_LENGTH];
const uint32_t dma_buffer_address = (uint32_t) dma_buffer;
const uint32_t dma_buffer_size = sizeof(dma_buffer);

static volatile uint32_t led_state[LED_COUNT];
static volatile size_t led_index = 0;

static const uint32_t led_compare_on_off[2] = {LED_COMPARE_OFF, LED_COMPARE_ON};

void Driver_LED_Init()
{
	// Attach GPDMA1 Channel 0 to TIM2
	LL_DMA_EnableIT_HT(GPDMA1, LL_DMA_CHANNEL_0);
	LL_DMA_EnableIT_TC(GPDMA1, LL_DMA_CHANNEL_0);

	// Start TIM2 Channel 1
	LL_TIM_SetUpdateSource(TIM2, LL_TIM_UPDATESOURCE_COUNTER);
	LL_TIM_EnableDMAReq_UPDATE(TIM2);
	LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH1);
	LL_TIM_EnableCounter(TIM2);
}

void Driver_LED_SetColor(uint8_t x, uint8_t y, uint32_t color)
{
	if (x >= LED_MATRIX_WIDTH || y >= LED_MATRIX_HEIGHT) return;

	// Remaps XY coords to physical LED index
	led_state[x + y * LED_MATRIX_WIDTH] = color;
}

static void write_buffer(uint32_t color, uint8_t offset)
{
//	uint8_t g_offset = offset * LED_BITS;
//	uint8_t r_offset = g_offset + 8;
//	uint8_t b_offset = g_offset + 16;
//
//	for (uint8_t i = 0; i < 8; i++)
//	{
//		dma_buffer[r_offset + i] = led_compare_on_off[(color >> (COLOR_8BIT_R - i)) & 0b1];
//		dma_buffer[g_offset + i] = led_compare_on_off[(color >> (COLOR_8BIT_G - i)) & 0b1];
//		dma_buffer[b_offset + i] = led_compare_on_off[(color >> (COLOR_8BIT_B - i)) & 0b1];
//	}

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

void GPDMA1_Channel0_Handler(uint8_t offset)
{
	if (led_index < LED_COUNT) write_buffer(led_state[led_index], offset);
	else clear_buffer(offset);

	// Loop back around
	if (++led_index == LED_COUNT + LED_RESET_COUNT) led_index = 0;
}
