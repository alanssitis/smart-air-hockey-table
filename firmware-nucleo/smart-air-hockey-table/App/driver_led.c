#include "driver_led.h"

#include <stdbool.h>
#include "stm32u5xx_ll_dma.h"
#include "stm32u5xx_ll_tim.h"

#define LED_CHANNELS 4
#define LED_RESET_MSG_SIZE (LED_COLOR_DATA_SIZE * 3)
#define LED_DATA_LENGTH (LED_MATRIX_PIXEL_COUNT * LED_COLOR_DATA_SIZE)
#define LED_RESET_LENGTH (LED_CHANNELS * LED_RESET_MSG_SIZE)
#define LED_BUFFER_LENGTH (LED_DATA_LENGTH + LED_RESET_LENGTH)

#define COLOR_COMPARE_OFF 3
#define COLOR_COMPARE_ON 6
#define COLOR_8BIT_R 23
#define COLOR_8BIT_G 15
#define COLOR_8BIT_B 7

static uint8_t led_buffer[LED_BUFFER_LENGTH] = {0};
static bool is_transfer_requested;
static bool is_transfer_active; // non-volatile: GPDMA1_Channel0_Handler has lower priority

void Driver_LED_Init()
{
	Driver_LED_Clear();

	// Enable transfer complete DMA interrupt
	LL_DMA_EnableIT_TC(GPDMA1, LL_DMA_CHANNEL_0);

	// Start TIM2 Channel 1
	LL_TIM_EnableDMAReq_UPDATE(TIM2);
	LL_TIM_ConfigDMABurst(TIM2, LL_TIM_DMABURST_BASEADDR_CCR1, LL_TIM_DMABURST_LENGTH_4TRANSFERS, LL_TIM_DMA_UPDATE);
	LL_TIM_SetUpdateSource(TIM2, LL_TIM_UPDATESOURCE_COUNTER);
	LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH1);
	LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH2);
	LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH3);
	LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH4);
	LL_TIM_EnableCounter(TIM2);
}

void Driver_LED_SetColor(uint_fast8_t row, uint_fast8_t col, Color color)
{
	if (row >= LED_MATRIX_ROW_SIZE || col >= LED_MATRIX_COL_SIZE) return;

	// Adjust for snaking pattern by reversing every other row
	if (col & 1) row = (LED_MATRIX_ROW_SIZE - 1) - row;

	// (r, c) to linear index
	uint_fast32_t pixel_index = row + col * LED_MATRIX_ROW_SIZE;

	// Determine which channel LED is in
	uint_fast32_t channel = pixel_index / (LED_MATRIX_PIXEL_COUNT / LED_CHANNELS);

	// Transform to a per-channel linear index
	pixel_index %= LED_MATRIX_PIXEL_COUNT / LED_CHANNELS;

	// Offset of each color channel within the 24 buffer values that define a pixel
	uint_fast32_t g_offset = pixel_index * LED_COLOR_DATA_SIZE;
	uint_fast32_t r_offset = g_offset + 8;
	uint_fast32_t b_offset = g_offset + 16;

	for (uint_fast8_t i = 0; i < 8; i++)
	{
		uint_fast8_t r_bit = (color.red >> (7 - i)) & 0b1;
		uint_fast8_t g_bit = (color.green >> (7 - i)) & 0b1;
		uint_fast8_t b_bit = (color.blue >> (7 - i)) & 0b1;
		led_buffer[(r_offset + i) * LED_CHANNELS + channel] = (!r_bit) * COLOR_COMPARE_OFF + r_bit * COLOR_COMPARE_ON;
		led_buffer[(g_offset + i) * LED_CHANNELS + channel] = (!g_bit) * COLOR_COMPARE_OFF + g_bit * COLOR_COMPARE_ON;
		led_buffer[(b_offset + i) * LED_CHANNELS + channel] = (!b_bit) * COLOR_COMPARE_OFF + b_bit * COLOR_COMPARE_ON;
	}
	is_transfer_requested = true;
}

void Driver_LED_Clear()
{
	for (uint_fast32_t i = 0; i < LED_DATA_LENGTH; i++)
	{
		led_buffer[i] = COLOR_COMPARE_OFF;
	}
	is_transfer_requested = true;
}

void Driver_LED_Tick()
{
	if (is_transfer_requested && !is_transfer_active)
	{
		is_transfer_requested = false;
		is_transfer_active = true;

		// Configure GPDMA Channel to initiate transfer
		LL_DMA_ConfigAddresses(GPDMA1, LL_DMA_CHANNEL_0, (uint32_t) led_buffer, (uint32_t) &(TIM2->DMAR));
		LL_DMA_SetBlkDataLength(GPDMA1, LL_DMA_CHANNEL_0, LED_BUFFER_LENGTH);
		LL_DMA_EnableChannel(GPDMA1, LL_DMA_CHANNEL_0);
	}
}

void GPDMA1_Channel0_Handler()
{
	is_transfer_active = false;
}
