#include "driver_encoder.h"

#include "stm32u5xx_ll_tim.h"

#define ENCODER_SCALING 4

static volatile uint8_t is_button_pressed;
static volatile uint32_t rotation_basis;
static volatile int8_t rotation_delta;

void Driver_Encoder_Init()
{
	LL_TIM_EnableCounter(TIM3);
	is_button_pressed = 0; // Clear the flag since it is triggered at startup
}

void Driver_Encoder_Tick()
{
	rotation_delta = LL_TIM_GetCounter(TIM3) - rotation_basis;
}

uint8_t Driver_Encoder_PollButton()
{
	uint8_t result = is_button_pressed;
	is_button_pressed = 0;
	return result;
}

int8_t Driver_Encoder_PollRotation()
{
	int8_t result = rotation_delta % ENCODER_SCALING == 0 ? rotation_delta / ENCODER_SCALING : 0;
	if (result != 0) rotation_basis += rotation_delta;
	return result;
}

void Driver_Encoder_SetActive(uint8_t active)
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
	is_button_pressed = 1;
}
