#include "driver_hall_effect.h"

uint32_t hall_effect_cols;
uint16_t hall_effect_rows;

void Driver_Hall_Effect_ReadHalls()
{
	hall_effect_rows = 0xFFFF;
	hall_effect_rows ^= (GPIOA->IDR & ROW0_IDR) >> 6;
	hall_effect_rows ^= (GPIOA->IDR & ROW1_IDR) >> 6;
	hall_effect_rows ^= (GPIOC->IDR & ROW2_IDR) >> 3;
	hall_effect_rows ^= (GPIOC->IDR & ROW3_IDR) >> 2;
	hall_effect_rows ^= (GPIOB->IDR & ROW4_IDR) << 3;
	hall_effect_rows ^= (GPIOB->IDR & ROW5_IDR) << 2;
	hall_effect_rows ^= (GPIOE->IDR & ROW6_IDR) >> 1;
	hall_effect_rows ^= (GPIOE->IDR & ROW7_IDR) >> 1;
	hall_effect_rows ^= (GPIOE->IDR & ROW8_IDR) >> 1;
	hall_effect_rows ^= (GPIOE->IDR & ROW9_IDR) >> 1;
	hall_effect_rows ^= (GPIOE->IDR & ROW10_IDR) >> 1;
	hall_effect_rows ^= (GPIOE->IDR & ROW11_IDR) >> 1;
	hall_effect_rows ^= (GPIOE->IDR & ROW12_IDR) >> 1;
	hall_effect_rows ^= (GPIOE->IDR & ROW13_IDR) >> 1;
	hall_effect_rows ^= (GPIOE->IDR & ROW14_IDR) >> 1;
	hall_effect_rows ^= (GPIOB->IDR & ROW15_IDR) << 5;

	hall_effect_cols = 0xFFFFFFFF;
	hall_effect_cols ^= (GPIOE->IDR & COL0_IDR) >> 2;
	hall_effect_cols ^= (GPIOE->IDR & COL1_IDR);
	hall_effect_cols ^= (GPIOE->IDR & COL2_IDR) << 2;
	hall_effect_cols ^= (GPIOB->IDR & COL3_IDR) >> 6;
	hall_effect_cols ^= (GPIOB->IDR & COL4_IDR) >> 5;
	hall_effect_cols ^= (GPIOB->IDR & COL5_IDR) >> 2;
	hall_effect_cols ^= (GPIOB->IDR & COL6_IDR);
	hall_effect_cols ^= (GPIOB->IDR & COL7_IDR) >> 2;
	hall_effect_cols ^= (GPIOB->IDR & COL8_IDR) << 4;
	hall_effect_cols ^= (GPIOD->IDR & COL9_IDR) << 6;
	hall_effect_cols ^= (GPIOD->IDR & COL10_IDR) << 3;
	hall_effect_cols ^= (GPIOD->IDR & COL11_IDR) << 5;
	hall_effect_cols ^= (GPIOD->IDR & COL12_IDR) << 7;
	hall_effect_cols ^= (GPIOD->IDR & COL13_IDR) << 9;
	hall_effect_cols ^= (GPIOD->IDR & COL14_IDR) << 11;
	hall_effect_cols ^= (GPIOD->IDR & COL15_IDR) << 13;
	hall_effect_cols ^= (GPIOD->IDR & COL16_IDR) << 15;
	hall_effect_cols ^= (GPIOD->IDR & COL17_IDR) << 17;
	hall_effect_cols ^= (GPIOC->IDR & COL18_IDR) << 6;
	hall_effect_cols ^= (GPIOC->IDR & COL19_IDR) << 8;
	hall_effect_cols ^= (GPIOC->IDR & COL20_IDR) << 10;
	hall_effect_cols ^= (GPIOA->IDR & COL21_IDR) << 6;
	hall_effect_cols ^= (GPIOA->IDR & COL22_IDR) << 10;
	hall_effect_cols ^= (GPIOA->IDR & COL23_IDR) << 12;
	hall_effect_cols ^= (GPIOA->IDR & COL24_IDR) << 14;
	hall_effect_cols ^= (GPIOA->IDR & COL25_IDR) << 16;
	hall_effect_cols ^= (GPIOA->IDR & COL26_IDR) << 18;
	hall_effect_cols ^= (GPIOC->IDR & COL27_IDR) << 18;
	hall_effect_cols ^= (GPIOC->IDR & COL28_IDR) << 20;
	hall_effect_cols ^= (GPIOC->IDR & COL29_IDR) << 22;
	hall_effect_cols ^= (GPIOC->IDR & COL30_IDR) << 24;
	hall_effect_cols ^= (GPIOD->IDR & COL31_IDR) << 16;
}
