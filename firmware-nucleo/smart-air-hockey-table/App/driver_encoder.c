#include "driver_encoder.h"

#include "stm32u5xx_ll_gpio.h"
#include "stm32u5xx_ll_tim.h"

#include "app_core.h"

#define BUTTON_DELAY_TICKS 10
#define BUTTON_DELAY_MASK (BIT_MASK(uint32_t, BUTTON_DELAY_TICKS))
#define ENCODER_SCALING 4

static volatile uint32_t button_in;
static volatile bool is_button_down;
static volatile bool is_button_down_prev;
static volatile bool is_button_pressed;
static volatile uint32_t rotation_basis;

void Driver_Encoder_Init()
{
	LL_TIM_EnableCounter(TIM3);
}

void Driver_Encoder_Tick()
{
	// Shift register, result is true if lowest bits are cleared
	button_in <<= 1;
	button_in |= LL_GPIO_IsInputPinSet(GPIOE, LL_GPIO_PIN_5);
	is_button_down = !(button_in & BUTTON_DELAY_MASK);

	// The button is pressed if it is down, when it wasn't before
	if (is_button_down && !is_button_down_prev) is_button_pressed = true;
	is_button_down_prev = is_button_down;
}

bool Driver_Encoder_PollButton()
{
	// The state persists until polled
	bool result = is_button_pressed;
	is_button_pressed = false;
	return result;
}

int8_t Driver_Encoder_PollRotation()
{
	int8_t rotation_delta = LL_TIM_GetCounter(TIM3) - rotation_basis;

	// Only detect rotation on a multiple of ENCODER_SCALING (on the detent)
	int8_t result = rotation_delta / ENCODER_SCALING;
	rotation_basis += result * ENCODER_SCALING;
	return result;
}
