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
#include "stm32f4xx_hal.h"

#include "stm32f4xx_ll_adc.h"
#include "stm32f4xx_ll_dma.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_exti.h"
#include "stm32f4xx_ll_cortex.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_rtc.h"
#include "stm32f4xx_ll_spi.h"
#include "stm32f4xx_ll_tim.h"
#include "stm32f4xx_ll_usart.h"
#include "stm32f4xx_ll_gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdint.h>
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
#define KEY1_Pin LL_GPIO_PIN_3
#define KEY1_GPIO_Port GPIOE
#define KEY0_Pin LL_GPIO_PIN_4
#define KEY0_GPIO_Port GPIOE
#define T_CS_Pin LL_GPIO_PIN_13
#define T_CS_GPIO_Port GPIOC
#define LED0_Pin LL_GPIO_PIN_9
#define LED0_GPIO_Port GPIOF
#define LED1_Pin LL_GPIO_PIN_10
#define LED1_GPIO_Port GPIOF
#define WK_UP_Pin LL_GPIO_PIN_0
#define WK_UP_GPIO_Port GPIOA
#define T_SCK_Pin LL_GPIO_PIN_0
#define T_SCK_GPIO_Port GPIOB
#define T_PEN_Pin LL_GPIO_PIN_1
#define T_PEN_GPIO_Port GPIOB
#define T_PEN_EXTI_IRQn EXTI1_IRQn
#define T_MISO_Pin LL_GPIO_PIN_2
#define T_MISO_GPIO_Port GPIOB
#define T_MOSI_Pin LL_GPIO_PIN_11
#define T_MOSI_GPIO_Port GPIOF
#define F_CS_Pin LL_GPIO_PIN_14
#define F_CS_GPIO_Port GPIOB
#define LCD_BL_Pin LL_GPIO_PIN_15
#define LCD_BL_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define ENUM_ITEM(ITEM) ITEM,
#define ENUM_STRING(ITEM) #ITEM,
#define KEY_ENUM(NAME) \
  NAME(WK_UP)          \
  NAME(KEY0)           \
  NAME(KEY1)           \
  NAME(KEY_NUM)

typedef enum {
  KEY_ENUM(ENUM_ITEM)
} KEY_NAME;

#define _CCM_DATA __attribute__((section(".ccmram.data")))
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
