#include "driver_halleffect.h"

#include "stm32u575xx.h"

#define ROW0_IDR	GPIO_IDR_ID6	// GPIOA
#define ROW1_IDR	GPIO_IDR_ID7	// GPIOA
#define ROW2_IDR	GPIO_IDR_ID4	// GPIOC
#define ROW3_IDR	GPIO_IDR_ID5	// GPIOC
#define ROW4_IDR	GPIO_IDR_ID1	// GPIOB
#define ROW5_IDR	GPIO_IDR_ID2	// GPIOB
#define ROW6_IDR	GPIO_IDR_ID7	// GPIOE
#define ROW7_IDR	GPIO_IDR_ID8	// GPIOE
#define ROW8_IDR	GPIO_IDR_ID9	// GPIOE
#define ROW9_IDR	GPIO_IDR_ID10	// GPIOE
#define ROW10_IDR	GPIO_IDR_ID11	// GPIOE
#define ROW11_IDR	GPIO_IDR_ID12	// GPIOE
#define ROW12_IDR	GPIO_IDR_ID13	// GPIOE
#define ROW13_IDR	GPIO_IDR_ID14	// GPIOE
#define ROW14_IDR	GPIO_IDR_ID15	// GPIOE
#define ROW15_IDR	GPIO_IDR_ID10	// GPIOB

#define COL0_IDR	GPIO_IDR_ID2	// GPIOE
#define COL1_IDR	GPIO_IDR_ID1	// GPIOE
#define COL2_IDR	GPIO_IDR_ID0	// GPIOE
#define COL3_IDR	GPIO_IDR_ID9	// GPIOB
#define COL4_IDR	GPIO_IDR_ID8	// GPIOB
#define COL5_IDR	GPIO_IDR_ID7	// GPIOB
#define COL6_IDR	GPIO_IDR_ID6	// GPIOB
#define COL7_IDR	GPIO_IDR_ID5	// GPIOB
#define COL8_IDR	GPIO_IDR_ID4	// GPIOB
#define COL9_IDR	GPIO_IDR_ID3	// GPIOB
#define COL10_IDR	GPIO_IDR_ID7	// GPIOD
#define COL11_IDR	GPIO_IDR_ID6	// GPIOD
#define COL12_IDR	GPIO_IDR_ID5	// GPIOD
#define COL13_IDR	GPIO_IDR_ID4	// GPIOD
#define COL14_IDR	GPIO_IDR_ID3	// GPIOD
#define COL15_IDR	GPIO_IDR_ID2	// GPIOD
#define COL16_IDR	GPIO_IDR_ID1	// GPIOD
#define COL17_IDR	GPIO_IDR_ID0	// GPIOD
#define COL18_IDR	GPIO_IDR_ID12	// GPIOC
#define COL19_IDR	GPIO_IDR_ID11	// GPIOC
#define COL20_IDR	GPIO_IDR_ID10	// GPIOC
#define COL21_IDR	GPIO_IDR_ID15	// GPIOA
#define COL22_IDR	GPIO_IDR_ID12	// GPIOA
#define COL23_IDR	GPIO_IDR_ID11	// GPIOA
#define COL24_IDR	GPIO_IDR_ID10	// GPIOA
#define COL25_IDR	GPIO_IDR_ID9	// GPIOA
#define COL26_IDR	GPIO_IDR_ID8	// GPIOA
#define COL27_IDR	GPIO_IDR_ID9	// GPIOC
#define COL28_IDR	GPIO_IDR_ID8	// GPIOC
#define COL29_IDR	GPIO_IDR_ID7	// GPIOC
#define COL30_IDR	GPIO_IDR_ID6	// GPIOC
#define COL31_IDR	GPIO_IDR_ID15	// GPIOD

volatile uint16_t halleffect_rows;
volatile uint32_t halleffect_cols;

