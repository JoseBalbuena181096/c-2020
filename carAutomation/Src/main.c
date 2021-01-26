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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include<stdio.h>
#include<string.h>
//#include"PID.cpp"
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
char buffer[30];
uint8_t dato[1];
enum {start,reading,move,stop}state=stop;
float init_distance_mts=0,now_distance_mts=0;
int now_distance_cm=0,init_distance_cm=0;
float time_now_sec=0;
int speed_ms=0;
unsigned long last_time = 0;
int percentage = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void delay (uint32_t us)
{
	__HAL_TIM_SET_COUNTER(&htim3, 0);
	while ((__HAL_TIM_GET_COUNTER(&htim3))<us);
}

/************** PIN DEFINITION **************/
int distance_object;

unsigned int hcsr04_read (void)
{
	int distance = 0;
	uint32_t local_time=0;
	HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_RESET);  // pull the TRIG pin HIGH
	delay(2);  // wait for 2 us
	HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_SET);  // pull the TRIG pin HIGH
	delay(10);  // wait for 10 us
	HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_RESET);  // pull the TRIG pin low
	// read the time for which the pin is high
	__HAL_TIM_SET_COUNTER(&htim3, 0);
	while (!(HAL_GPIO_ReadPin(ECHO_GPIO_Port, ECHO_Pin))){  // wait for the ECHO pin to go high
		if( __HAL_TIM_GET_COUNTER(&htim3)>=65534)
				return 0;
	}
	__HAL_TIM_SET_COUNTER(&htim3, 0);
	while (HAL_GPIO_ReadPin(ECHO_GPIO_Port, ECHO_Pin))    // while the pin is high
	 {
		if( __HAL_TIM_GET_COUNTER(&htim3)>=65534)
				return 0;
	 }
	local_time = __HAL_TIM_GET_COUNTER(&htim3);
	distance  = local_time * 0.034/2;
	return distance;
}

/* USER CODE BEGIN 0 */
void Motor_R (signed int speed);
///Function to set speed of left motor///
void Motor_L (signed int speed);
/* USER CODE END 0 */

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
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_USART2_UART_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start(&htim3);
  HAL_TIM_Base_Start(&htim2);
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	 while(HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin) && state==stop){
		  if(!HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin)){
			  state = start;
		  }
		  now_distance_cm = hcsr04_read();
		  sprintf(buffer,"\t%d,%d,%d\n",now_distance_cm,speed_ms,percentage);
		  HAL_UART_Transmit(&huart2,(uint8_t*)buffer,(uint16_t)strlen(buffer),(uint32_t)200);
	  }
	  if(state==start){
		  __HAL_TIM_SET_COUNTER(&htim2, 0);
		  state = reading;
	  }
	  else if(state==reading){
		  init_distance_cm = hcsr04_read();
		  init_distance_mts= init_distance_cm/100.0;
		  __HAL_TIM_SET_COUNTER(&htim2, 0);
		  state = move;
	  }
	  else if(state==move){
		  now_distance_cm = hcsr04_read();
		  now_distance_mts = hcsr04_read()/100.0;
		  time_now_sec = (float)( __HAL_TIM_GET_COUNTER(&htim2))/1000.0;
		  speed_ms = (int)((init_distance_cm-now_distance_cm)/time_now_sec);
		  if(now_distance_cm>=(init_distance_cm*0.7) && now_distance_cm<(init_distance_cm*1.1)){
			  Motor_L (400);
			  Motor_R (400);
			  percentage = 100;
			  sprintf(buffer,"\t%d,%d,%d\n",now_distance_cm,speed_ms,percentage);
			  HAL_UART_Transmit(&huart2,(uint8_t*)buffer,(uint16_t)strlen(buffer),(uint32_t)200);
		  }
		  else if(now_distance_cm>=(init_distance_cm*0.5) && now_distance_cm<(init_distance_cm*0.7)){
			  Motor_L (400*0.7);
			  Motor_R (400*0.7);
			  percentage = 70;
			  sprintf(buffer,"\t%d,%d,%d\n",now_distance_cm,speed_ms,percentage);
			  HAL_UART_Transmit(&huart2,(uint8_t*)buffer,(uint16_t)strlen(buffer),(uint32_t)200);
		  }
		  else if(now_distance_cm>=(init_distance_cm*0.3) && now_distance_cm<(init_distance_cm*0.5)){
			  Motor_L (400*0.5);
			  Motor_R (400*0.5);
			  percentage = 50;
			  sprintf(buffer,"\t%d,%d,%d\n",now_distance_cm,speed_ms,percentage);
			  HAL_UART_Transmit(&huart2,(uint8_t*)buffer,(uint16_t)strlen(buffer),(uint32_t)200);
		  }
		  else if(now_distance_cm>=(init_distance_cm*0.1) && now_distance_cm<(init_distance_cm*0.3)){
			  Motor_L (400*0.42);
			  Motor_R (400*0.42);
			  percentage = 42;
			  sprintf(buffer,"\t%d,%d,%d\n",now_distance_cm,speed_ms,percentage);
			  HAL_UART_Transmit(&huart2,(uint8_t*)buffer,(uint16_t)strlen(buffer),(uint32_t)200);

		  }
		  else if(now_distance_cm>=(6) && now_distance_cm<(init_distance_cm*0.1)){
			  Motor_L (400*0.36);
			  Motor_R (400*0.36);
			  percentage = 36;
			  sprintf(buffer,"\t%d,%d,%d\n",now_distance_cm,speed_ms,percentage);
			  HAL_UART_Transmit(&huart2,(uint8_t*)buffer,(uint16_t)strlen(buffer),(uint32_t)200);
		  }
		  else if(now_distance_cm>=(0) && now_distance_cm<(6)){
			  Motor_L (0);
			  Motor_R (0);
			  percentage = 0;
			  speed_ms = 0;
			  sprintf(buffer,"\t%d,%d,%d\n",now_distance_cm,speed_ms,percentage);
			  HAL_UART_Transmit(&huart2,(uint8_t*)buffer,(uint16_t)strlen(buffer),(uint32_t)200);
			  state = stop;
		  }
	  }
	  else if(state == stop){
		  state = start;
	  }
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

void Motor_R (signed int speed)
{
   if(!speed) {
	   __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
   }
   else {
      speed = (speed >= 500) ? 500 : speed;
      if (speed >=1){
      __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,speed);
      HAL_GPIO_WritePin(GPIOE,IN1_Pin,GPIO_PIN_SET);
      HAL_GPIO_WritePin(GPIOE,IN2_Pin,GPIO_PIN_RESET);
      }
      else {
      speed *= (0-1);
      speed = (speed >= 500) ? 500 : speed;
      __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,speed);
      HAL_GPIO_WritePin(GPIOE,IN1_Pin,GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOE,IN2_Pin,GPIO_PIN_SET);
      }
   }
   return;
}

///Function to set speed of left motor///
void Motor_L (signed int speed)
{
   if(!speed) {
	   __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
   }
   else {
      speed = (speed >= 500) ? 500 : speed;
      if (speed >=1){
      __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,speed*0.95);
      HAL_GPIO_WritePin(GPIOE,IN3_Pin,GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOE,IN4_Pin,GPIO_PIN_SET);
      }
      else {
      speed *= (0-1);
      speed = (speed >= 500) ? 500 : speed;
      __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,speed*0.95);
      HAL_GPIO_WritePin(GPIOE,IN3_Pin,GPIO_PIN_SET);
      HAL_GPIO_WritePin(GPIOE,IN4_Pin,GPIO_PIN_RESET);
      }
   }
   return;
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
