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
static uint_fast32_t ticks_completed = 0;
//static uint_fast32_t ticks_missed;

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

	App_StateMachine_GameTick();
//	Driver_HallEffect_PollInputs();

	int col_pos = 0, row_pos = 0;
	int num_col_on = __builtin_popcount(halleffect_cols);
	int num_row_on = __builtin_popcount(halleffect_rows);

	if (num_col_on > 0 && num_row_on > 0) {
		for (int i = 0; i < LED_MATRIX_COL_NUM; i++) {
			if (halleffect_cols & 1 << i) {
				col_pos += i;
			}
			if (i < LED_MATRIX_ROW_NUM && halleffect_rows & 1 << i) {
				row_pos += i;
			}

		}
		col_pos /= num_col_on;
		row_pos /= num_row_on;
	} else {
		col_pos = LED_MATRIX_COL_NUM + 2;
		row_pos = LED_MATRIX_ROW_NUM + 2;
	}

	for (uint_fast8_t col = 0; col < LED_MATRIX_COL_NUM; col++)
	{
		for (uint_fast8_t row = 0; row < LED_MATRIX_ROW_NUM; row++)
		{
			if (col_pos == col && row_pos == row) {
				Driver_LED_SetColor(col, row, (Color) { 0xff, 0, 0 });
			} else if (abs(col - col_pos) <= 1 && abs(row - row_pos) <= 1) {
				Driver_LED_SetColor(col, row, (Color) { 0, 0xff, 0xff });
			} else {
				Driver_LED_SetColor(col, row, (Color) { 0, 0, 0 });
			}
		}
	}

	Driver_LED_Tick();

	ticks_completed++;
}
