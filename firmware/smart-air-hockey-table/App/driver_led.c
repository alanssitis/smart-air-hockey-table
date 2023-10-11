#include "driver_led.h"

#include <stddef.h>
#include "stm32f4xx_ll_tim.h"
#include "stm32f4xx_ll_dma.h"
#include "stm32f4xx_ll_bus.h"

#define LED_MATRIX_WIDTH 30
#define LED_MATRIX_HEIGHT 1
#define LED_COUNT (LED_MATRIX_WIDTH * LED_MATRIX_HEIGHT)
#define LED_RESET_COUNT 3
#define LED_BITS 24
#define LED_COMPARE_RESET 0
#define LED_COMPARE_OFF 34
#define LED_COMPARE_ON 69
#define DMA_BUFFER_LENGTH (2 * LED_BITS)
#define COLOR_8BIT_R 23
#define COLOR_8BIT_G 15
#define COLOR_8BIT_B 7

static volatile uint32_t led_state[LED_COUNT];
static volatile uint16_t dma_buffer[DMA_BUFFER_LENGTH];
static volatile size_t led_index = 0;

static const uint16_t led_compare_on_off[2] = {LED_COMPARE_OFF, LED_COMPARE_ON};

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
	if (offset >= DMA_BUFFER_LENGTH / LED_BITS) return;
	uint8_t g_offset = offset * LED_BITS;
	uint8_t r_offset = g_offset + 8;
	uint8_t b_offset = g_offset + 16;

	for (uint8_t i = 0; i < 8; i++)
	{
		dma_buffer[r_offset + i] = led_compare_on_off[(color >> (COLOR_8BIT_R - i)) & 0b1];
		dma_buffer[g_offset + i] = led_compare_on_off[(color >> (COLOR_8BIT_G - i)) & 0b1];
		dma_buffer[b_offset + i] = led_compare_on_off[(color >> (COLOR_8BIT_B - i)) & 0b1];
	}
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
