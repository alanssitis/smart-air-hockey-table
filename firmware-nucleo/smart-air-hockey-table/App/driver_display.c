#include "driver_display.h"

#include <stdarg.h>
#include <stdio.h>
#include "stm32u5xx_ll_gpio.h"
#include "stm32u5xx_ll_utils.h"
#include "stm32u5xx_ll_spi.h"

#include "font_6x8.h"
#include "font_score.h"

#define DISPLAY_COLUMNS 128
#define DISPLAY_PAGES 8
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

static void transmit_start(Display display, bool is_data)
{
	// DC
	if (is_data) LL_GPIO_SetOutputPin(GPIOD, LL_GPIO_PIN_12);
	else LL_GPIO_ResetOutputPin(GPIOD, LL_GPIO_PIN_12);

	// CS#
	if (display & DISPLAY_0) LL_GPIO_ResetOutputPin(GPIOD, LL_GPIO_PIN_10);
	if (display & DISPLAY_1) LL_GPIO_ResetOutputPin(GPIOD, LL_GPIO_PIN_11);
}

static void transmit_end()
{
	// CS#
	LL_GPIO_SetOutputPin(GPIOD, LL_GPIO_PIN_10 | LL_GPIO_PIN_11);
}

static void transmit_word(uint8_t word)
{
	LL_SPI_TransmitData8(SPI2, word);
	while (!LL_SPI_IsActiveFlag_TXC(SPI2));
}

static void set_region(Display display, uint_fast8_t col_start, uint_fast8_t col_end, uint_fast8_t page_start, uint_fast8_t page_end)
{
	transmit_start(display, false);
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
	LL_SPI_StartMasterTransfer(SPI2);

	// Hardware reset all OLEDs on shared pin
	LL_GPIO_ResetOutputPin(GPIOD, LL_GPIO_PIN_13);
	LL_mDelay(1);
	LL_GPIO_SetOutputPin(GPIOD, LL_GPIO_PIN_13);
	LL_mDelay(1);

	// Use bitmask to send to all displays simultaneously
	transmit_start(DISPLAY_ALL, false);

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

	Driver_Display_Clear(DISPLAY_ALL);
	Driver_Display_SetPowered(DISPLAY_ALL, true);
}

void Driver_Display_Clear(Display display)
{
	set_region(display, 0, DISPLAY_COLUMNS - 1, 0, DISPLAY_PAGES - 1);

	transmit_start(display, true);
	for (uint_fast32_t i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT / 8; i++) transmit_word(0x00);
	transmit_end();
}

void Driver_Display_SetPowered(Display display, bool is_powered)
{
	// Controls power to the OLED panel, the driver IC is always on
	transmit_start(display, false);
	transmit_word(is_powered ? 0xAF : 0xAE);
	transmit_end();
}

void Driver_Display_Print(Display display, uint_fast8_t line, uint_fast8_t offset, const char* format, ...)
{
	if (line >= DISPLAY_PAGES) return;
	if (offset * FONT_6X8_WIDTH >= DISPLAY_COLUMNS) return;

	// Write format string to buffer
	char buffer[2 * DISPLAY_COLUMNS / FONT_6X8_WIDTH]; // Extra space for escape codes
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, sizeof(buffer), format, args);
	va_end(args);

	char* string = &buffer[0];

	// Send string to the display
	bool inverted = 0;
	uint_fast8_t column = 0;
	set_region(display, offset * FONT_6X8_WIDTH, DISPLAY_COLUMNS - 1, line, line);
	transmit_start(display, true);
	while (*string != '\0')
	{
		uint8_t character = *(string++);
		if (character == '\t')
		{
			inverted = !inverted;
			continue;
		}
		if (character < FONT_6X8_MIN || character > FONT_6X8_MAX) character = ' ';
		const uint8_t* character_data = font_6x8[character - FONT_6X8_MIN];
		for (uint_fast8_t col = 0; col < FONT_6X8_WIDTH; col++)
		{
			uint8_t column_data = character_data[col];
			if (inverted) column_data = ~column_data;
			transmit_word(column_data);

			// Stop if we run out of room
			if (++column == DISPLAY_COLUMNS) break;
		}
	}
	transmit_end();
}

void Driver_Display_ShowScore(Display display, uint_fast8_t score_a, uint_fast8_t score_b)
{
	if (score_a > 99 || score_b > 99) return;

	uint_fast8_t tens_a = (score_a / 10);
	uint_fast8_t ones_a = score_a - tens_a * 10;
	uint_fast8_t tens_b = (score_b / 10);
	uint_fast8_t ones_b = score_b - tens_b * 10;

	set_region(display, (DISPLAY_COLUMNS - FONT_SCORE_WIDTH * 5) / 2, DISPLAY_COLUMNS - 1, 3, 6);

	transmit_start(display, true);
	for (uint_fast32_t i = 0; i < sizeof(font_score[tens_a]); i++) transmit_word(font_score[tens_a][i]);
	for (uint_fast32_t i = 0; i < sizeof(font_score[ones_a]); i++) transmit_word(font_score[ones_a][i]);
	for (uint_fast32_t i = 0; i < sizeof(font_dash); i++) transmit_word(font_dash[i]);
	for (uint_fast32_t i = 0; i < sizeof(font_score[tens_b]); i++) transmit_word(font_score[tens_b][i]);
	for (uint_fast32_t i = 0; i < sizeof(font_score[ones_b]); i++) transmit_word(font_score[ones_b][i]);
	transmit_end();
}

void Driver_Display_FontTest(Display display)
{
	Driver_Display_Print(display, 0, 0, " !\"#$%%&'()*+,-./");
	Driver_Display_Print(display, 1, 0, "0123456789:;<=>?");
	Driver_Display_Print(display, 2, 0, "@ABCDEFGHIJKLMNO");
	Driver_Display_Print(display, 3, 0, "PQRSTUVWXYZ[\\]^_");
	Driver_Display_Print(display, 4, 0, "`abcdefghijklmno");
	Driver_Display_Print(display, 5, 0, "pqrstuvwxyz{|}~ ");
}
