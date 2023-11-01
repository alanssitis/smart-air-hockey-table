#include "app_core.h"

#include "stm32u5xx_ll_tim.h"
#include "stm32u5xx_ll_utils.h"

#include "app_statemachine.h"
#include "driver_led.h"
#include "driver_display.h"
#include "driver_encoder.h"
#include "driver_goal.h"
#include "driver_relay.h"
#include "driver_halleffect.h"

//static volatile uint_fast32_t ticks_elapsed; // volatile: TIM6_Handler has higher priority
static uint_fast32_t ticks_completed;
//static uint_fast32_t ticks_missed;color

void App_Init()
{
	// Configure SysTick to have 1 ms time based on 160 MHz, used by LL_mDelay
	LL_Init1msTick(160000000);

	Driver_LED_Init();
	Driver_Display_Init();
	Driver_Encoder_Init();

	// Last step: set initial state
	App_StateMachine_Init();

	// Start 1000 Hz "superloop"
//	LL_TIM_SetUpdateSource(TIM6, LL_TIM_UPDATESOURCE_COUNTER);
	LL_TIM_SetUpdateSource(TIM7, LL_TIM_UPDATESOURCE_COUNTER);
//	LL_TIM_EnableIT_UPDATE(TIM6);
	LL_TIM_EnableIT_UPDATE(TIM7);
//	LL_TIM_EnableCounter(TIM6);
	LL_TIM_EnableCounter(TIM7);
}

void TIM6_Handler()
{
//	ticks_elapsed++;
}

void TIM7_Handler()
{
//	if (ticks_completed + ticks_missed != ticks_elapsed)
//	{
//		uint_fast32_t ticks_difference = ticks_elapsed - ticks_completed - ticks_missed;
//		ticks_missed += ticks_difference;
//		// Doesn't do much yet, but we can detect when ticks are missed
//	}

//	App_StateMachine_GameTick();
//	Color three[3] = {{0x33, 0, 0}, {0, 0x33, 0}, {0, 0, 0x33}};
//	for (int j = 0; j < LED_MATRIX_COL_SIZE; j++) {
//		for (int i = 0; i < LED_MATRIX_ROW_SIZE; i++) {
//			Driver_LED_SetColor(i, j, three[(i + j) % 3]);
//		}
//	}
	static int i = 0;
	static int counter = 0;
	Color color = {0x30, 0x30, 0x30};
	Color blank = {0, 0, 0};

	if (--counter <= 0) {
		if (i > 0) {
			int j = i - 1;
			Driver_LED_SetColor(j % LED_MATRIX_ROW_SIZE, j / LED_MATRIX_ROW_SIZE, blank);
		} else {
			Driver_LED_SetColor(LED_MATRIX_ROW_SIZE - 1, 3, blank);
		}
		Driver_LED_SetColor(i % LED_MATRIX_ROW_SIZE, i / LED_MATRIX_ROW_SIZE, color);
		counter = 200;
		i++;
	}
	if (i >= 16 * 4) i = 0;
	Driver_LED_Tick();
	ticks_completed++;
}
