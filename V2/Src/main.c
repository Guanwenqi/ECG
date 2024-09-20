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
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include <string.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
#include "lcd_init.h"
#include "pic.h"
#include "delay.h"
#include "stdlib.h"
#include "arm_math.h"
#include <stdio.h>




//������������
char *strx,*extstrx,*Readystrx; 
char RxBuffer[200],Rxcouter; 
//extern char RxBuffer[200],Rxcouter; 
char *strstr(const char *, const char *); 
uint8_t Res;
int len;
int number;
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define numStages 2 /*2��IIR�˲��ĸ��� */
#define TEST_LENGTH_SAMPLES 3080 /* �������� */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint16_t wave1_data1[ADC_LENGTH]={0};
uint16_t wave1_data2[ADC_LENGTH]={0};
uint16_t adc_data[ADC_LENGTH]={0};

//Ҫ���͸���Ѷ�Ƶ���������
uint16_t send_data[ADC_LENGTH/20]={0};

//��¼��Ļ��һ��ͼÿ�������ʵ���꣨0-240��
uint16_t wave1_data_old[WAVE_XMAX]={0};

static float32_t testInput_f32_50Hz_200Hz[TEST_LENGTH_SAMPLES]; /* ������ */


static float32_t testOutput[TEST_LENGTH_SAMPLES]; /*�˲������� */
static float32_t IIRStateF32[4 * numStages]; /*״̬����,��СnumTaps +blockSize- 1*/

/*������˹�����˲���ϵ��49-51Hz*/
const float32_t IIRCoeffs32BS[5 * numStages] = {
1.0f, -1.902150579414243258824512849969323724508f,  1.0f, 1.890851463462387416214482982468325644732f, -0.991033147905535649613284476799890398979f, 
1.0f, -1.902150579414243258824512849969323724508f,  1.0f, 1.896548069620123788681098631059285253286f, -0.991274058481610320647803291649324819446f

};



//���������������ڷŴ���С����
float height_limit=3.3;//�߶����ޣ�����3.3V

float width_limit=3;//������ޣ�����3s

//������ͣ���ܱ�־λ
uint16_t stopbutton=0;


//���ڼ�����ÿ��һ��ʱ�䷢��һ������
uint16_t cnt_for_send=0;


//DMA���˵�ַ
uint16_t *p_save_wave1 = wave1_data1;

//��ͼ��ȡ���ݵ�ַ
uint16_t *p_draw_wave1 = wave1_data2;

//ADC�ɼ���ַ
uint16_t *p_adc_wave1 = adc_data;

__IO uint8_t AdcConvEnd = 0;
uint16_t i_limite=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

//������ݽ���buf
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)  
{   
UNUSED(huart);   
if(huart->Instance ==USART1){  
RxBuffer[Rxcouter++] = Res;      
HAL_UART_Receive_IT(&huart1, (uint8_t *)&Res, 1);     
}  
} 

//��ӽ�������buffer�Ĵ���
void Clear_Buffer(void) 
{
  uint8_t i;
  for (i = 0; i < Rxcouter; i++)
    RxBuffer[i] = 0;
  Rxcouter = 0; 
}




