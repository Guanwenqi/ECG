/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "adc.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"
#include "lcd_init.h"
#include "lcd.h"
#include "delay.h"
#include "math.h"
#include "Key.h"
#define A 250       
uint16_t Get_Adc(ADC_HandleTypeDef *adcHandle)
{
	HAL_ADC_Start(adcHandle);
	HAL_ADC_PollForConversion (adcHandle,10);
	return (uint16_t )HAL_ADC_GetValue (adcHandle);
}
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */
uint16_t max;
uint16_t min=240;
uint16_t count=0;
extern uint8_t signal;
extern uint16_t data1[240];
extern uint16_t data2[240];
extern uint16_t showmax;
extern uint16_t showmin;
extern uint8_t high;//2.72V
extern uint8_t low;//0.54V
uint16_t trigger;
uint16_t down;
uint8_t i=1;
uint8_t k;
uint16_t temp;
uint8_t flag;
uint8_t fin;
uint16_t dur;
extern float_t ytran;
extern float_t xtran;
float_t ini_ytran=1;
float_t ini_xtran=1;
uint8_t cnty=1;
uint8_t cntx=1;
uint16_t cntbuzz=1;
/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim7;
extern UART_HandleTypeDef huart1;
/* USER CODE BEGIN EV */

/*void LAverageFilter()
{
  int i;
  int temp,sum,flag=0;
  data[0]=HAL_ADC_GetValue(&hadc2);
	for(i=1;i<240;i++)
	{
		temp=HAL_ADC_GetValue(&hadc2);
		if((temp-data[i-1])>A||((data[i-1]-temp)>A))
		{
		  i--;flag++;
		}
		else
		{
			data[i]=temp;
		}
	}
  for(i=0;i<240;i++)
  {
    sum+=data[i];
  } 
}*/
/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */ 
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */

  /* USER CODE END TIM2_IRQn 0 */
  HAL_TIM_IRQHandler(&htim2);
  /* USER CODE BEGIN TIM2_IRQn 1 */

  /* USER CODE END TIM2_IRQn 1 */
}

/**
  * @brief This function handles TIM3 global interrupt.
  */
void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */
   Key2_Run();
  /* USER CODE END TIM3_IRQn 0 */
  HAL_TIM_IRQHandler(&htim3);
  /* USER CODE BEGIN TIM3_IRQn 1 */

  /* USER CODE END TIM3_IRQn 1 */
}

/**
  * @brief This function handles TIM4 global interrupt.
  */
