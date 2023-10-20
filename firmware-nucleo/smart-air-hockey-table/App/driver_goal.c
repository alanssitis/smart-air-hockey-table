#include "driver_goal.h"

#include "stm32u575xx.h"

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

bool Driver_Goal_Poll(GoalSensor goalSensor)
{
	switch (goalSensor)
	{
		case LDR1IN:
		{
			return (LDR1IN_GPIO->IDR & LDR1IN_IDR);
			break;
		}
		case LDR2IN:
		{
			return (LDR2IN_GPIO->IDR & LDR2IN_IDR);
			break;
		}
		case LDR3IN:
		{
			return (LDR3IN_GPIO->IDR & LDR3IN_IDR);
			break;
		}
		case LDR4IN:
		{
			return (LDR4IN_GPIO->IDR & LDR4IN_IDR);
			break;
		}
	}

	// If this return is hit, then goalSensor is invalid
	return false;
}
