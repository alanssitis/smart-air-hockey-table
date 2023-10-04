#include "driver_display.h"

#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_spi.h"

#include "font_6x8.h"
#include "font_score.h"

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
	if (display & 0x01) LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_3);
	//if (display & 0x02) LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_X);
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

	// Use bitmask to send to all displays simultaneously
	transmit_start(0xFF, 0);

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
	transmit_start(0xFF, 0);
	transmit_word(0xAF);
	transmit_end();
}

void Driver_Display_Clear(uint8_t display)
{
	set_region(display, 0, DISPLAY_COLUMNS - 1, 0, DISPLAY_PAGES - 1);

	transmit_start(display, 1);
	for (size_t i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT / 8; i++) transmit_word(0x00);
	transmit_end();
}

void Driver_Display_Print(uint8_t display, uint8_t line, uint8_t offset, const char* format, ...)
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
	uint8_t inverted = 0, column = 0;
	set_region(display, offset * FONT_6X8_WIDTH, DISPLAY_COLUMNS - 1, line, line);
	transmit_start(display, 1);
	while (*string != '\0')
	{
		uint8_t character = *(string++);
		if (character == '\t') {
			inverted = !inverted;
			continue;
		}
		if (character < FONT_6X8_MIN || character > FONT_6X8_MAX){
			continue;
		}

		const uint8_t* character_data = font_6x8[character - FONT_6X8_MIN];
		for (uint8_t col = 0; col < FONT_6X8_WIDTH; col++)
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

void Driver_Display_ShowScore(uint8_t display, uint8_t score_a, uint8_t score_b)
{
	if (score_a > 99 || score_b > 99) return;

	uint8_t tens_a = (score_a / 10) % 10;
	uint8_t ones_a = score_a % 10;
	uint8_t tens_b = (score_b / 10) % 10;
	uint8_t ones_b = score_b % 10;

	set_region(display, (DISPLAY_COLUMNS - FONT_SCORE_WIDTH * 5) / 2, DISPLAY_COLUMNS - 1, 3, 6);

	transmit_start(display, 1);
	for (size_t i = 0; i < sizeof(font_score[tens_a]); i++) transmit_word(font_score[tens_a][i]);
	for (size_t i = 0; i < sizeof(font_score[ones_a]); i++) transmit_word(font_score[ones_a][i]);
	for (size_t i = 0; i < sizeof(font_score[10]); i++) transmit_word(font_score[10][i]);
	for (size_t i = 0; i < sizeof(font_score[tens_b]); i++) transmit_word(font_score[tens_b][i]);
	for (size_t i = 0; i < sizeof(font_score[ones_b]); i++) transmit_word(font_score[ones_b][i]);
	transmit_end();

	Driver_Display_Print(display, 0, 0, "\t     Goals Scored     ");
}

void Driver_Display_FontTest(uint8_t display)
{
	Driver_Display_Clear(display);
	Driver_Display_Print(0, 0, 0, " !\"#$%%&'()*+,-./");
	Driver_Display_Print(0, 1, 0, "0123456789:;<=>?");
	Driver_Display_Print(0, 2, 0, "@ABCDEFGHIJKLMNO");
	Driver_Display_Print(0, 3, 0, "PQRSTUVWXYZ[\\]^_");
	Driver_Display_Print(0, 4, 0, "`abcdefghijklmno");
	Driver_Display_Print(0, 5, 0, "pqrstuvwxyz{|}~ ");
}
