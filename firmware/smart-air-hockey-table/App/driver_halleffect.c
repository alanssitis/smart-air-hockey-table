#include "driver_halleffect.h"

#include "stm32u575xx.h"

// ROW0 PA6
#define ROW0_GPIO	GPIOA
#define ROW0_IDR	GPIO_IDR_ID6
// ROW1 PA7
#define ROW1_GPIO	GPIOA
#define ROW1_IDR	GPIO_IDR_ID7
// ROW2 PC4
#define ROW2_GPIO	GPIOC
#define ROW2_IDR	GPIO_IDR_ID4
// ROW3 PC5
#define ROW3_GPIO	GPIOC
#define ROW3_IDR	GPIO_IDR_ID5
// ROW4 PC6
#define ROW4_GPIO	GPIOB
#define ROW4_IDR	GPIO_IDR_ID1
// ROW5 PB2
#define ROW5_GPIO	GPIOB
#define ROW5_IDR	GPIO_IDR_ID2
// ROW6 PE7
#define ROW6_GPIO	GPIOE
#define ROW6_IDR	GPIO_IDR_ID7
// ROW7 PE8
#define ROW7_GPIO	GPIOE
#define ROW7_IDR	GPIO_IDR_ID8
// ROW8 PE9
#define ROW8_GPIO	GPIOE
#define ROW8_IDR	GPIO_IDR_ID9
// ROW9 PE10
#define ROW9_GPIO	GPIOE
#define ROW9_IDR	GPIO_IDR_ID10
// ROW10 PE11
#define ROW10_GPIO	GPIOE
#define ROW10_IDR	GPIO_IDR_ID11
// ROW11 PE12
#define ROW11_GPIO	GPIOE
#define ROW11_IDR	GPIO_IDR_ID12
// ROW12 PE13
#define ROW12_GPIO	GPIOE
#define ROW12_IDR	GPIO_IDR_ID13
// ROW13 PE14
#define ROW13_GPIO	GPIOE
#define ROW13_IDR	GPIO_IDR_ID14
// ROW14 PE15
#define ROW14_GPIO	GPIOE
#define ROW14_IDR	GPIO_IDR_ID15
// ROW15 PB10
#define ROW15_GPIO	GPIOB
#define ROW15_IDR	GPIO_IDR_ID10

// COL0 PE2
#define COL0_GPIO	GPIOE
#define COL0_IDR	GPIO_IDR_ID2
// COL1 PE1
#define COL1_GPIO	GPIOE
#define COL1_IDR	GPIO_IDR_ID1
// COL2 PE0
#define COL2_GPIO	GPIOE
#define COL2_IDR	GPIO_IDR_ID0
// COL3 PB9
#define COL3_GPIO	GPIOB
#define COL3_IDR	GPIO_IDR_ID9
// COL4 PB8
#define COL4_GPIO	GPIOB
#define COL4_IDR	GPIO_IDR_ID8
// COL5 PB7
#define COL5_GPIO	GPIOB
#define COL5_IDR	GPIO_IDR_ID7
// COL6 PB6
#define COL6_GPIO	GPIOB
#define COL6_IDR	GPIO_IDR_ID6
// COL7 PB5
#define COL7_GPIO	GPIOB
#define COL7_IDR	GPIO_IDR_ID5
// COL8 PB4
#define COL8_GPIO	GPIOB
#define COL8_IDR	GPIO_IDR_ID4
// COL9 PB3
#define COL9_GPIO	GPIOB
#define COL9_IDR	GPIO_IDR_ID3
// COL10 PD7
#define COL10_GPIO	GPIOD
#define COL10_IDR	GPIO_IDR_ID7
// COL11 PD6
#define COL11_GPIO	GPIOD
#define COL11_IDR	GPIO_IDR_ID6
// COL12 PD5
#define COL12_GPIO	GPIOD
#define COL12_IDR	GPIO_IDR_ID5
// COL13 PD4
#define COL13_GPIO	GPIOD
#define COL13_IDR	GPIO_IDR_ID4
// COL14 PD3
#define COL14_GPIO	GPIOD
#define COL14_IDR	GPIO_IDR_ID3
// COL15 PD2
#define COL15_GPIO	GPIOD
#define COL15_IDR	GPIO_IDR_ID2
// COL16 PD1
#define COL16_GPIO	GPIOD
#define COL16_IDR	GPIO_IDR_ID1
// COL17 PD0
#define COL17_GPIO	GPIOD
#define COL17_IDR	GPIO_IDR_ID0
// COL18 PC12
#define COL18_GPIO	GPIOC
#define COL18_IDR	GPIO_IDR_ID12
// COL19 PC11
#define COL19_GPIO	GPIOC
#define COL19_IDR	GPIO_IDR_ID11
// COL20 PC10
#define COL20_GPIO	GPIOC
#define COL20_IDR	GPIO_IDR_ID10
// COL21 PA15
#define COL21_GPIO	GPIOA
#define COL21_IDR	GPIO_IDR_ID15
// COL22 PA12
#define COL22_GPIO	GPIOA
#define COL22_IDR	GPIO_IDR_ID12
// COL23 PA11
#define COL23_GPIO	GPIOA
#define COL23_IDR	GPIO_IDR_ID11
// COL24 PA10
#define COL24_GPIO	GPIOA
#define COL24_IDR	GPIO_IDR_ID10
// COL25 PA9
#define COL25_GPIO	GPIOA
#define COL25_IDR	GPIO_IDR_ID9
// COL26 PA8
#define COL26_GPIO	GPIOA
#define COL26_IDR	GPIO_IDR_ID8
// COL27 PC9
#define COL27_GPIO	GPIOC
#define COL27_IDR	GPIO_IDR_ID9
// COL28 PC8
#define COL28_GPIO	GPIOC
#define COL28_IDR	GPIO_IDR_ID8
// COL29 PC7
#define COL29_GPIO	GPIOC
#define COL29_IDR	GPIO_IDR_ID7
// COL30 PC6
#define COL30_GPIO	GPIOC
#define COL30_IDR	GPIO_IDR_ID6
// COL31 PD15
#define COL31_GPIO	GPIOD
#define COL31_IDR	GPIO_IDR_ID15

