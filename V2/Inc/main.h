/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
/* USER CODE BEGIN Private defines */
//波形显示的最大点数
#define WAVE_XMAX 220
#define WAVE_YMAX 220

//1ms采集一次，这是3.08s的数据
#define ADC_LENGTH 3080




//按键操作
#define KEY1_DOWN HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_6) == GPIO_PIN_RESET
#define KEY1_UP HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_6) == GPIO_PIN_SET
#define KEY2_DOWN HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_7) == GPIO_PIN_RESET
#define KEY2_UP HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_7) == GPIO_PIN_SET


//ADC精度范围
#define ADC_RANGE 4096




#define ScaleValue 0.991156f /* 滤波器放大系数 */





/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