uint16_t FindTriggerPointchen(float triggerV, uint8_t isRise, uint16_t *p_data);
void GetHeartRate(uint16_t *p_data);
void DrawAxis(void);
void WaveClear(void);
static void arm_iir_f32_bs(void);
void ShowGap(float vertical, float horizontal);
void DrawWave(uint16_t *p_old, uint16_t *p_draw,float vertical,float horizontal);
uint16_t FindTriggerPoint(float triggerV, uint8_t isRise, uint16_t *p_data);
void sendDynamicArrayOverMQTT();
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
  MX_DMA_Init();
  MX_SPI5_Init();
  MX_ADC1_Init();
  MX_TIM3_Init();
  MX_TIM6_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	delay_init(180);
	LCD_Init();
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	HAL_TIM_Base_Start(&htim3);
	HAL_TIM_Base_Start_IT(&htim6);	
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)p_adc_wave1, ADC_LENGTH);
  HAL_UART_Receive_IT(&huart1,&Res,1);//�����жϽ���


	  //ģ���ʼ�����ȴ�5��
		printf("ģ���ʼ��");
		HAL_Delay(5000);
		
		//��ѯ�汾��Ϣ
		printf("ATI\r\n");
		HAL_Delay(1000);
		strx=strstr((const char*)RxBuffer,(const char*)"Fibocom");
		while(strx==NULL)
		{
			Clear_Buffer();	
			printf("��ѯ��Ϣʧ��");
			HAL_Delay(1000);
			printf("ATI\r\n");
		  HAL_Delay(1000);		
			strx=strstr((const char*)RxBuffer,(const char*)"Fibocom");
		}
		Clear_Buffer();
		printf("�汾��Ϣ��ȷ");
		HAL_Delay(1000);
		
		//����IP
		printf("AT+MIPCALL?\r\n");
		HAL_Delay(1000);
		strx=strstr((const char*)RxBuffer,(const char*)"+MIPCALL: 1");
		while(strx==NULL)
		{
			Clear_Buffer();	
			printf("��δ��ȡ��IP");
			HAL_Delay(1000);
			printf("AT+MIPCALL=1\r\n");
			HAL_Delay(1000);
			strx=strstr((const char*)RxBuffer,(const char*)"+MIPCALL: ");
		}
		Clear_Buffer();
		printf("��ȡIP�ɹ�");
		HAL_Delay(1000);


//����ƽ̨�豸��Ϣ
		printf("AT+TCDEVINFOSET=1,\"XTRLYBWVYB\",\"L610\",\"cGF2xK1RsUx/sLFBd3YjjQ==\"\r\n");
		HAL_Delay(500);
		strx=strstr((const char*)RxBuffer,(const char*)"+TCDEVINFOSET: OK");
		while(strx==NULL)
		{
			Clear_Buffer();	
			printf("ƽ̨��Ϣ����ʧ��");
			HAL_Delay(500);
			printf("AT+TCDEVINFOSET=1,\"XTRLYBWVYB\",\"L610\",\"cGF2xK1RsUx/sLFBd3YjjQ==\"\r\n");
			HAL_Delay(500);
			strx=strstr((const char*)RxBuffer,(const char*)"+TCDEVINFOSET: OK");
		}
		Clear_Buffer();
		printf("ƽ̨��Ϣ���óɹ�");
		HAL_Delay(500);


//�������Ӳ���������
		printf("AT+TCMQTTCONN=1,20000,240,1,0\r\n");
		HAL_Delay(1000);
		strx=strstr((const char*)RxBuffer,(const char*)"+TCMQTTCONN: OK");
		while(strx==NULL)
		{
			Clear_Buffer();	
			printf("����ʧ��");
			HAL_Delay(2000);
			printf("AT+TCMQTTCONN=1,20000,240,1,0\r\n");
			HAL_Delay(2000);
			strx=strstr((const char*)RxBuffer,(const char*)"+TCMQTTCONN: OK");
		}
		Clear_Buffer();
		printf("���ӳɹ�");
		HAL_Delay(2000);

 printf("AT+GTTS=1,\"׼������\",1\r\n");
 HAL_Delay(1000);





  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

//�������
Clear_Buffer();


