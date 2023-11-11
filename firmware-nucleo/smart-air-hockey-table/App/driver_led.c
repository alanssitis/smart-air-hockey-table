#include "driver_led.h"

#include <stdbool.h>
#include "stm32u5xx_ll_dma.h"
#include "stm32u5xx_ll_tim.h"
#include "precomputed_color_data_field.h"

#define LED_CHANNELS 4
#define LED_CHANNEL_DATA_LENGTH (LED_MATRIX_PIXEL_COUNT * LED_COLOR_DATA_SIZE / LED_CHANNELS)
#define LED_CHANNEL_RESET_LENGTH (LED_COLOR_DATA_SIZE * 3)
#define LED_CHANNEL_LENGTH (LED_CHANNEL_DATA_LENGTH + LED_CHANNEL_RESET_LENGTH)
#define COLOR_COMPARE_OFF 3

static uint8_t led_buffer[LED_CHANNELS][LED_CHANNEL_LENGTH] = {0};
static bool is_transfer_requested;
static uint_fast8_t active_transfers_mask; // non-volatile: GPDMA1_Channel0123_Handler has lower priority

void Driver_LED_Init()
{
	Driver_LED_Clear();

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

void Driver_LED_SetColor(uint_fast8_t col, uint_fast8_t row, Color color)
{
	if (row >= LED_MATRIX_ROW_NUM || col >= LED_MATRIX_COL_NUM) return;

	// Adjust for snaking pattern by reversing every other row
	if (row & 1) col = (LED_MATRIX_COL_NUM - 1) - col;

	// (r, c) to linear index
	uint_fast32_t pixel_index = col + row * LED_MATRIX_COL_NUM;

	// Determine which channel LED is in
	uint_fast32_t channel = pixel_index / (LED_MATRIX_PIXEL_COUNT / LED_CHANNELS);

	// Transform to a per-channel linear index
	pixel_index %= LED_MATRIX_PIXEL_COUNT / LED_CHANNELS;

	// Color channel base address with pointer magic
	uint64_t *color_base = (uint64_t *) &(led_buffer[channel][pixel_index * LED_COLOR_DATA_SIZE]);

	// Assign values
	*(color_base++) = color_data_field[color.green];
	*(color_base++) = color_data_field[color.red];
	*(color_base) = color_data_field[color.blue];

	is_transfer_requested = true;
}

void Driver_LED_Clear()
{
	for (uint_fast32_t channel = 0; channel < LED_CHANNELS; channel++)
	{
		for (uint_fast32_t i = 0; i < LED_CHANNEL_DATA_LENGTH; i++)
		{
			led_buffer[channel][i] = COLOR_COMPARE_OFF;
		}
	}
	is_transfer_requested = true;
}

void Driver_LED_Tick()
{
	if (is_transfer_requested && active_transfers_mask == 0)
	{
		is_transfer_requested = false;
		active_transfers_mask = 0xF;

		// Configure GPDMA Channel to initiate transfer
		LL_DMA_ConfigAddresses(GPDMA1, LL_DMA_CHANNEL_0, (uint32_t) led_buffer[0], (uint32_t) &(TIM2->CCR1));
		LL_DMA_ConfigAddresses(GPDMA1, LL_DMA_CHANNEL_1, (uint32_t) led_buffer[1], (uint32_t) &(TIM2->CCR2));
		LL_DMA_ConfigAddresses(GPDMA1, LL_DMA_CHANNEL_2, (uint32_t) led_buffer[2], (uint32_t) &(TIM2->CCR3));
		LL_DMA_ConfigAddresses(GPDMA1, LL_DMA_CHANNEL_3, (uint32_t) led_buffer[3], (uint32_t) &(TIM2->CCR4));
		LL_DMA_SetBlkDataLength(GPDMA1, LL_DMA_CHANNEL_0, LED_CHANNEL_LENGTH);
		LL_DMA_SetBlkDataLength(GPDMA1, LL_DMA_CHANNEL_1, LED_CHANNEL_LENGTH);
		LL_DMA_SetBlkDataLength(GPDMA1, LL_DMA_CHANNEL_2, LED_CHANNEL_LENGTH);
		LL_DMA_SetBlkDataLength(GPDMA1, LL_DMA_CHANNEL_3, LED_CHANNEL_LENGTH);
		LL_DMA_EnableChannel(GPDMA1, LL_DMA_CHANNEL_0);
		LL_DMA_EnableChannel(GPDMA1, LL_DMA_CHANNEL_1);
		LL_DMA_EnableChannel(GPDMA1, LL_DMA_CHANNEL_2);
		LL_DMA_EnableChannel(GPDMA1, LL_DMA_CHANNEL_3);

	}
}

void GPDMA1_Channel0123_Handler(uint_fast8_t channel)
{
	active_transfers_mask ^= 1 << channel;
}
