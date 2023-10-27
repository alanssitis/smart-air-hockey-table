#include "driver_goal.h"

#include "stm32u575xx.h"
#include "app_core.h"

// Goal Sensor LDR1IN
#define LDR1IN_GPIO	GPIOE
#define LDR1IN_IDR	GPIO_IDR_ID6

// Goal Sensor LDR2IN
#define LDR2IN_GPIO	GPIOF	// GPIOC for master PCB
#define LDR2IN_IDR	GPIO_IDR_ID13

// Goal Sensor LDR3IN
#define LDR3IN_GPIO	GPIOF	// GPIOC for master PCB
#define LDR3IN_IDR	GPIO_IDR_ID14

// Goal Sensor LDR4IN
#define LDR4IN_GPIO	GPIOF	// GPIOC for master PCB
#define LDR4IN_IDR	GPIO_IDR_ID15

#define GOAL_DELAY_TICKS	10											// TIME IN TICKS
#define GOAL_DELAY_MASK		(BIT_MASK(uint_fast32_t, GOAL_DELAY_TICKS))	// BITMASK

static uint_fast32_t ldr1_in;
static uint_fast32_t ldr2_in;
static uint_fast32_t ldr3_in;
static uint_fast32_t ldr4_in;

bool ldr1_goal;
bool ldr2_goal;
bool ldr3_goal;
bool ldr4_goal;

void Driver_Goal_Poll()
{
	// TODO: Potential optimization in combining both inputs
	// Meaning we would only need to track two registers instead of one for debouncing
	ldr1_in <<= 1;
	ldr2_in <<= 1;
	ldr3_in <<= 1;
	ldr4_in <<= 1;

	// need to put the IDR value into the rightmost value for the LDR
	// after we shifted by 1 to left, we AND the IDR to isolate the LDR bit
	// we then shift to the right by the bit position
	//
	// for example, LDR1 is on pin 6, so its bit is position 6
	// thus, we shift to the right by 6 to place it in bit position 0
	ldr1_in |= (LDR1IN_GPIO->IDR & LDR1IN_IDR) >> 6;
	ldr2_in |= (LDR2IN_GPIO->IDR & LDR2IN_IDR) >> 13;
	ldr3_in |= (LDR3IN_GPIO->IDR & LDR3IN_IDR) >> 14;
	ldr4_in |= (LDR4IN_GPIO->IDR & LDR4IN_IDR) >> 15;

	// result is true if ldrx_in has any of the lowest GOAL_DELAY_TICKS bits set
	ldr1_goal = !(ldr1_in & GOAL_DELAY_MASK);
	ldr2_goal = !(ldr2_in & GOAL_DELAY_MASK);
	ldr3_goal = !(ldr3_in & GOAL_DELAY_MASK);
	ldr4_goal = !(ldr4_in & GOAL_DELAY_MASK);
}
