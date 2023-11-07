#include "app_core.h"

#include <stdbool.h>
#include "stm32u5xx_ll_tim.h"
#include "stm32u5xx_ll_gpio.h"
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
//static uint_fast32_t ticks_missed;

void App_Init()
{
	// Configure SysTick to have 1 ms time based on 160 MHz, used by LL_mDelay
	LL_Init1msTick(160000000);

	Driver_LED_Init();
	Driver_Display_Init();
	Driver_Encoder_Init();

	// Last step: set initial state
	//App_StateMachine_Init();

	Driver_Display_Print(DISPLAY_ALL, 0, 0, "\tSmart Air Hockey Table");

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

	//App_StateMachine_GameTick();

	static uint_fast32_t rotation;
	static bool is_pressed;

	if (Driver_Encoder_PollButton())
	{
		is_pressed = !is_pressed;
		if (is_pressed) Driver_Encoder_PollRotation();
	}

	LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_2 | LL_GPIO_PIN_3);
	if (is_pressed) {
		rotation += Driver_Encoder_PollRotation();
		LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_0 << (rotation % 4));
	}

	static uint_fast32_t score_a, score_b;
	if (ticks_completed % 500 == 0) score_a++;
	if (ticks_completed % 900 == 0) score_b++;
	if (ticks_completed % 500 == 0 || ticks_completed % 900 == 0)
	{
		Driver_Display_ShowScore(DISPLAY_0, score_a % 100, score_b % 100);
		Driver_Display_ShowScore(DISPLAY_1, score_b % 100, score_a % 100);
	}

	Driver_HallEffect_PollInputs();

	if ((halleffect_cols & 0b01) && (halleffect_rows & 0b01)) Driver_LED_SetColor(0, 0, (Color) { 0x77, 0x77, 0x77 });
	else Driver_LED_SetColor(0, 0, (Color) { 0x00, 0x00, 0x00 });
	if ((halleffect_cols & 0b01) && (halleffect_rows & 0b10)) Driver_LED_SetColor(0, 1, (Color) { 0x77, 0x77, 0x77 });
	else Driver_LED_SetColor(0, 1, (Color) { 0x00, 0x00, 0x00 });
	if ((halleffect_cols & 0b10) && (halleffect_rows & 0b01)) Driver_LED_SetColor(1, 0, (Color) { 0x77, 0x77, 0x77 });
	else Driver_LED_SetColor(1, 0, (Color) { 0x00, 0x00, 0x00 });
	if ((halleffect_cols & 0b10) && (halleffect_rows & 0b10)) Driver_LED_SetColor(1, 1, (Color) { 0x77, 0x77, 0x77 });
	else Driver_LED_SetColor(1, 1, (Color) { 0x00, 0x00, 0x00 });

	Driver_LED_Tick();

	ticks_completed++;
}