//��ѭ��
while (1)
{
  if (AdcConvEnd == 1)
  {

    // ��p_draw_wave1�����ݸ��Ƶ�testInput_f32_50Hz_200Hz
    // for (int p = 0; p < ADC_LENGTH; p++)
    // {
    //   testInput_f32_50Hz_200Hz[p] = p_draw_wave1[p];
    // }

    // ���ú���arm_iir_f32_bsʵ�ִ����˲���,�˲�������ݱ�����testOutput��
    // arm_iir_f32_bs();

    // ��testOutput�����ݸ��Ƶ�p_draw_wave1
    // for (int p = 0; p < ADC_LENGTH; p++)
    // {
    //   p_draw_wave1[p] = testOutput[p] * ScaleValue;
    // }


    GetHeartRate(p_draw_wave1); // �õ�Ƶ��,���а������ϴ����ݵĴ���


    DrawWave(wave1_data_old, p_draw_wave1, height_limit, width_limit); // ������

   //�����ݴ���send_data�����У����ȱ��ԭ����1/20
     for(int i=1;i<=(ADC_LENGTH/20);i++)
     {
       send_data[i-1]=p_draw_wave1[20*i-1];
    }
    

  //printf("AT+TCMQTTPUB =\"$thing/up/property/XTRLYBWVYB/L610\",1,\"{\\\"method\\\":\\\"report\\\",\\\"clientToken\\\":\\\"123\\\",\\\"params\\\":{\\\"c1\\\":[1,1,1,1,6,6,6,6,6,6,778,888,1000,1,1,1,1,6,6,6,6,6,6,778,888,1000,1,1,1,1,6,6,6,6,6,6,778,888,1000]}}\"\r\n");
    
if(send_data[0]!=0){
 printf("AT+TCMQTTPUB=\"$thing/up/property/XTRLYBWVYB/L610\",1,\"{\\\"method\\\":\\\"report\\\",\\\"clientToken\\\":\\\"123\\\",\\\"params\\\":{\\\"c%d\\\":[",cnt_for_send);
    for (int i = 0; i < ADC_LENGTH/20; i++) {
        printf("%d", send_data[i]);
        if (i <ADC_LENGTH/20-1) {
            printf(",");
        }
    }
    printf("]}}\"\r\n");


	 	Clear_Buffer();

  cnt_for_send++;

}

if(cnt_for_send==9){
  cnt_for_send=0;


 printf("AT+GTTS=1,\"ҽ�������ƶ��յ����ı���\",1\r\n");
delay_ms(500);

  printf("AT+TCMQTTDISCONN\r\n");




}
     







     AdcConvEnd = 0;
   }

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

//void ShowWave(void)

/*��ˮƽ���š���ֱ���Ź��ܵĻ��Ʋ��κ���*/
void DrawWave(uint16_t *p_old, uint16_t *p_draw,float vertical, float horizontal)
{

static uint8_t isDraw=0;
//��һ�λ��Ʋ���ʱ�������ʷ����,����������
 
if(!isDraw)
		{
		WaveClear();
		DrawAxis();
    isDraw=1;
		}
if(	stopbutton==0)//stopbutton����ͣ�İ�ť������ȥ�Ժ�Ͳ���ʾ������
{

//��Ч��ֱ�߶����ű���
float vertical_scale=vertical/3.3f;
//��Чˮƽ������ű���
float horizontal_scale=horizontal/3.0f;
 
  //�����ʷ����
  for(int i=0;i<WAVE_XMAX;i++)
	{
    i_limite = (i+1) < WAVE_XMAX ? (i+1) : WAVE_XMAX-1;
   LCD_DrawLine(i, p_old[i], i+1, p_old[i_limite], WHITE);
  }

  ShowGap(height_limit, width_limit);//��ʾ��λ





  //�����²���
    for(int i=0;i<WAVE_XMAX;i++)
	{
    if(i)
		{
			i_limite = (i+1) < WAVE_XMAX ? (i+1) : WAVE_XMAX-1;
      //����14��ԭ���ǣ��ܹ�p_draw��3080�����ݣ���ÿ����֮��ļ����14�����ݣ�����Ի���3080/14=220����
      int j = i * 14 * horizontal_scale;//ǿ������ת��
      int k = i_limite * 14 * horizontal_scale;//ǿ������ת��
      LCD_DrawLine(i, WAVE_XMAX-p_draw[j]*15/(256*vertical_scale), i+1,WAVE_XMAX-p_draw[k]*15/(256*vertical_scale),RED);
	   p_old[i]=WAVE_XMAX-p_draw[j]*15/(256*vertical_scale);

		}
		else
		{
			
      int k1 = 14 * horizontal_scale;//ǿ������ת��
			LCD_DrawLine(0, WAVE_XMAX-p_draw[0]*15/(256*vertical_scale), 1,WAVE_XMAX-p_draw[k1]*15/(256*vertical_scale), RED);

       p_old[0]=WAVE_XMAX-p_draw[0]*15/(256*vertical_scale);
		}

    //delay��Ŀ����ʵ�ִ����һ�����ͼ��Ч��
   // delay_ms(1);


	}

}
	
}



//  ������ַ
void ChangeWave1Address(void)
{
	uint16_t *p_temp;
	p_temp = p_save_wave1;
	p_save_wave1 = p_draw_wave1;
	p_draw_wave1 = p_temp;
}



// �ɼ���ɻص�����
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if(hadc == &hadc1)
    {
        if(AdcConvEnd == 0)
        {
            AdcConvEnd = 1;
            ChangeWave1Address(); // ��ַ�л�
        }
        // ������һ�ε�����
				memcpy(p_save_wave1,p_adc_wave1,sizeof(wave1_data2));
        // ���µĴ����ַ��ʼ����
        HAL_ADC_Start_DMA(&hadc1, (uint32_t *)p_adc_wave1, ADC_LENGTH);
    }
}

