#include "stm32f4xx_hal.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"
#include "lcd_init.h"
#include "lcd.h"
#include "delay.h"
#include "math.h"
#define WAIT1_DOWN 1
#define CONFIRM1_DOWN 2
#define Y_TRAN 3
#define WAIT1_UP 4
#define CONFIRM1_UP 5
#define X_TRAN 6
#define CONFIRM2_DOWN 7
#define CONFIRM2_UP 8
#define WAIT2_DOWN 9
#define WAIT2_UP 10
#define Key1_Low HAL_GPIO_ReadPin  (GPIOG,GPIO_PIN_7)==0 
#define Key1_High HAL_GPIO_ReadPin  (GPIOG,GPIO_PIN_7)==1
#define Key2_Low HAL_GPIO_ReadPin  (GPIOG,GPIO_PIN_6)==0 
#define Key2_High HAL_GPIO_ReadPin  (GPIOG,GPIO_PIN_6)==1
void Key_Run(void);
void Key2_Run(void);
