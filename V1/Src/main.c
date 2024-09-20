/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "adc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd_init.h"
#include "lcd.h"
#include "pic.h"
#include "delay.h"
#include "math.h"
#include "Key.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint16_t showmax;
uint16_t showmin=4095;
uint16_t data1[240];
uint16_t data2[240];
uint8_t high=200;
uint8_t low=40;
float_t ytran=1;
float_t xtran=1.0;
extern uint16_t dur;
char *strx,*extstrx,*Readystrx;
char RxBuffer[200],Rxcounter;
extern char RxBuffer[200],Rxcounter;
char *strstr(const char*,const char*);
uint8_t  Res;
uint8_t enable=0;
uint8_t signal=0;
uint8_t alarm=0;
uint8_t frsign=0;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	UNUSED(huart);
	if(huart->Instance==USART1 )
	{
		RxBuffer[Rxcounter++]=Res;
		HAL_Delay(200);
		HAL_UART_Receive_IT(&huart1,(uint8_t *)&Res,1);
	}
}
void write(void)
{
	RxBuffer[Rxcounter++]=Res;
	return;
}
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
void Clear_Buffer(void)
{
	uint8_t i;
	for(i=0;i<Rxcounter ;i++)
		RxBuffer [i]=0;
	Rxcounter =0;
}
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC2_Init();
  MX_SPI5_Init();
  MX_TIM6_Init();
  MX_TIM2_Init();
  MX_TIM7_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

	delay_init(180);
	LCD_Init ();
	LCD_Fill (0,0,LCD_W,LCD_H,WHITE);
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_Base_Start_IT(&htim4);
HAL_TIM_Base_Start_IT(&htim6);
HAL_TIM_Base_Start_IT(&htim7);
HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);