/**��ʾ��ֱ��ˮƽ��λ
***vertical ��ֱ��λ��3.3 6.6 ����ֱ�����������3.3v��6.6v
***horizontal ˮƽ��λ��WAVE_KHZ(20)����һ����Ļ��ʾ20k���ε���������
**/
void ShowGap(float vertical, float horizontal)
{
  	float showedvertical=vertical;

	LCD_ShowFloatNum1(WAVE_XMAX-20, WAVE_YMAX-41*5, showedvertical, 3, WHITE, BLACK, 16);

	float showhorizontal = horizontal;
 
  LCD_ShowFloatNum1(WAVE_XMAX-31, WAVE_YMAX-10, showhorizontal*4/5, 3, WHITE, BLACK, 16);
 
  LCD_ShowFloatNum1(WAVE_XMAX-41*3+10, WAVE_YMAX-10, showhorizontal*2/5,3, WHITE, BLACK, 16);
    LCD_ShowFloatNum1(WAVE_XMAX-41*5+10, WAVE_YMAX-10, 0,3, WHITE, BLACK, 16);
}


//����
void WaveClear(void)
{
	LCD_Fill(0,0,240,240,WHITE);
	LCD_Fill(0,0,240,240,WHITE);
}

//����������
void DrawAxis(void)
{
	int i=0;
	for(i=0;i<WAVE_XMAX+10;i+=41)
	{
		LCD_DrawLine(i, WAVE_YMAX+1, i, WAVE_YMAX+10, BLACK);
	}
	for(i=0;i<WAVE_YMAX+10;i+=41)
	{
		LCD_DrawLine(WAVE_XMAX+1, i, WAVE_XMAX+10, i, BLACK);
	}
	LCD_DrawLine(0, WAVE_YMAX+10, WAVE_XMAX+10, WAVE_YMAX+10, BLACK);
	
	LCD_DrawLine(WAVE_XMAX+10, 0, WAVE_XMAX+10, WAVE_YMAX+10, BLACK);
}


// ͨ�� MQTT ���Ͷ�̬����ĺ���
void sendDynamicArrayOverMQTT() {
    
    //printf("AT+TCMQTTPUB =\"$thing/up/property/XTRLYBWVYB/L610\",1,\"{\\\"method\\\":\\\"report\\\",\\\"clientToken\\\":\\\"123\\\",\\\"params\\\":{\\\"c1\\\":[1,1,1,1,6,6,6,6,6,6]}}\"\r\n");
    
    // Construct the MQTT message with the dynamic array values
    printf("AT+TCMQTTPUB=\"$thing/up/property/XTRLYBWVYB/L610\",1,\"{\\\"method\\\":\\\"report\\\",\\\"clientToken\\\":\\\"123\\\",\\\"params\\\":{\\\"c1\\\":[");

    for (int i = 0; i < ADC_LENGTH/10; i++) {
        printf("%d", send_data[i]);
        if (i <ADC_LENGTH/10-1) {
            printf(",");
        }
    }
    printf("]}}\"\r\n");


	 	Clear_Buffer();





}













