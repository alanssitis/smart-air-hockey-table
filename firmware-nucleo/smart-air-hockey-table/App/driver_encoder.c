#include "driver_encoder.h"

#include "stm32u5xx_ll_tim.h"

#define ENCODER_SCALING_SHIFT 2

static bool is_button_pressed; // non-volatile: EXTI5_Handler has lower priority
static uint_fast32_t rotation_basis;

void Driver_Encoder_Init()
{
	LL_TIM_EnableCounter(TIM3);
	is_button_pressed = false; // Clear the flag since it is triggered at startup
}

bool Driver_Encoder_PollButton()
{
	bool result = is_button_pressed;
	is_button_pressed = false;
	return result;
}

int_fast8_t Driver_Encoder_PollRotation()
{
	int_fast8_t rotation_delta = LL_TIM_GetCounter(TIM3) - rotation_basis;

	// Only detect rotation on a multiple of ENCODER_SCALING (2 ^ ENCODER_SCALING_SHIFT)
	int_fast8_t result = rotation_delta >> ENCODER_SCALING_SHIFT;
	rotation_basis += result << ENCODER_SCALING_SHIFT;
	return result;
}

void Driver_Encoder_SetActive(bool active)
{
	if (active)
	{
		NVIC_EnableIRQ(EXTI5_IRQn);
		LL_TIM_EnableCounter(TIM3);
	}
	else
	{
		NVIC_DisableIRQ(EXTI5_IRQn);
		LL_TIM_DisableCounter(TIM3);
	}
}

void EXTI5_Handler()
{
	is_button_pressed = true;
}