void Driver_HallEffect_PollInputs()
{
	halleffect_rows = 0xFFFF;
	halleffect_rows ^= (GPIOA->IDR & ROW0_IDR) >> 6;
	halleffect_rows ^= (GPIOA->IDR & ROW1_IDR) >> 6;
	halleffect_rows ^= (GPIOC->IDR & ROW2_IDR) >> 3;
	halleffect_rows ^= (GPIOC->IDR & ROW3_IDR) >> 2;
	halleffect_rows ^= (GPIOB->IDR & ROW4_IDR) << 3;
	halleffect_rows ^= (GPIOB->IDR & ROW5_IDR) << 2;
	halleffect_rows ^= (GPIOE->IDR & ROW6_IDR) >> 1;
	halleffect_rows ^= (GPIOE->IDR & ROW7_IDR) >> 1;
	halleffect_rows ^= (GPIOE->IDR & ROW8_IDR) >> 1;
	halleffect_rows ^= (GPIOE->IDR & ROW9_IDR) >> 1;
	halleffect_rows ^= (GPIOE->IDR & ROW10_IDR) >> 1;
	halleffect_rows ^= (GPIOE->IDR & ROW11_IDR) >> 1;
	halleffect_rows ^= (GPIOE->IDR & ROW12_IDR) >> 1;
	halleffect_rows ^= (GPIOE->IDR & ROW13_IDR) >> 1;
	halleffect_rows ^= (GPIOE->IDR & ROW14_IDR) >> 1;
	halleffect_rows ^= (GPIOB->IDR & ROW15_IDR) << 5;

	halleffect_cols = 0xFFFFFFFF;
	halleffect_cols ^= (GPIOE->IDR & COL0_IDR) >> 2;
	halleffect_cols ^= (GPIOE->IDR & COL1_IDR);
	halleffect_cols ^= (GPIOE->IDR & COL2_IDR) << 2;
	halleffect_cols ^= (GPIOB->IDR & COL3_IDR) >> 6;
	halleffect_cols ^= (GPIOB->IDR & COL4_IDR) >> 5;
	halleffect_cols ^= (GPIOB->IDR & COL5_IDR) >> 2;
	halleffect_cols ^= (GPIOB->IDR & COL6_IDR);
	halleffect_cols ^= (GPIOB->IDR & COL7_IDR) >> 2;
	halleffect_cols ^= (GPIOB->IDR & COL8_IDR) << 4;
	halleffect_cols ^= (GPIOD->IDR & COL9_IDR) << 6;
	halleffect_cols ^= (GPIOD->IDR & COL10_IDR) << 3;
	halleffect_cols ^= (GPIOD->IDR & COL11_IDR) << 5;
	halleffect_cols ^= (GPIOD->IDR & COL12_IDR) << 7;
	halleffect_cols ^= (GPIOD->IDR & COL13_IDR) << 9;
	halleffect_cols ^= (GPIOD->IDR & COL14_IDR) << 11;
	halleffect_cols ^= (GPIOD->IDR & COL15_IDR) << 13;
	halleffect_cols ^= (GPIOD->IDR & COL16_IDR) << 15;
	halleffect_cols ^= (GPIOD->IDR & COL17_IDR) << 17;
	halleffect_cols ^= (GPIOC->IDR & COL18_IDR) << 6;
	halleffect_cols ^= (GPIOC->IDR & COL19_IDR) << 8;
	halleffect_cols ^= (GPIOC->IDR & COL20_IDR) << 10;
	halleffect_cols ^= (GPIOA->IDR & COL21_IDR) << 6;
	halleffect_cols ^= (GPIOA->IDR & COL22_IDR) << 10;
	halleffect_cols ^= (GPIOA->IDR & COL23_IDR) << 12;
	halleffect_cols ^= (GPIOA->IDR & COL24_IDR) << 14;
	halleffect_cols ^= (GPIOA->IDR & COL25_IDR) << 16;
	halleffect_cols ^= (GPIOA->IDR & COL26_IDR) << 18;
	halleffect_cols ^= (GPIOC->IDR & COL27_IDR) << 18;
	halleffect_cols ^= (GPIOC->IDR & COL28_IDR) << 20;
	halleffect_cols ^= (GPIOC->IDR & COL29_IDR) << 22;
	halleffect_cols ^= (GPIOC->IDR & COL30_IDR) << 24;
	halleffect_cols ^= (GPIOD->IDR & COL31_IDR) << 16;
}