//�õ�����
void GetHeartRate(uint16_t *p_data)
{
  //��1.5vΪ�����㣬Ѱ�������ش�����
	uint16_t triggerPoint = FindTriggerPoint(1.5, 1, p_data+100);
  //��1.5vΪ�����㣬Ѱ���½��ش�����
	uint16_t downPoint = FindTriggerPoint(1.5,0,100+1+p_data+triggerPoint);
  
	uint16_t sectriggerPoint=FindTriggerPoint(1.5,1,100+2+p_data+downPoint+triggerPoint);
	uint32_t x_diff=sectriggerPoint+downPoint+2 ;
	float divider = 1000.0 ;
	float freq=60.0*divider/x_diff;//�õ�����
 
 	
  
  //printf("AT+TCMQTTPUB=\"$thing/up/property/XTRLYBWVYB/L610\",1,\"{\\\"method\\\":\\\"report\\\",\\\"clientToken\\\":\\\"123\\\",\\\"params\\\":{\\\"BPM_in_cloud\\\":1}}\"\r\n");


	LCD_ShowFloatNum1(40, 20, freq, 5, GRAYBLUE, WHITE, 32);
  if(freq>120){
    printf("AT+GTTS=1,\"���ʹ���\",1\r\n");
    delay_ms(100);
    
}
 if(freq<60){
    printf("AT+GTTS=1,\"���ʹ���\",1\r\n");
    delay_ms(100);
    
}


	 printf("AT+TCMQTTPUB=\"$thing/up/property/XTRLYBWVYB/L610\",1,\"{\\\"method\\\":\\\"report\\\",\\\"clientToken\\\":\\\"123\\\",\\\"params\\\":{\\\"BPM_in_cloud\\\":%f}}\"\r\n",freq);

Clear_Buffer();
	
	
	
	LCD_ShowString(120, 20, (uint8_t *)"BPM", GRAYBLUE, WHITE,32, 0);//BPM�����ʵĵ�λ
 
}

























//����׼��Ѱ�Ҵ����㷽�����������ݴ���
uint16_t FindTriggerPointchen(float triggerV, uint8_t isRise, uint16_t *p_data)
{
  uint16_t triggerADC = triggerV/3.3f * ADC_RANGE;
	uint16_t i;
	int8_t ensure = 2;
	int8_t j;
	uint16_t right = 0;
	uint16_t tolerance =20;//�ݴ���
	for(i=ensure+1;i<ADC_LENGTH-1-WAVE_XMAX;i++)
	{
		if(isRise)
		{
			if( (triggerADC<p_data[i])&&p_data[i]<=triggerADC+tolerance)
			{
				for(j=-ensure;j<=ensure;j+=2)
				{
					if(p_data[i+j]>p_data[i+j-1]&&p_data[i+j]<=p_data[i+j+1])
					{
						++right;
					}
					
				}
				if(right==ensure+1)
				{
					return i;
				}
			}
		}
		else
		{
			for(j=-ensure;j<=ensure;j+=2)
			{
				if(p_data[i+j]<p_data[i+j-1]&&p_data[i+j]>=p_data[i+j+1])
				{
					++right;
				}
				
			}
			if(right==ensure+1)
			{
				return i;
			}
		}
	}
	return 1;
}


uint16_t FindTriggerPoint(float triggerV, uint8_t isRise, uint16_t *p_data)
{
    uint16_t triggerADC = triggerV/3.3f * ADC_RANGE;
    uint16_t i;
    for(i=1;i<ADC_LENGTH-1-WAVE_XMAX;i++)
    {
        if(isRise)
        {
            if( (triggerADC<p_data[i]) && (p_data[i-1]<p_data[i]) && (p_data[i]<=p_data[i+1]) )
            {
                return i;
            }
        }
        else
        {
            if( (triggerADC<p_data[i]) && (p_data[i-1]>p_data[i]) && (p_data[i]>=p_data[i+1]) )
            {
                return i;
            }
        }
    }
    return 1;
}


/*
������:arm_iir_f32_bs
����˵��:���ú���arm_iir_f32_bsʵ�ִ����˲���
����ֵ:��
*/
static void arm_iir_f32_bs(void)
{
  uint32_t i;
  arm_biquad_casd_df1_inst_f32 S;
 

  /* ��ʼ�� */
 
arm_biquad_cascade_df1_init_f32(&S, numStages, (float32_t *)&IIRCoeffs32BS[0], (float32_t *)&IIRStateF32[0]);
  /* IIR�˲� */
  arm_biquad_cascade_df1_f32(&S, testInput_f32_50Hz_200Hz, testOutput, TEST_LENGTH_SAMPLES); 

}














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
