#include "driver_led.h"

#include <stddef.h>
#include "stm32u5xx_ll_dma.h"
#include "stm32u5xx_ll_tim.h"

#define LED_MATRIX_WIDTH 2
#define LED_MATRIX_HEIGHT 2
#define LED_BITS 24
#define LED_COMPARE_RESET 0
#define LED_COMPARE_OFF 3
#define LED_COMPARE_ON 6
#define DMA_BUFFER_LENGTH (LED_MATRIX_WIDTH * LED_MATRIX_HEIGHT * LED_BITS)
#define COLOR_8BIT_R 23
#define COLOR_8BIT_G 15
#define COLOR_8BIT_B 7

static volatile uint8_t dma_buffer[DMA_BUFFER_LENGTH + 1]; // Leave space for reset value
static volatile uint8_t is_transfer_requested;
static volatile uint8_t is_transfer_active;

void Driver_LED_Init()
{
	Driver_LED_Clear();
	dma_buffer[DMA_BUFFER_LENGTH] = LED_COMPARE_RESET;

	// Enable transfer complete DMA interrupt
	LL_DMA_EnableIT_TC(GPDMA1, LL_DMA_CHANNEL_0);

	// Start TIM2 Channel 1
	LL_TIM_EnableDMAReq_UPDATE(TIM2);
	LL_TIM_SetUpdateSource(TIM2, LL_TIM_UPDATESOURCE_COUNTER);
	LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH1);
	LL_TIM_EnableCounter(TIM2);
}

void Driver_LED_SetColor(uint8_t x, uint8_t y, uint32_t color)
{
	if (x >= LED_MATRIX_WIDTH || y >= LED_MATRIX_HEIGHT) return;

	// Remaps XY coords to physical LED index
	if (y & 1) x = (LED_MATRIX_WIDTH - 1) - x;
	size_t pixel_index = x + y * LED_MATRIX_WIDTH;

	// Offset of each color channel within the 24 buffer values that define a pixel
	size_t g_offset = pixel_index * LED_BITS;
	size_t r_offset = g_offset + 8;
	size_t b_offset = g_offset + 16;

	for (uint8_t i = 0; i < 8; i++)
	{
		uint8_t r_bit = (color >> (COLOR_8BIT_R - i)) & 0b1;
		uint8_t g_bit = (color >> (COLOR_8BIT_G - i)) & 0b1;
		uint8_t b_bit = (color >> (COLOR_8BIT_B - i)) & 0b1;
		dma_buffer[r_offset + i] = (~r_bit) * LED_COMPARE_OFF + r_bit * LED_COMPARE_ON;
		dma_buffer[g_offset + i] = (~g_bit) * LED_COMPARE_OFF + g_bit * LED_COMPARE_ON;
		dma_buffer[b_offset + i] = (~b_bit) * LED_COMPARE_OFF + b_bit * LED_COMPARE_ON;
	}
	is_transfer_requested = 1;
}

void Driver_LED_Clear()
{
	for (size_t i = 0; i < DMA_BUFFER_LENGTH; i++)
	{
		dma_buffer[i] = LED_COMPARE_OFF;
	}
	is_transfer_requested = 1;
}

void Driver_LED_Tick()
{
	if (is_transfer_requested && !is_transfer_active)
	{
		is_transfer_requested = 0;
		is_transfer_active = 1;

		// Configure GPDMA Channel 0
		LL_DMA_ConfigAddresses(GPDMA1, LL_DMA_CHANNEL_0, (uint32_t) dma_buffer, (uint32_t) &(TIM2->CCR1));
		LL_DMA_SetBlkDataLength(GPDMA1, LL_DMA_CHANNEL_0, sizeof(dma_buffer));
		LL_DMA_EnableChannel(GPDMA1, LL_DMA_CHANNEL_0);
	}
}

void GPDMA1_Channel0_Handler()
{
	is_transfer_active = 0;
}
