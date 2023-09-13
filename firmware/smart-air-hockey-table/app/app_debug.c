#include <stdio.h>
#include <stdarg.h>
#include <inttypes.h>
#include "app_debug.h"
#include "stm32f4xx_ll_tim.h"

static volatile uint32_t time_ms;

void Debug_Init()
{
	// Start TIM4
	LL_TIM_EnableIT_UPDATE(TIM4);
	LL_TIM_SetUpdateSource(TIM4, LL_TIM_UPDATESOURCE_COUNTER);
	LL_TIM_EnableCounter(TIM4);

	// Clear the terminal
	printf("\033c");
	printf("\033[3J");
}

void Debug_Log(const char* format, ...)
{
	va_list args;
	printf("[%4"PRIu32".%03"PRIu32"] ", time_ms / 1000, time_ms % 1000);
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
	printf("\r\n");
}

void TIM4_Handler()
{
	time_ms++;
}
