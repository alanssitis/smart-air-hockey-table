#include "driver_display.h"

#include <stddef.h>
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_spi.h"

#include "app_debug.h"

#define DISPLAY_COUNT 2
#define DISPLAY_COLUMNS 128
#define DISPLAY_PAGES 8
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

static void transmit_start(uint8_t display, uint8_t isData)
{
	// DC
	if (isData) LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_2);
	else LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_2);

	// CS#
	if (display == 0)
	{
		LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_3);
	} else if (display == 1)
	{
		//LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_X);
	}
	LL_mDelay(1);
}

static void transmit_end()
{
	// CS#
	LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_3);
	//LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_X);
}

static void transmit_word(uint8_t word)
{
	LL_SPI_TransmitData8(SPI2, word);
	while (!LL_SPI_IsActiveFlag_TXE(SPI2));
}

static void set_region(uint8_t display, uint8_t col_start, uint8_t col_end, uint8_t page_start, uint8_t page_end)
{
	transmit_start(display, 0);
	transmit_word(0x21);
	transmit_word(col_start);
	transmit_word(col_end);
	transmit_word(0x22);
	transmit_word(page_start);
	transmit_word(page_end);
	transmit_end();
}

void Driver_Display_Init()
{
	LL_SPI_Enable(SPI2);

	// Hardware reset all OLEDs on shared pin
	LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_0);
	LL_mDelay(1);
	LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_0);
	LL_mDelay(1);

	for (uint8_t i = 0; i < DISPLAY_COUNT; i++)
	{
		transmit_start(i, 0);

		// Vertical addressing mode
		transmit_word(0x20);
		transmit_word(0x01);

		// Flip horizontal and vertical
		transmit_word(0xA1);
		transmit_word(0xC8);

		// Maximum contrast
		transmit_word(0x81);
		transmit_word(0xFF);

		transmit_end();

		// Clear GDDRAM
		Driver_Display_Clear(i);

		// Display on
		transmit_start(i, 0);
		transmit_word(0xAF);
		transmit_end();
	}
}

void Driver_Display_Clear(uint8_t display)
{
	if (display >= DISPLAY_COUNT) return;

	set_region(display, 0, DISPLAY_COLUMNS - 1, 0, DISPLAY_PAGES - 1);

	transmit_start(display, 1);
	for (size_t i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT / 8; i++) transmit_word(0x00);
	transmit_end();
}