//HAL_UART_Receive_IT(&huart1,&Res,1);
	
		printf("start");
		HAL_Delay(500);
		
		printf("ATI\r\n");
		HAL_Delay(1000);
	//	strx=strstr((const char*)RxBuffer,(const char*)"Fibocom");
 /*	while(strx==NULL)
		{
			Clear_Buffer();	
			printf("fail");
			HAL_Delay(1000);
			printf("ATI\r\n");
		  HAL_Delay(1000);		
			strx=strstr((const char*)RxBuffer,(const char*)"Fibocom");
		}*/
		Clear_Buffer();
		printf("success");
		HAL_Delay(500);
		
		printf("AT+MIPCALL?\r\n");
		HAL_Delay(500);
		/*strx=strstr((const char*)RxBuffer,(const char*)"+MIPCALL: 1");
		while(strx==NULL)
		{
			Clear_Buffer();	
			printf("NO IP");
			HAL_Delay(1000);
			printf("AT+MIPCALL=1\r\n");
			HAL_Delay(1000);
			strx=strstr((const char*)RxBuffer,(const char*)"+MIPCALL: ");
		}*/
		Clear_Buffer();
		printf("GET IP");
		HAL_Delay(500);
		
		printf("AT+TCDEVINFOSET=1,\"69KWHNFFUV\",\"test1\",\"qx+oUYsrSWLERY7GuzcI4A==\"\r\n");
		HAL_Delay(500);
		/*strx=strstr((const char*)RxBuffer,(const char*)"+TCDEVINFOSET: OK");
		while(strx==NULL)
		{
			Clear_Buffer();	
			printf("fail");
			HAL_Delay(1000);
		printf("AT+TCDEVINFOSET=1,\"69KWHNFFUV\",\"test1\",\"qx+oUYsrSWLERY7GuzcI4A==\"\r\n");
			HAL_Delay(1000);
			strx=strstr((const char*)RxBuffer,(const char*)"+TCDEVINFOSET: OK");
		}*/
		Clear_Buffer();
		printf("success");
		HAL_Delay(500);
		
		printf("AT+TCMQTTCONN=1,20000,240,1,0\r\n");
		HAL_Delay(500);
		/*strx=strstr((const char*)RxBuffer,(const char*)"+TCMQTTCONN: OK");
		while(strx==NULL)
		{
			Clear_Buffer();	
			printf("fail");
			HAL_Delay(1000);
			printf("AT+TCMQTTCONN=1,20000,240,1,1\r\n");
			HAL_Delay(1000);
			strx=strstr((const char*)RxBuffer,(const char*)"+TCMQTTCONN: OK");
		}*/
		Clear_Buffer();
		printf("success");
		HAL_Delay(500);
		
		printf("AT+TCMQTTSUB=\"$thing/down/property/69KWHNFFUV/test1\",1\r\n");
		HAL_Delay(500);
		/*strx=strstr((const char*)RxBuffer,(const char*)"+TCMQTTSUB: OK");
		while(strx==NULL)
		{
			Clear_Buffer();	
			printf("fail");
			HAL_Delay(1000);
			printf("AT+TCMQTTSUB=\"$thing/down/property/69KWHNFFUV/test1\",1\r\n");
			HAL_Delay(1000);
			strx=strstr((const char*)RxBuffer,(const char*)"+TCMQTTSUB: OK");
		}*/
		Clear_Buffer();
		printf("success");
		enable=1;
		HAL_Delay(500);
		printf("AT+GTTS=1,\"开始心电检监测\",1\r\n");
		HAL_Delay(2000);
		printf("AT+GTTS=0\r\n");
		HAL_Delay(500);
	printf("AT+TCMQTTPUB=\"$thing/up/property/69KWHNFFUV/test1\",1,\"{\\\"method\\\":\\\"report\\\",\\\"clientToken\\\":\\\"123\\\",\\\"params\\\":{\\\"lac\\\":%d}}\"\r\n",31);
		HAL_Delay(500);
			printf("AT+TCMQTTPUB=\"$thing/up/property/69KWHNFFUV/test1\",1,\"{\\\"method\\\":\\\"report\\\",\\\"clientToken\\\":\\\"123\\\",\\\"params\\\":{\\\"cid\\\":%d}}\"\r\n",17);
		HAL_Delay(500);
		while(enable)
		{
			frsign =(int)(60/(dur*12*1e-3));
			printf("AT+TCMQTTPUB=\"$thing/up/property/69KWHNFFUV/test1\",1,\"{\\\"method\\\":\\\"report\\\",\\\"clientToken\\\":\\\"123\\\",\\\"params\\\":{\\\"span\\\":%d}}\"\r\n",(int)(60/(dur*12*1e-3)));
		HAL_Delay(500);
			printf("AT+TCMQTTPUB=\"$thing/up/property/69KWHNFFUV/test1\",1,\"{\\\"method\\\":\\\"report\\\",\\\"clientToken\\\":\\\"123\\\",\\\"params\\\":{\\\"ytran\\\":%f}}\"\r\n",ytran);
		HAL_Delay(500);
			printf("AT+TCMQTTPUB=\"$thing/up/property/69KWHNFFUV/test1\",1,\"{\\\"method\\\":\\\"report\\\",\\\"clientToken\\\":\\\"123\\\",\\\"params\\\":{\\\"xtran\\\":%f}}\"\r\n",xtran);
		HAL_Delay(500);
			if((frsign>120||frsign<50)&&alarm==0)
			{
				alarm++;
				printf("AT+GTTS=1,\"心率异常已发送您的位置\",1\r\n");
		HAL_Delay(3000);
		printf("AT+GTTS=0\r\n");
		HAL_Delay(500);
			}
			
		/*HAL_Delay(50000000);
			//}
			if(strx)
			{
				Clear_Buffer();
				printf("GET NUMBER");
				HAL_Delay(500);
			}
		while(strx==NULL)
		{
			Clear_Buffer();	
			printf("fail");
			HAL_Delay(1000);
			printf("AT+TCMQTTPUB=\"$thing/up/property/69KWHNFFUV/test1\",1,\"{\\\"method\\\":\\\"report\\\",\\\"clientToken\\\":\\\"123\\\",\\\"params\\\":{\\\"span\\\":%d}}\"\r\n",(int)(60/(dur*12*1e-3)));
			HAL_Delay(1000);
			strx=strstr((const char*)RxBuffer,(const char*)"success");
		}*/
		Clear_Buffer();
		printf("success");
		HAL_Delay(500);
	}
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode 
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
