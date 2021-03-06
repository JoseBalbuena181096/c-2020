/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include"stdio.h"
#include"string.h"
#include <math.h>
#define MPU6050_AD 0xd0
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t DatoRecibido[15];
uint8_t DatoEnviado[2];
float Y=0.0;
float u;
float Kp=350.0;//200//360//360//360//350//350//350//350//350//350//352
float Ki=0.0;
float Kd=145;//2.5//8//9//10//9.5//13//20//50//115//125//140
float Ref=0.0;
int atras=0;
int adelante=0;
float x[3]={0,0,0};
float pedirDatosMPU();
void motores(double U);
char bufer[30];
int Y_b=0;
uint8_t DatoSerial[1];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void configurarMPU6050();
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
  MX_I2C1_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  configurarMPU6050();
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
  __HAL_UART_ENABLE_IT(&huart2,UART_IT_RXNE);
  //HAL_TIM_Base_Start_IT(&htim3);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  Y=pedirDatosMPU();
	  x[0]=Ref-Y;
	  x[2]=x[0]+x[2];
	  u=Kp*x[0]+Ki*(x[2])+Kd*(x[0]-x[1]);
	  motores(u);
	  x[1]=x[0];
	  //sprintf(bufer,"Y: %i \n",Y_b);
	  //HAL_UART_Transmit(&huart3, (uint8_t*)bufer, (uint16_t)strlen(bufer), (uint32_t)100);
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void configurarMPU6050(){
	   DatoEnviado[0]=0x6B;
	   DatoEnviado[1]=0x00;
	   HAL_I2C_Master_Transmit(&hi2c1,(uint16_t)MPU6050_AD,DatoEnviado,2,100);
	   DatoEnviado[0]=0x19;
	   DatoEnviado[1]=0x07;
	   HAL_I2C_Master_Transmit(&hi2c1,(uint16_t)MPU6050_AD,DatoEnviado,2,100);
	   DatoEnviado[0]=0x19;
	   DatoEnviado[1]=0x07;
	   HAL_I2C_Master_Transmit(&hi2c1,(uint16_t)MPU6050_AD,DatoEnviado,2,100);
}

float pedirDatosMPU(){
	  float AY;
	  DatoEnviado[0]=0x3B;
	  DatoEnviado[1]=0x00;
	  HAL_I2C_Master_Transmit(&hi2c1,(uint16_t)MPU6050_AD,&DatoEnviado[0],1,100);
	  HAL_I2C_Master_Receive(&hi2c1,(uint16_t)MPU6050_AD,DatoRecibido,14,100);
	  AY=(float)(((int16_t)(DatoRecibido[2]<<8|DatoRecibido[3]))/(float)1638);
	  return AY;
}

void motores(double U){
	if (U < -1750){
		U=-1750;
	}
	if (U >1750){
			U=1750;
			}

	atras=0;
	adelante=0;
	if (U>0){
		adelante=U;       // PWM de los motores atras y adelante (las dos ruedas funcionan al tiempo)
		}
	if (U<0){
		atras =fabs(U);
		}
	if (adelante != 0){
	     if (atras == 0 ){
	    	 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,adelante);
		 	 HAL_GPIO_WritePin(GPIOA,IN1_Pin,GPIO_PIN_SET);	///MANDAR UNO AL PIN IN1 DEL PUENTE H
		 	 HAL_GPIO_WritePin(GPIOA,IN2_Pin,GPIO_PIN_RESET);
		 	 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,adelante);
		 	 HAL_GPIO_WritePin(GPIOC,IN3_Pin,GPIO_PIN_RESET);	///MANDAR UNO AL PIN IN1 DEL PUENTE H
		 	 HAL_GPIO_WritePin(GPIOC,IN4_Pin,GPIO_PIN_SET);
	     }
	  }

	if (atras != 0){
		if (adelante == 0){
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,atras);
		 	HAL_GPIO_WritePin(GPIOA,IN1_Pin,GPIO_PIN_RESET);	///MANDAR CERO AL PIN IN1 DEL PUENTE H
		 	HAL_GPIO_WritePin(GPIOA,IN2_Pin,GPIO_PIN_SET);
		    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,atras);
		    HAL_GPIO_WritePin(GPIOC,IN3_Pin,GPIO_PIN_SET);	///MANDAR CERO AL PIN IN1 DEL PUENTE H
		 	HAL_GPIO_WritePin(GPIOC,IN4_Pin,GPIO_PIN_RESET);
				}
			}
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
