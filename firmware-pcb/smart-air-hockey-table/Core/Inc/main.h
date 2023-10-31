/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_hal.h"
#include "stm32u5xx_ll_dma.h"
#include "stm32u5xx_ll_icache.h"
#include "stm32u5xx_ll_pwr.h"
#include "stm32u5xx_ll_crs.h"
#include "stm32u5xx_ll_rcc.h"
#include "stm32u5xx_ll_bus.h"
#include "stm32u5xx_ll_system.h"
#include "stm32u5xx_ll_exti.h"
#include "stm32u5xx_ll_cortex.h"
#include "stm32u5xx_ll_utils.h"
#include "stm32u5xx_ll_spi.h"
#include "stm32u5xx_ll_tim.h"
#include "stm32u5xx_ll_gpio.h"
#include "stm32u5xx_ll_lpgpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define COL0_Pin LL_GPIO_PIN_2
#define COL0_GPIO_Port GPIOE
#define ENCODER_A_Pin LL_GPIO_PIN_3
#define ENCODER_A_GPIO_Port GPIOE
#define ENCODER_B_Pin LL_GPIO_PIN_4
#define ENCODER_B_GPIO_Port GPIOE
#define ENCODER_SW_Pin LL_GPIO_PIN_5
#define ENCODER_SW_GPIO_Port GPIOE
#define ENCODER_SW_EXTI_IRQn EXTI5_IRQn
#define LDR1IN_Pin LL_GPIO_PIN_6
#define LDR1IN_GPIO_Port GPIOE
#define LDR2IN_Pin LL_GPIO_PIN_13
#define LDR2IN_GPIO_Port GPIOC
#define LDR3IN_Pin LL_GPIO_PIN_14
#define LDR3IN_GPIO_Port GPIOC
#define LDR4IN_Pin LL_GPIO_PIN_15
#define LDR4IN_GPIO_Port GPIOC
#define OSC_IN_Pin LL_GPIO_PIN_0
#define OSC_IN_GPIO_Port GPIOH
#define DEBUG1_Pin LL_GPIO_PIN_0
#define DEBUG1_GPIO_Port GPIOC
#define DEBUG2_Pin LL_GPIO_PIN_1
#define DEBUG2_GPIO_Port GPIOC
#define DEBUG3_Pin LL_GPIO_PIN_2
#define DEBUG3_GPIO_Port GPIOC
#define DEBUG4_Pin LL_GPIO_PIN_3
#define DEBUG4_GPIO_Port GPIOC
#define LED1OUT_Pin LL_GPIO_PIN_0
#define LED1OUT_GPIO_Port GPIOA
#define LED2OUT_Pin LL_GPIO_PIN_1
#define LED2OUT_GPIO_Port GPIOA
#define LED3OUT_Pin LL_GPIO_PIN_2
#define LED3OUT_GPIO_Port GPIOA
#define LED4OUT_Pin LL_GPIO_PIN_3
#define LED4OUT_GPIO_Port GPIOA
#define ROW0_Pin LL_GPIO_PIN_6
#define ROW0_GPIO_Port GPIOA
#define ROW1_Pin LL_GPIO_PIN_7
#define ROW1_GPIO_Port GPIOA
#define ROW2_Pin LL_GPIO_PIN_4
#define ROW2_GPIO_Port GPIOC
#define ROW3_Pin LL_GPIO_PIN_5
#define ROW3_GPIO_Port GPIOC
#define ROW4_Pin LL_GPIO_PIN_1
#define ROW4_GPIO_Port GPIOB
#define ROW5_Pin LL_GPIO_PIN_2
#define ROW5_GPIO_Port GPIOB
#define ROW6_Pin LL_GPIO_PIN_7
#define ROW6_GPIO_Port GPIOE
#define ROW7_Pin LL_GPIO_PIN_8
#define ROW7_GPIO_Port GPIOE
#define ROW8_Pin LL_GPIO_PIN_9
#define ROW8_GPIO_Port GPIOE
#define ROW9_Pin LL_GPIO_PIN_10
#define ROW9_GPIO_Port GPIOE
#define ROW10_Pin LL_GPIO_PIN_11
#define ROW10_GPIO_Port GPIOE
#define ROW11_Pin LL_GPIO_PIN_12
#define ROW11_GPIO_Port GPIOE
#define ROW12_Pin LL_GPIO_PIN_13
#define ROW12_GPIO_Port GPIOE
#define ROW13_Pin LL_GPIO_PIN_14
#define ROW13_GPIO_Port GPIOE
#define ROW14_Pin LL_GPIO_PIN_15
#define ROW14_GPIO_Port GPIOE
#define ROW15_Pin LL_GPIO_PIN_10
#define ROW15_GPIO_Port GPIOB
#define MEM1CS_Pin LL_GPIO_PIN_12
#define MEM1CS_GPIO_Port GPIOB
#define SPI2SCK_Pin LL_GPIO_PIN_13
#define SPI2SCK_GPIO_Port GPIOB
#define SPI2MISO_Pin LL_GPIO_PIN_14
#define SPI2MISO_GPIO_Port GPIOB
#define SPI2MOSI_Pin LL_GPIO_PIN_15
#define SPI2MOSI_GPIO_Port GPIOB
#define MEMHOLD_Pin LL_GPIO_PIN_8
#define MEMHOLD_GPIO_Port GPIOD
#define MEMW_Pin LL_GPIO_PIN_9
#define MEMW_GPIO_Port GPIOD
#define OLED1CS_Pin LL_GPIO_PIN_10
#define OLED1CS_GPIO_Port GPIOD
#define OLED2CS_Pin LL_GPIO_PIN_11
#define OLED2CS_GPIO_Port GPIOD
#define OLEDDC_Pin LL_GPIO_PIN_12
#define OLEDDC_GPIO_Port GPIOD
#define OLEDRES_Pin LL_GPIO_PIN_13
#define OLEDRES_GPIO_Port GPIOD
#define RELAY_Pin LL_GPIO_PIN_14
#define RELAY_GPIO_Port GPIOD
#define COL31_Pin LL_GPIO_PIN_15
#define COL31_GPIO_Port GPIOD
#define COL30_Pin LL_GPIO_PIN_6
#define COL30_GPIO_Port GPIOC
#define COL29_Pin LL_GPIO_PIN_7
#define COL29_GPIO_Port GPIOC
#define COL28_Pin LL_GPIO_PIN_8
#define COL28_GPIO_Port GPIOC
#define COL27_Pin LL_GPIO_PIN_9
#define COL27_GPIO_Port GPIOC
#define COL26_Pin LL_GPIO_PIN_8
#define COL26_GPIO_Port GPIOA
#define COL25_Pin LL_GPIO_PIN_9
#define COL25_GPIO_Port GPIOA
#define COL24_Pin LL_GPIO_PIN_10
#define COL24_GPIO_Port GPIOA
#define COL23_Pin LL_GPIO_PIN_11
#define COL23_GPIO_Port GPIOA
#define COL22_Pin LL_GPIO_PIN_12
#define COL22_GPIO_Port GPIOA
#define SWDIO_Pin LL_GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin LL_GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define COL21_Pin LL_GPIO_PIN_15
#define COL21_GPIO_Port GPIOA
#define COL20_Pin LL_GPIO_PIN_10
#define COL20_GPIO_Port GPIOC
#define COL19_Pin LL_GPIO_PIN_11
#define COL19_GPIO_Port GPIOC
#define COL18_Pin LL_GPIO_PIN_12
#define COL18_GPIO_Port GPIOC
#define COL17_Pin LL_GPIO_PIN_0
#define COL17_GPIO_Port GPIOD
#define COL16_Pin LL_GPIO_PIN_1
#define COL16_GPIO_Port GPIOD
#define COL15_Pin LL_GPIO_PIN_2
#define COL15_GPIO_Port GPIOD
#define COL14_Pin LL_GPIO_PIN_3
#define COL14_GPIO_Port GPIOD
#define COL13_Pin LL_GPIO_PIN_4
#define COL13_GPIO_Port GPIOD
#define COL12_Pin LL_GPIO_PIN_5
#define COL12_GPIO_Port GPIOD
#define COL11_Pin LL_GPIO_PIN_6
#define COL11_GPIO_Port GPIOD
#define COL10_Pin LL_GPIO_PIN_7
#define COL10_GPIO_Port GPIOD
#define COL9_Pin LL_GPIO_PIN_3
#define COL9_GPIO_Port GPIOB
#define COL8_Pin LL_GPIO_PIN_4
#define COL8_GPIO_Port GPIOB
#define COL7_Pin LL_GPIO_PIN_5
#define COL7_GPIO_Port GPIOB
#define COL6_Pin LL_GPIO_PIN_6
#define COL6_GPIO_Port GPIOB
#define COL5_Pin LL_GPIO_PIN_7
#define COL5_GPIO_Port GPIOB
#define COL4_Pin LL_GPIO_PIN_8
#define COL4_GPIO_Port GPIOB
#define COL3_Pin LL_GPIO_PIN_9
#define COL3_GPIO_Port GPIOB
#define COL2_Pin LL_GPIO_PIN_0
#define COL2_GPIO_Port GPIOE
#define COL1_Pin LL_GPIO_PIN_1
#define COL1_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
