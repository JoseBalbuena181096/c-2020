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
#include "adc.h"
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
#define white 1
#define black 0
#define OUT_LINE  100.0
#define MAX_SPEED 460
#define MED_SPEED 215//125
//-----------------------
#define ON 33456254
#define OFF 33441974
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint32_t input_capture = 0;
uint32_t input_diference = 0;
uint32_t input_last = 0;
uint32_t data_ir = 0;
uint8_t sample_ir = 0;
enum {START,DATA,DATA_FULL}state_ir = START;
/***************************************/
enum {CENTER,RIGHT,LEFT} out_state = CENTER;
enum {HOME,STARTING,RUN} state = HOME;
int sensors_qtr[16] = {0};
float error_now = 0.0;
float error_last = 0.0;
int	speed_1 = 0;
int speed_2 = 0;
int U = 0;
float KP = 28.8;//48
float KD = 70.2;//132.0
//float KB = 0.25;//0.15-0.35
int brake = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void ReadDataIR();
int read_adc();
void read_sensors_qtr(int *sensors);
float get_error(int *sensors,int background);
void Motor_L (signed int speed);
void Motor_R(signed int speed);
void turbine(int speed);
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
  MX_ADC1_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
  HAL_GPIO_WritePin(GPIOC, DIS_A_Pin,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, DIS_B_Pin,GPIO_PIN_RESET);
  turbine(12);
  HAL_Delay(8000);
  HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  ReadDataIR();
	  switch (state)
	  {
			case HOME:
				turbine(12);
				Motor_R(0);
				Motor_L(0);
				break;
			case STARTING:
				turbine(12);
				state = RUN;
				HAL_Delay(650);
				break;
			case RUN:
				//Read real error -10 to 10
				error_now =  get_error(sensors_qtr,white);
				//Out line
				if(error_now == OUT_LINE)
				{
					//Out line state machine//
					switch (out_state)
					{
	            		case CENTER:
	            			speed_1 = MED_SPEED;
	                    	speed_2 = MED_SPEED;
	                    	break;
	            		case RIGHT:
	            			speed_1 = MAX_SPEED;
	            			speed_2 = (0-MAX_SPEED);
	            			break;
	            		case LEFT:
	                    	speed_1 = (0-MAX_SPEED);
	                    	speed_2 = MAX_SPEED;
	                    	break;
					}
				}
				//On line
				else
				{
					U  = (int)(KP*error_now + KD*(error_now-error_last));

	        		//brake = (int)(50.0*(1.0-exp(0.0 - KB*fabs(error_now))));
	            	//speed_1  = (MED_SPEED-brake) + U;
                	//speed_2  = (MED_SPEED-brake) - U;

					speed_1  = MED_SPEED + U;
					speed_2  = MED_SPEED - U;
					error_last = error_now;
				}
				Motor_R(speed_2);
				Motor_L(speed_1);
				break;
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
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
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
int read_adc()
{
	int sensor_value = 4095;
	HAL_ADC_Start(&hadc1);
	if(HAL_ADC_PollForConversion(&hadc1,5) == HAL_OK)
		sensor_value  = (int)(HAL_ADC_GetValue(&hadc1));
	//HAL_ADC_Stop(&hadc1);
	return sensor_value;
}
//---------------------------------------------------------------------
void read_sensors_qtr(int *sensors)
{
	for(int i = 0;i<16;i++)
	{
		HAL_GPIO_WritePin(GPIOE, S0_Pin, i & 0x01);
		HAL_GPIO_WritePin(GPIOE, S1_Pin, i & 0x02);
		HAL_GPIO_WritePin(GPIOE, S2_Pin, i & 0x04);
		HAL_GPIO_WritePin(GPIOE, S3_Pin, i & 0x08);
		sensors[i] = read_adc();
	}
}
//---------------------------------------------------------------------
float get_error(int *sensors,int background)
{
	int max;
	int min;
	int threshold;
	int range;
	int bit_sensor[16];
	int sum = 0;
	int weigth[8] = {14,12,10,8,6,4,2,1};
	int errorLeft = 0;
	int errorRight = 0;
	float error = 0.0;
	//Read 8 samples from each sensor
	read_sensors_qtr(sensors);
	max = min = sensors[0];
	for(int i=1;i<16;i++)
	{
		if(sensors[i] > max)
			max = sensors[i];
		if(sensors[i] < min)
			min = sensors[i];
	}
	range = max-min;
	if(range>400)
	{
		threshold = (range/2)+min;
		for(int i=0;i<16;i++)
		{
			if(background)
				bit_sensor[i] = (sensors[i] < threshold) ? 1 : 0;
			else
				bit_sensor[i] = (sensors[i] > threshold) ? 1 : 0;
		}
		for(int i=0;i<8;i++)
		{
			errorLeft += bit_sensor[i]*weigth[i];
			errorRight += bit_sensor[15-i]*weigth[i];
		}
		for(int i=0;i<16;i++)
			sum += bit_sensor[i];
		error = (float)(errorRight-errorLeft)/(float)(sum);
        out_state = ((error <= 1.0)&&(error >=(0-1.0)))  ? CENTER : out_state;
        out_state = ((error > 1.0)&&(error <=14.0))        ?  RIGHT: out_state;
        out_state = ((error <(0-1.0))&&(error >=(0-14.0))) ? LEFT: out_state;
        return error;
	}
	else
		return OUT_LINE;
	return error;
}

//---------------------------------------------------------------------
void Motor_L(signed int speed)
{
	if(!speed)
	{
		HAL_GPIO_WritePin(GPIOC, DIR_A_Pin,GPIO_PIN_RESET);
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,0);

	}
	else
	{
		speed = (speed >= MAX_SPEED) ? MAX_SPEED : speed;
		if (speed >=1)
		{
			HAL_GPIO_WritePin(GPIOC, DIR_A_Pin,GPIO_PIN_RESET);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,speed);
		}
		else
		{
			speed *= (0-1);
			speed = (speed >= MAX_SPEED) ? MAX_SPEED : speed;
			HAL_GPIO_WritePin(GPIOC, DIR_A_Pin,GPIO_PIN_SET);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,speed);
      }
   }
   return;
}
//---------------------------------------------------------------------
void Motor_R(signed int speed)
{
	if(!speed)
	{
		HAL_GPIO_WritePin(GPIOB, DIR_B_Pin,GPIO_PIN_RESET);
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2,0);

	}
	else
	{
		speed = (speed >= MAX_SPEED) ? MAX_SPEED : speed;
		if (speed >=1)
		{
			HAL_GPIO_WritePin(GPIOB, DIR_B_Pin,GPIO_PIN_SET);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2,speed);
		}
		else
		{
			speed *= (0-1);
			speed = (speed >= MAX_SPEED) ? MAX_SPEED : speed;
			HAL_GPIO_WritePin(GPIOB, DIR_B_Pin,GPIO_PIN_RESET);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2,speed);
      }
   }
   return;
}
//---------------------------------------------------------------------
void turbine(int speed)
{
	if(speed < 0)
		speed = 0;
	else if(speed > 100)
		speed = 100;
	speed += 100;
	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1,speed);
	return;
}
//-------------------------------------------------------------------
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance==TIM2)
	{
		if(state_ir==START)
		{
			//Restart counter
			__HAL_TIM_SetCounter(&htim2,0);
			//Clear variables
			input_capture = 0;
			input_last = 0;
			input_diference = 0;
			sample_ir = 0;
			//start protocol
			state_ir = DATA;
	  }
	  else if(state_ir==DATA)
	  {
		  //Read Timer
		  input_capture = __HAL_TIM_GetCompare(&htim2, TIM_CHANNEL_1);    //read TIM2 channel 1 capture value
		  //get difference between first pulse to second pulse
		  input_diference = input_capture-input_last;
		  //if the logic bit 1 occurs
		  if(input_diference>210 && input_diference<230)//225
		  {
			 data_ir |= (1UL << (31-sample_ir));   // write 1
		 	 sample_ir++;//increase sample
		  }
		  //if the logic bit 0 occurs
		  else if(input_diference>108 && input_diference<116)//112
		  {
			  data_ir &= ~(1UL << (31-sample_ir));//write 0
			  sample_ir++;//increase sample
		  }
		  if(sample_ir==31)//if sample is 31 data is full
			  state_ir = DATA_FULL;
		  input_last = input_capture;
     }
   }
}
//--------------------------------------------------------------------------------------
void ReadDataIR(){
	  if(state_ir==DATA_FULL)
	  {
		  switch (data_ir)
		  {
		     	 case ON:
		     		 state = STARTING;
		      		 break;
		         case OFF:
						 state = HOME;
		        	 break;
		  }
		  HAL_Delay(150);
		  //data_ir = 0;
		  state_ir = START;
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
