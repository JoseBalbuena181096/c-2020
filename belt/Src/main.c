/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define ONE 16724174
#define TWO 16718054
#define ZERO 16756814
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint32_t  count=0, input_capture=0,input_diference=0,input_last=0,data=0;
uint8_t sample=0;
enum {START,DATA,DATA_FULL}stateIR=START;
enum {OFF,ON,HIGHER,LOWER}stateBelt=OFF;
enum {FIRST,SECOND}step=FIRST;
enum {HIGHER_5,LOWER_5,STOP_5}motorSpeed5=STOP_5;
enum {HIGHER_24,LOWER_24,STOP_24}motorSpeed24=STOP_24;
unsigned long time_counter=0,previusTime=0,previusTime1=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int ReadData();
void lower_belt();
void higher_belt();
void motor24(int velocity);
void motor5(int velocity);
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
  MX_TIM2_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
  HAL_TIM_Base_Start_IT(&htim3);
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
  __HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_2,0);
  __HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_3,0);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  /*
	  if(ReadData()){
		  if((data==ONE||data==TWO)&& (stateBelt==OFF)){
			  stateBelt=ON;
			  HAL_GPIO_WritePin(LED_STOP_GPIO_Port,LED_STOP_Pin,GPIO_PIN_RESET);
			  previusTime=time_counter;
		  }
		  else if(data==ZERO){
			  motor24(0);
			  motor5(0);
			  motorSpeed5=STOP_5;
			  motorSpeed24=STOP_24;
			  HAL_GPIO_WritePin(LED_STOP_GPIO_Port,LED_STOP_Pin,GPIO_PIN_SET);
			  stateBelt=OFF;
		  }
	  }
	  */
	  if(HAL_GPIO_ReadPin(SW0_GPIO_Port, SW0_Pin)==0){
		  data=ZERO;
	  }
	  else if(HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin)==0){
		  data=ONE;
	  }
	  else if(HAL_GPIO_ReadPin(SW2_GPIO_Port, SW2_Pin)==0){
		  data=TWO;
	  }
	  if((data==ONE||data==TWO)&& (stateBelt==OFF)){
		  stateBelt=ON;
		  HAL_GPIO_WritePin(LED_STOP_GPIO_Port,LED_STOP_Pin,GPIO_PIN_RESET);
		  previusTime=time_counter;
	  }
	  else if(data==ZERO){
		  motor24(0);
		  motor5(0);
		  motorSpeed5=STOP_5;
		  motorSpeed24=STOP_24;
		  HAL_GPIO_WritePin(LED_STOP_GPIO_Port,LED_STOP_Pin,GPIO_PIN_SET);
		  stateBelt=OFF;
	  }
	  /////////////////////////////////////////
	  if(stateBelt==ON && data==ONE){
		  stateBelt=LOWER;
	  }
	  else if(stateBelt==ON && data==TWO){
		  stateBelt=HIGHER;
	  }
	  if(stateBelt==LOWER){
		  lower_belt();
	  }
	  else if(stateBelt==HIGHER){
		  higher_belt();
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
int ReadData(){
	count = __HAL_TIM_GetCounter(&htim2);
	  if(stateIR==DATA_FULL){
		  HAL_Delay(150);
		  stateIR=START;
		  return 1;
	  }
	 return 0;
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
 if (htim->Instance==TIM2)
  {
	 if(stateIR==START){
		 //Restart counter
		 __HAL_TIM_SetCounter(&htim2, 0);
		 //Clear variables
	 	 input_capture=0;
	 	 input_last=0;
	 	 input_diference=0;
	 	 sample=0;
	 	 //start protocol
	 	stateIR=DATA;
	 }
	 else if(stateIR==DATA)
	 {
		 //Read Timer
		 input_capture= __HAL_TIM_GetCompare(&htim2, TIM_CHANNEL_1);    //read TIM2 channel 1 capture value
		 //get difference between first pulse to second pulse
		 input_diference=input_capture-input_last;
		 //if the logic bit 1 occurs
		 if(input_diference>210 && input_diference<230){//225
			 data |= (1UL << (31-sample));   // write 1
		 	 sample++;//increase sample
	 	 }
		 //if the logic bit 0 occurs
		 else if(input_diference>108 && input_diference<116){//112
			 data &= ~(1UL << (31-sample));//write 0
			 sample++;//increase sample
		 }
		 if(sample==31){//if sample is 31 data is full
			 stateIR=DATA_FULL;
		 }
		 input_last=input_capture;
      }
   }
}

void lower_belt(){
	if(step==FIRST){
		motor5(750);
		motor24(0);
		motorSpeed5=LOWER_5;
		motorSpeed24=STOP_24;
	}
	if((time_counter-previusTime>=7)&& step==FIRST){
		step=SECOND;
		previusTime = time_counter;
	}
	if(step==SECOND){
		motor5(0);
		motor24(900);
		motorSpeed5=STOP_5;
		motorSpeed24=LOWER_24;
	}
	if((time_counter-previusTime>=5) && step==SECOND){
		step=FIRST;
		previusTime=time_counter;
	}
}
void higher_belt(){
	if(step==FIRST){
			motor5(999);
			motor24(0);
			motorSpeed5=HIGHER_5;
			motorSpeed24=STOP_24;
		}
		if((time_counter-previusTime>=7)&& step==FIRST){
			step=SECOND;
			previusTime = time_counter;
		}
		if(step==SECOND){
			motor5(0);
			motor24(999);
			motorSpeed5=STOP_5;
			motorSpeed24=HIGHER_24;
		}
		if((time_counter-previusTime>=5) && step==SECOND){
			step=FIRST;
			previusTime=time_counter;
		}
}
void motor24(int velocity){
	 HAL_GPIO_WritePin(GPIOE,D2_1_Pin,GPIO_PIN_RESET);
	 HAL_GPIO_WritePin(GPIOE,D2_2_Pin,GPIO_PIN_SET);
	  __HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_3,velocity);
}
void motor5(int velocity){
	 HAL_GPIO_WritePin(GPIOE,D1_1_Pin,GPIO_PIN_SET);
	 HAL_GPIO_WritePin(GPIOE,D1_2_Pin,GPIO_PIN_RESET);
	__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_2,velocity);
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