void TIM4_IRQHandler(void)
{
  /* USER CODE BEGIN TIM4_IRQn 0 */
	 if(1.0f/(dur*12*1e-3)>2||1.0f/(dur*12*1e-3)<=0.85)
		{
			cntbuzz%=799;
			if(cntbuzz<400)
			{
    HAL_GPIO_TogglePin (GPIOB,GPIO_PIN_15); 
			cntbuzz++;
			}
			else
				cntbuzz++;
		
		}
  /* USER CODE END TIM4_IRQn 0 */
  HAL_TIM_IRQHandler(&htim4);
  /* USER CODE BEGIN TIM4_IRQn 1 */

  /* USER CODE END TIM4_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}

/**
  * @brief This function handles TIM6 global interrupt, DAC1 and DAC2 underrun error interrupts.
  */
void TIM6_DAC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM6_DAC_IRQn 0 */
	if(!count)
	{
	data1[count]=239-Get_Adc (&hadc2)*240/4096;
		count++;
	}
	else
	{
		temp=239-Get_Adc (&hadc2)*240/4096;
		for(i=count;i>=1;)
		{
			if(fabs(temp-data1[i-1])>A)
		{
		  i--;
			if(!i)
			{
				flag++;
				if(flag>3)
				{
					flag=0;
					data1[i]=temp;
					count=i+1;
				}
			}
		}
		else
		{
			data1[count]=temp;
			count=i+1;
			break;
		}
		}
	}
	if(data1[count-1]>=max)
		max=data1[count-1];
	if(data1[count-1]<min)
		min=data1[count-1];
	if(count==240)
	{
		count=0;
		showmax=min;
		showmin=max;
		max=0;
		min=4095;
  /* USER CODE BEGIN TIM6_DAC_IRQn 0 */
 if(ini_xtran!=xtran||ini_ytran!=ytran)
 {
	 for(i=1;i<240;i++)
	 {
		 	LCD_DrawLine((i-1)/(ini_xtran),209-(239-data2[i-1])/(ini_ytran),i/(ini_xtran),209-(239-data2[i])/(ini_ytran),WHITE);
	 }
	 if(ini_xtran!=xtran)
	 {
		 ini_xtran =xtran;
	 }
	 else if(ini_ytran!=ytran)
	 {
		 ini_ytran =ytran;
	 }
 }
		
			for(i=1;i<240;i++)
		{
		
			if(!fin&&data1[i-1]<=low)
			{
				for(k=i;k<240;k++)
				{
				
					if(data1[k]>=high)
					{
						trigger=i-1;
						down=k;
						fin=1;
						break;
					}
						
				}
			}
				else if(!fin&&data1[i-1]>=high)
			{
				for(k=i;k<240;k++)
				{
					
					if(data1[k]<=low)
					{
						down=i-1;
						trigger=k;
						fin=1;
						break;
					}
				}
			}
				dur=fabs(trigger-down);
		
			if((i-1)/xtran>30&&i/xtran<240)
			{
			LCD_DrawLine((i-1)/xtran,209-(239-data2[i-1])/(ytran),i/xtran,209-(239-data2[i])/(ytran),WHITE);
		  LCD_DrawLine((i-1)/xtran,209-(239-data1[i-1])/ytran,i/xtran,209-(239-data1[i])/ytran,RED);
			}
			data2[i-1]=data1[i-1];
		}
		
		data2[239]=data1[239];
		LCD_ShowString(163,10,(uint8_t *)"Max:",BLACK,WHITE,16,0);
  LCD_ShowString(163,25,(uint8_t *)"Min:",BLACK,WHITE,16,0);
		LCD_ShowString(163,40,(uint8_t *)"Fre:",BLACK,WHITE,16,0);
			LCD_ShowString(65,10,(uint8_t *)"Y:x",BLACK,WHITE,16,0);
		  	LCD_ShowString(65,25,(uint8_t *)"X:x",BLACK,WHITE,16,0);
		LCD_ShowString(65,40,(uint8_t *)"Rate:",BLACK,WHITE,16,0);
		LCD_ShowFloatNum1(95,10,ytran,3,BLACK,WHITE,16);
		LCD_ShowFloatNum1(95,25,xtran,3,BLACK,WHITE,16);
  LCD_ShowFloatNum1(193,10,(239-showmax)*3.3/240,3,BLACK,WHITE,16);
  LCD_ShowFloatNum1(193,25,(239-showmin)*3.3/240,3,BLACK,WHITE,16);
		LCD_ShowFloatNum1(193,40,1.0f/(dur*12*1e-3),3,BLACK,WHITE,16);
		LCD_ShowIntNum(105,40,60/(dur*12*1e-3),3,BLACK,WHITE,16);
		LCD_DrawLine(35,210,235,210,BLACK);
		LCD_DrawLine(35,210,35,20,BLACK);
		//x-axis
		LCD_ShowIntNum(15,215,0*xtran,3,BLACK,WHITE,16);
		LCD_ShowIntNum(55,215,30*xtran,3,BLACK,WHITE,16);
		LCD_ShowIntNum(95,215,60*xtran,3,BLACK,WHITE,16);
		LCD_ShowIntNum(135,215,90*xtran,3,BLACK,WHITE,16);
		LCD_ShowIntNum(175,215,120*xtran,3,BLACK,WHITE,16);
			LCD_ShowString(215,215,(uint8_t *)"/ms",BLACK,WHITE,16,0);
		//y-axis
		//LCD_ShowFloatNum1(10,215,0,3,BLACK,WHITE,16);
		LCD_ShowFloatNum1(2,175,0.55*ytran,3,BLACK,WHITE,16);
		LCD_ShowFloatNum1(2,135,1.1*ytran,3,BLACK,WHITE,16);
		LCD_ShowFloatNum1(2,95,1.65*ytran,3,BLACK,WHITE,16);
		LCD_ShowFloatNum1(2,55,2.2*ytran,3,BLACK,WHITE,16);
			LCD_ShowString(10,15,(uint8_t *)"/V",BLACK,WHITE,16,0);
				//LCD_ShowIntNum(175,155,signal,3,BLACK,WHITE,16);
	trigger=0;
		down=0;
		fin=0;
	}
 
  /* USER CODE END TIM6_DAC_IRQn 0 */
  HAL_TIM_IRQHandler(&htim6);
  /* USER CODE BEGIN TIM6_DAC_IRQn 1 */
  /* USER CODE END TIM6_DAC_IRQn 1 */
}

/**
  * @brief This function handles TIM7 global interrupt.
  */
void TIM7_IRQHandler(void)
{
  /* USER CODE BEGIN TIM7_IRQn 0 */
 Key_Run();
  /* USER CODE END TIM7_IRQn 0 */
  HAL_TIM_IRQHandler(&htim7);
  /* USER CODE BEGIN TIM7_IRQn 1 */

  /* USER CODE END TIM7_IRQn 1 */
}

/* USER CODE BEGIN 1 */
/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
