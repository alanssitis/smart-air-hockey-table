#include "driver_hall_effect.h"

void Driver_Hall_Effect_ReadHalls()
{
	hall_effect_rows = 0;
	hall_effect_rows |= (GPIOA->ODR & ROW0_ODR) >> 6;
	hall_effect_rows |= (GPIOA->ODR & ROW1_ODR) >> 6;
	hall_effect_rows |= (GPIOC->ODR & ROW2_ODR) >> 3;
	hall_effect_rows |= (GPIOC->ODR & ROW3_ODR) >> 2;
	hall_effect_rows |= (GPIOB->ODR & ROW4_ODR) << 3;
	hall_effect_rows |= (GPIOB->ODR & ROW5_ODR) << 2;
	hall_effect_rows |= (GPIOE->ODR & ROW6_ODR) >> 1;
	hall_effect_rows |= (GPIOE->ODR & ROW7_ODR) >> 1;
	hall_effect_rows |= (GPIOE->ODR & ROW8_ODR) >> 1;
	hall_effect_rows |= (GPIOE->ODR & ROW9_ODR) >> 1;
	hall_effect_rows |= (GPIOE->ODR & ROW10_ODR) >> 1;
	hall_effect_rows |= (GPIOE->ODR & ROW11_ODR) >> 1;
	hall_effect_rows |= (GPIOE->ODR & ROW12_ODR) >> 1;
	hall_effect_rows |= (GPIOE->ODR & ROW13_ODR) >> 1;
	hall_effect_rows |= (GPIOE->ODR & ROW14_ODR) >> 1;
	hall_effect_rows |= (GPIOB->ODR & ROW15_ODR) << 5;

	hall_effect_cols = 0;
	hall_effect_cols |= (GPIOE->ODR & COL0_ODR) >> 2;
	hall_effect_cols |= (GPIOE->ODR & COL1_ODR);
	hall_effect_cols |= (GPIOE->ODR & COL2_ODR) << 2;
	hall_effect_cols |= (GPIOB->ODR & COL3_ODR) >> 6;
	hall_effect_cols |= (GPIOB->ODR & COL4_ODR) >> 5;
	hall_effect_cols |= (GPIOB->ODR & COL5_ODR) >> 2;
	hall_effect_cols |= (GPIOB->ODR & COL6_ODR);
	hall_effect_cols |= (GPIOB->ODR & COL7_ODR) >> 2;
	hall_effect_cols |= (GPIOB->ODR & COL8_ODR) << 4;
	hall_effect_cols |= (GPIOD->ODR & COL9_ODR) << 6;
	hall_effect_cols |= (GPIOD->ODR & COL10_ODR) << 3;
	hall_effect_cols |= (GPIOD->ODR & COL11_ODR) << 5;
	hall_effect_cols |= (GPIOD->ODR & COL12_ODR) << 7;
	hall_effect_cols |= (GPIOD->ODR & COL13_ODR) << 9;
	hall_effect_cols |= (GPIOD->ODR & COL14_ODR) << 11;
	hall_effect_cols |= (GPIOD->ODR & COL15_ODR) << 13;
	hall_effect_cols |= (GPIOD->ODR & COL16_ODR) << 15;
	hall_effect_cols |= (GPIOD->ODR & COL17_ODR) << 17;
	hall_effect_cols |= (GPIOC->ODR & COL18_ODR) << 6;
	hall_effect_cols |= (GPIOC->ODR & COL19_ODR) << 8;
	hall_effect_cols |= (GPIOC->ODR & COL20_ODR) << 10;
	hall_effect_cols |= (GPIOA->ODR & COL21_ODR) << 6;
	hall_effect_cols |= (GPIOA->ODR & COL22_ODR) << 10;
	hall_effect_cols |= (GPIOA->ODR & COL23_ODR) << 12;
	hall_effect_cols |= (GPIOA->ODR & COL24_ODR) << 14;
	hall_effect_cols |= (GPIOA->ODR & COL25_ODR) << 16;
	hall_effect_cols |= (GPIOA->ODR & COL26_ODR) << 18;
	hall_effect_cols |= (GPIOC->ODR & COL27_ODR) << 18;
	hall_effect_cols |= (GPIOC->ODR & COL28_ODR) << 20;
	hall_effect_cols |= (GPIOC->ODR & COL29_ODR) << 22;
	hall_effect_cols |= (GPIOC->ODR & COL30_ODR) << 24;
	hall_effect_cols |= (GPIOD->ODR & COL31_ODR) << 16;
}