uint16_t halleffect_rows;
uint32_t halleffect_cols;

void Driver_HallEffect_PollInputs()
{
	// need to shift hall data to correct bit position
	// shift amount calculated by function (IDR bit position - desired bit position)
	//
	// example: ROW1 is on PA7, so IDR is bit 7
	// desired position is bit 1 (2nd from right), so we calculate shift amount to be (7 - 1) = 6
	// final shift is >> 6
	//
	// positive values are right shift
	// negative values are left shift
	// zero value is no shift

	halleffect_rows = 0xFFFF;
	halleffect_rows ^= (ROW0_GPIO->IDR & ROW0_IDR) >> 6;
	halleffect_rows ^= (ROW1_GPIO->IDR & ROW1_IDR) >> 6;
	halleffect_rows ^= (ROW2_GPIO->IDR & ROW2_IDR) >> 2;
	halleffect_rows ^= (ROW3_GPIO->IDR & ROW3_IDR) >> 2;
	halleffect_rows ^= (ROW4_GPIO->IDR & ROW4_IDR) << 3;
	halleffect_rows ^= (ROW5_GPIO->IDR & ROW5_IDR) << 3;
	halleffect_rows ^= (ROW6_GPIO->IDR & ROW6_IDR) >> 1;
	halleffect_rows ^= (ROW7_GPIO->IDR & ROW7_IDR) >> 1;
	halleffect_rows ^= (ROW8_GPIO->IDR & ROW8_IDR) >> 1;
	halleffect_rows ^= (ROW9_GPIO->IDR & ROW9_IDR) >> 1;
	halleffect_rows ^= (ROW10_GPIO->IDR & ROW10_IDR) >> 1;
	halleffect_rows ^= (ROW11_GPIO->IDR & ROW11_IDR) >> 1;
	halleffect_rows ^= (ROW12_GPIO->IDR & ROW12_IDR) >> 1;
	halleffect_rows ^= (ROW13_GPIO->IDR & ROW13_IDR) >> 1;
	halleffect_rows ^= (ROW14_GPIO->IDR & ROW14_IDR) >> 1;
	halleffect_rows ^= (ROW15_GPIO->IDR & ROW15_IDR) << 5;

	halleffect_cols = 0xFFFFFFFF;
	halleffect_cols ^= (COL0_GPIO->IDR & COL0_IDR) >> 2;
	halleffect_cols ^= (COL1_GPIO->IDR & COL1_IDR);
	halleffect_cols ^= (COL2_GPIO->IDR & COL2_IDR) << 2;
	halleffect_cols ^= (COL3_GPIO->IDR & COL3_IDR) >> 6;
	halleffect_cols ^= (COL4_GPIO->IDR & COL4_IDR) >> 4;
	halleffect_cols ^= (COL5_GPIO->IDR & COL5_IDR) >> 2;
	halleffect_cols ^= (COL6_GPIO->IDR & COL6_IDR);
	halleffect_cols ^= (COL7_GPIO->IDR & COL7_IDR) << 2;
	halleffect_cols ^= (COL8_GPIO->IDR & COL8_IDR) << 4;
	halleffect_cols ^= (COL9_GPIO->IDR & COL9_IDR) << 6;
	halleffect_cols ^= (COL10_GPIO->IDR & COL10_IDR) << 3;
	halleffect_cols ^= (COL11_GPIO->IDR & COL11_IDR) << 5;
	halleffect_cols ^= (COL12_GPIO->IDR & COL12_IDR) << 7;
	halleffect_cols ^= (COL13_GPIO->IDR & COL13_IDR) << 9;
	halleffect_cols ^= (COL14_GPIO->IDR & COL14_IDR) << 11;
	halleffect_cols ^= (COL15_GPIO->IDR & COL15_IDR) << 13;
	halleffect_cols ^= (COL16_GPIO->IDR & COL16_IDR) << 15;
	halleffect_cols ^= (COL17_GPIO->IDR & COL17_IDR) << 17;
	halleffect_cols ^= (COL18_GPIO->IDR & COL18_IDR) << 6;
	halleffect_cols ^= (COL19_GPIO->IDR & COL19_IDR) << 8;
	halleffect_cols ^= (COL20_GPIO->IDR & COL20_IDR) << 10;
	halleffect_cols ^= (COL21_GPIO->IDR & COL21_IDR) << 6;
	halleffect_cols ^= (COL22_GPIO->IDR & COL22_IDR) << 10;
	halleffect_cols ^= (COL23_GPIO->IDR & COL23_IDR) << 12;
	halleffect_cols ^= (COL24_GPIO->IDR & COL24_IDR) << 14;
	halleffect_cols ^= (COL25_GPIO->IDR & COL25_IDR) << 16;
	halleffect_cols ^= (COL26_GPIO->IDR & COL26_IDR) << 18;
	halleffect_cols ^= (COL27_GPIO->IDR & COL27_IDR) << 18;
	halleffect_cols ^= (COL28_GPIO->IDR & COL28_IDR) << 20;
	halleffect_cols ^= (COL29_GPIO->IDR & COL29_IDR) << 22;
	halleffect_cols ^= (COL30_GPIO->IDR & COL30_IDR) << 24;
	halleffect_cols ^= (COL31_GPIO->IDR & COL31_IDR) << 16;
}
