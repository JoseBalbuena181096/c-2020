/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
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
#include "stm32f4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <math.h>
#define MPU6050_AD 0xd0
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */
/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define RAD_TO_DEG  57.295779
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
extern uint8_t DatoRecibido[15];
extern uint8_t DatoEnviado[2];
float Y=0.0;
float u;
float Kp=40.0;//200
float Ki=0.0;
float Kd=1.0;//2.5
float Ref=0.0;
int atras=0;
int adelante=0;
float x[3]={0,0,0};
void pedirDatosMPU();
void motores(double U);
double A[2]={0};
double AY=0.0,AX=0.0,AZ=0.0,GY=0.0,GX=0.0;
double Acc[2]={0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim3;
/* USER CODE BEGIN EV */
extern TIM_HandleTypeDef htim1;
extern I2C_HandleTypeDef hi2c1;
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
  * @brief This function handles TIM3 global interrupt.
  */
void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */

  /* USER CODE END TIM3_IRQn 0 */
  HAL_TIM_IRQHandler(&htim3);
  /* USER CODE BEGIN TIM3_IRQn 1 */
  pedirDatosMPU();
  Y=A[0];
  x[0]=Ref-Y;
  x[2]=x[0]+x[2];
  u=Kp*x[0]+Ki*(x[2])*0.001+Kd*(x[0]-x[1])/0.001;
  motores(u);
  x[1]=x[0];
  /* USER CODE END TIM3_IRQn 1 */
}

/* USER CODE BEGIN 1 */

void pedirDatosMPU(){
	  DatoEnviado[0]=0x3B;
	  DatoEnviado[1]=0x00;
	  HAL_I2C_Master_Transmit(&hi2c1,(uint16_t)MPU6050_AD,&DatoEnviado[0],1,100);
	  HAL_I2C_Master_Receive(&hi2c1,(uint16_t)MPU6050_AD,DatoRecibido,14,100);
	  AX=(float)(((int16_t)(DatoRecibido[0]<<8|DatoRecibido[1]))/(float)1638.0);
	  AY=(float)(((int16_t)(DatoRecibido[2]<<8|DatoRecibido[3]))/(float)1638.0);
	  AZ=(float)(((int16_t)(DatoRecibido[4]<<8|DatoRecibido[5]))/(float)1638.0);
	  GY=(float)(((int16_t)(DatoRecibido[10]<<8|DatoRecibido[11]))/(float)131.0);
	  GX=(float)(((int16_t)(DatoRecibido[8]<<8|DatoRecibido[9]))/(float)131.0);
	  Acc[1] = atan(((0-1)*AX)/sqrt(pow(AY,2) + pow(AZ,2)))*RAD_TO_DEG;
	  Acc[0] = atan(AY/sqrt(pow(AX,2) + pow(AZ,2)))*RAD_TO_DEG;//AY
	  A[0] = 0.98 *(A[0]+GX*0.001) + 0.02*Acc[0];//AC
	  A[1] = 0.98 *(A[1]+GY*0.001) + 0.02*Acc[1];
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
/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
