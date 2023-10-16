#include "app_core.h"

#include "stm32u5xx_ll_tim.h"
#include "stm32u5xx_ll_utils.h"

#include "driver_led.h"
#include "driver_display.h"
#include "driver_encoder.h"

static volatile uint32_t ticks_elapsed;
static volatile uint32_t ticks_completed;
static volatile uint32_t ticks_missed;

void App_Init()
{
	// Configure SysTick to have 1 ms time based on 160 MHz, used by LL_mDelay
	LL_Init1msTick(160000000);

	Driver_LED_Init();
	Driver_Display_Init();
	Driver_Encoder_Init();

	// Start 1000 Hz "superloop"
	LL_TIM_SetUpdateSource(TIM6, LL_TIM_UPDATESOURCE_COUNTER);
	LL_TIM_SetUpdateSource(TIM7, LL_TIM_UPDATESOURCE_COUNTER);
	LL_TIM_EnableIT_UPDATE(TIM6);
	LL_TIM_EnableIT_UPDATE(TIM7);
	LL_TIM_EnableCounter(TIM6);
	LL_TIM_EnableCounter(TIM7);
}

void TIM6_Handler()
{
	ticks_elapsed++;
}

void TIM7_Handler()
{
	if (ticks_completed + ticks_missed != ticks_elapsed)
	{
		uint32_t ticks_difference = ticks_elapsed - ticks_completed - ticks_missed;
		ticks_missed += ticks_difference;
		// Doesn't do much yet, but we can detect when ticks are missed
	}

	// TODO: State machine logic

	static int8_t selection;
	static int8_t isDone;
	if (!isDone)
	{
		selection += Driver_Encoder_PollRotation();
		while (selection < 0) selection += 4;
		while (selection > 3) selection -= 4;
		Driver_Display_Print(0xFF, 0, 0, "%sStart Game", selection == 0 ? "> \t" : "  ");
		Driver_Display_Print(0xFF, 1, 0, "%sSettings", selection == 1 ? "> \t" : "  ");
		Driver_Display_Print(0xFF, 2, 0, "%sCredits", selection == 2 ? "> \t" : "  ");
		Driver_Display_Print(0xFF, 3, 0, "%sPower Off", selection == 3 ? "> \t" : "  ");
		if (Driver_Encoder_PollButton())
		{
			Driver_Display_Clear(0xFF);
			isDone = 1;
		}
	}
	else
	{
		Driver_Display_Print(0xFF, 0, 0, "Selection: %10"PRIi32, (int32_t) selection);
	}

	Driver_Display_Print(0xFF, 7, 0, "Missed: %10"PRIu32, ticks_missed);

	Driver_LED_Tick();
	Driver_Encoder_Tick();

	ticks_completed++;
}

uint32_t App_GetTicksElapsed()
{
	return ticks_elapsed;
}
