#include "driver_led.h"

#include <stdbool.h>
#include <stddef.h>
#include "stm32u5xx_ll_dma.h"
#include "stm32u5xx_ll_tim.h"

#define LED_MATRIX_WIDTH 2
#define LED_MATRIX_HEIGHT 8
#define LED_BITS 24
#define LED_CHANNELS 4
#define LED_RESET_LENGTH (LED_BITS * 3)
#define LED_COMPARE_RESET 0
#define LED_COMPARE_OFF 3
#define LED_COMPARE_ON 6
#define DMA_BUFFER_LENGTH (LED_MATRIX_WIDTH * LED_MATRIX_HEIGHT * LED_BITS / LED_CHANNELS)
#define COLOR_8BIT_R 23
#define COLOR_8BIT_G 15
#define COLOR_8BIT_B 7

static volatile uint8_t dma_buffer[LED_CHANNELS][DMA_BUFFER_LENGTH + LED_RESET_LENGTH]; // Leave space for reset values
static volatile bool is_transfer_requested;
static volatile bool is_transfer_active;
static volatile size_t transfers_active;

void Driver_LED_Init()
{
	Driver_LED_Clear();

	// Populate reset values at end of each channel
	for (size_t channel = 0; channel < LED_CHANNELS; channel++)
	{
		for (size_t i = 0; i < LED_RESET_LENGTH; i++)
		{
			dma_buffer[channel][DMA_BUFFER_LENGTH + i] = LED_COMPARE_RESET;
		}
	}

	// Enable transfer complete DMA interrupt
	LL_DMA_EnableIT_TC(GPDMA1, LL_DMA_CHANNEL_0);
	LL_DMA_EnableIT_TC(GPDMA1, LL_DMA_CHANNEL_1);
	LL_DMA_EnableIT_TC(GPDMA1, LL_DMA_CHANNEL_2);
	LL_DMA_EnableIT_TC(GPDMA1, LL_DMA_CHANNEL_3);

	// Start TIM2 Channel 1
	LL_TIM_EnableDMAReq_UPDATE(TIM2);
	LL_TIM_SetUpdateSource(TIM2, LL_TIM_UPDATESOURCE_COUNTER);
	LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH1);
	LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH2);
	LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH3);
	LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH4);
	LL_TIM_EnableCounter(TIM2);
}

void Driver_LED_SetColor(uint8_t x, uint8_t y, uint32_t color)
{
	if (x >= LED_MATRIX_WIDTH || y >= LED_MATRIX_HEIGHT) return;

	// Remaps XY coords to per-channel physical LED index
	if (y & 1) x = (LED_MATRIX_WIDTH - 1) - x;
	size_t pixel_index = x + y * LED_MATRIX_WIDTH;
	size_t channel = pixel_index / (DMA_BUFFER_LENGTH / LED_BITS);
	pixel_index %= DMA_BUFFER_LENGTH / LED_BITS;

	// Offset of each color channel within the 24 buffer values that define a pixel
	size_t g_offset = pixel_index * LED_BITS;
	size_t r_offset = g_offset + 8;
	size_t b_offset = g_offset + 16;

	for (uint8_t i = 0; i < 8; i++)
	{
		uint8_t r_bit = (color >> (COLOR_8BIT_R - i)) & 0b1;
		uint8_t g_bit = (color >> (COLOR_8BIT_G - i)) & 0b1;
		uint8_t b_bit = (color >> (COLOR_8BIT_B - i)) & 0b1;
		dma_buffer[channel][r_offset + i] = (!r_bit) * LED_COMPARE_OFF + r_bit * LED_COMPARE_ON;
		dma_buffer[channel][g_offset + i] = (!g_bit) * LED_COMPARE_OFF + g_bit * LED_COMPARE_ON;
		dma_buffer[channel][b_offset + i] = (!b_bit) * LED_COMPARE_OFF + b_bit * LED_COMPARE_ON;
	}
	is_transfer_requested = true;
}

void Driver_LED_Clear()
{
	for (size_t channel = 0; channel < LED_CHANNELS; channel++)
	{
		for (size_t i = 0; i < DMA_BUFFER_LENGTH; i++)
		{
			dma_buffer[channel][i] = LED_COMPARE_OFF;
		}
	}
	is_transfer_requested = true;
}

void Driver_LED_Tick()
{
	if (is_transfer_requested && !is_transfer_active)
	{
		is_transfer_requested = false;
		is_transfer_active = true;
		transfers_active = LED_CHANNELS;

		// Configure GPDMA Channels to initiate transfer
		LL_DMA_ConfigAddresses(GPDMA1, LL_DMA_CHANNEL_0, (uint32_t) dma_buffer[0], (uint32_t) &(TIM2->CCR1));
		LL_DMA_ConfigAddresses(GPDMA1, LL_DMA_CHANNEL_1, (uint32_t) dma_buffer[1], (uint32_t) &(TIM2->CCR2));
		LL_DMA_ConfigAddresses(GPDMA1, LL_DMA_CHANNEL_2, (uint32_t) dma_buffer[2], (uint32_t) &(TIM2->CCR3));
		LL_DMA_ConfigAddresses(GPDMA1, LL_DMA_CHANNEL_3, (uint32_t) dma_buffer[3], (uint32_t) &(TIM2->CCR4));
		LL_DMA_SetBlkDataLength(GPDMA1, LL_DMA_CHANNEL_0, DMA_BUFFER_LENGTH + LED_RESET_LENGTH);
		LL_DMA_SetBlkDataLength(GPDMA1, LL_DMA_CHANNEL_1, DMA_BUFFER_LENGTH + LED_RESET_LENGTH);
		LL_DMA_SetBlkDataLength(GPDMA1, LL_DMA_CHANNEL_2, DMA_BUFFER_LENGTH + LED_RESET_LENGTH);
		LL_DMA_SetBlkDataLength(GPDMA1, LL_DMA_CHANNEL_3, DMA_BUFFER_LENGTH + LED_RESET_LENGTH);
		LL_DMA_EnableChannel(GPDMA1, LL_DMA_CHANNEL_0);
		LL_DMA_EnableChannel(GPDMA1, LL_DMA_CHANNEL_1);
		LL_DMA_EnableChannel(GPDMA1, LL_DMA_CHANNEL_2);
		LL_DMA_EnableChannel(GPDMA1, LL_DMA_CHANNEL_3);
	}
}

void GPDMA1_Channel0123_Handler()
{
	if (--transfers_active == 0) is_transfer_active = false;
}
