/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <string.h>
#include <stdio.h>
#include "ssd1306.h"
#include "ssd1306_tests.h"
#include "ssd1306_fonts.h"

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

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

RTC_HandleTypeDef hrtc;

TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_RTC_Init(void);
static void MX_TIM2_Init(void);
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
  MX_I2C1_Init();
  MX_RTC_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  GPIO_PinState previous = GPIO_PIN_RESET; // pull
  GPIO_PinState b_previous = GPIO_PIN_RESET;
  GPIO_PinState timer_previous = GPIO_PIN_RESET;
  GPIO_PinState stop_previous = GPIO_PIN_RESET;
  GPIO_PinState stop = GPIO_PIN_RESET;
  uint8_t count =0;
  uint8_t choose =0;
  static uint8_t timer_intro_done = 0;
  uint8_t sec =0;
  uint32_t min =0;
  uint32_t hour=0;
  uint32_t total_secs=0;
  uint32_t updated_GetTick=0;
  uint8_t timer_running =0;
  uint32_t tick =0; // for HALget tick
  uint32_t total =0;// sec
  uint32_t hh =  0; // hours from min
  uint32_t mm =  0; // min from sec
  uint32_t ss =  0;// sec fpr range
  uint8_t lap_hh = 0; // laps
  uint8_t lap_mm = 0; //laps
  uint8_t lap_ss = 0; // laps
  char TIME[30];
  char year[30];
  RTC_TimeTypeDef sTime = {0};   // declare this outside the loop
  RTC_DateTypeDef sDate = {0};
  HAL_TIM_Base_Start_IT(&htim2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  GPIO_PinState button_1 = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8);
	  GPIO_PinState button_2 = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9);
	  GPIO_PinState button_3 = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10);
	  GPIO_PinState button_6 = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11);
	  GPIO_PinState button_4 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14);
	  GPIO_PinState button_5 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15);
	  if(button_1 != previous){
		  if (button_1 == GPIO_PIN_RESET) {   // ← only count the press edge
			  choose =(choose +1)%3;
		  }
		  previous = button_1;
		  ssd1306_Fill(Black);
		  snprintf(TIME, sizeof(TIME), "%u", choose);
		  ssd1306_SetCursor(0, 0);
		  ssd1306_WriteString(TIME, Font_7x10, White);
	  }
	  if(button_3 !=  b_previous){
		  if (button_3 == GPIO_PIN_RESET) {   // ← only count the press edge
			  count =(count +1)%4;
		  }
		  b_previous = button_3;
		  ssd1306_Fill(Black);
		  snprintf(TIME, sizeof(TIME), "%u", count);
		  ssd1306_SetCursor(0, 0);
		  ssd1306_WriteString(TIME, Font_7x10, White);
	  }
	  if(choose == 0){
		  ssd1306_Fill(Black);
		  ssd1306_SetCursor(40, 20);
     	  ssd1306_WriteString("time", Font_11x18, White);
     	  ssd1306_SetCursor(0, 20);
     	  ssd1306_WriteString("<", Font_11x18, White);
     	  ssd1306_SetCursor(115, 20);
     	  ssd1306_WriteString(">", Font_11x18, White);
     	  ssd1306_UpdateScreen();
     	  HAL_Delay(50);
     	  if(button_2 == GPIO_PIN_RESET){
     		 choose = 2;
     		 HAL_Delay(50);
     	  }
     	  else if( button_6  == GPIO_PIN_RESET){
     		  HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
     		  HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
              snprintf(year,sizeof(year),"%04d-%02d-%02d",sDate.Year + 2000, sDate.Month, sDate.Date);
              snprintf(TIME,sizeof(TIME),"%02d:%02d:%02d",sTime.Hours, sTime.Minutes, sTime.Seconds);
              ssd1306_Fill(Black);
			  ssd1306_SetCursor(1, 0);
			  ssd1306_WriteString("Date:", Font_7x10, White);
			  ssd1306_SetCursor(1, 30);
			  ssd1306_WriteString("Time:", Font_7x10, White);
              ssd1306_SetCursor(1, 40);
              ssd1306_WriteString(TIME, Font_11x18, White);
              ssd1306_SetCursor(1, 10);
     		  ssd1306_WriteString(year, Font_11x18, White);
     		  ssd1306_UpdateScreen();
     		  HAL_Delay(2000);
     	  }
	  }
	  else if(choose == 1){
		  ssd1306_Fill(Black);
		  ssd1306_SetCursor(35, 20);
		  ssd1306_WriteString("timer", Font_11x18, White);
		  ssd1306_SetCursor(0, 20);
		  ssd1306_WriteString("<", Font_11x18, White);
    	  ssd1306_SetCursor(115, 20);
		  ssd1306_WriteString(">", Font_11x18, White);
	 	  if(button_2  == GPIO_PIN_RESET){
	 		  choose = 0;
	 		  HAL_Delay(50);
	 	  }
	 	  else if(count == 1){
	 		  if (!timer_intro_done)
	 		  {
	 			  ssd1306_Fill(Black);
	 		      ssd1306_SetCursor(24, 20);
	 		      ssd1306_WriteString("00:", Font_11x18, White);
	 		      ssd1306_SetCursor(54, 20);
	 		      ssd1306_WriteString("00:", Font_11x18, White);
	 		      ssd1306_SetCursor(85, 20);
	 		      ssd1306_WriteString("00", Font_11x18, White);
	 		      ssd1306_UpdateScreen();
	 		      HAL_Delay(1000);                 // hold 1.5 s (blocking is OK here, once)
	 		      timer_intro_done = 1;            // never do this again
	 		    }
	 		  if(button_4 == GPIO_PIN_RESET){
	 			  sec = (sec+1)%60;
	 		  }
	 		  else if(button_5 == GPIO_PIN_RESET){
	 			 sec  = (sec >= 1) ? sec - 1 : 0;   // 0 → 60, else subtract (ternary operation)
	 		  }
	 		  ssd1306_Fill(Black);
	 		  ssd1306_SetCursor(85, 40);
	 		  ssd1306_WriteString("^", Font_11x18, White);
			  ssd1306_SetCursor(78, 20);                      // using after the loop is best way to print updated variables
		 	  ssd1306_WriteString(":", Font_11x18, White);
		 	  ssd1306_SetCursor(46, 20);
		 	  ssd1306_WriteString(":", Font_11x18, White);
	 		  ssd1306_SetCursor(75, 50);
	 		  ssd1306_WriteString("SECONDS", Font_7x10, White);
	 		  snprintf(TIME, sizeof(TIME), "%u", sec);
	 		  ssd1306_SetCursor(85, 20);
	 		  ssd1306_WriteString(TIME, Font_11x18, White);
	 		  snprintf(TIME, sizeof(TIME), "%lu", min);
	 		  ssd1306_SetCursor(54, 20);
	 		  ssd1306_WriteString(TIME, Font_11x18, White);
	 		  snprintf(TIME, sizeof(TIME), "%lu", hour);
	 		  ssd1306_SetCursor(24, 20);
	 		  ssd1306_WriteString(TIME, Font_11x18, White);
	 	  }
	 	  else if(count  ==2){
	 		  if(button_4 == GPIO_PIN_RESET){
	 			 min = (min+1)%60;
	 		  }
	 		  else if(button_5 == GPIO_PIN_RESET){
	 			 min  = (min > 0) ? min  - 1 : 0;
	 		  }
	 		 ssd1306_Fill(Black);
	 		 ssd1306_SetCursor(54, 40);
	 		 ssd1306_WriteString("^", Font_11x18, White);
	 		 ssd1306_SetCursor(46, 20);
	 		 ssd1306_WriteString(":", Font_11x18, White);
	 		 ssd1306_SetCursor(78, 20);
	 		 ssd1306_WriteString(":", Font_11x18, White);
	 		 ssd1306_SetCursor(40, 50);
	 		 ssd1306_WriteString("MINUTES", Font_7x10, White);
	 		 snprintf(TIME, sizeof(TIME), "%u", sec);
	 		 ssd1306_SetCursor(85,20);
	 		 ssd1306_WriteString(TIME, Font_11x18, White);
	 		 snprintf(TIME, sizeof(TIME), "%lu", min);
	 		 ssd1306_SetCursor(54, 20);
	 		 ssd1306_WriteString(TIME, Font_11x18, White);
	 		 snprintf(TIME, sizeof(TIME), "%lu", hour);
	 	     ssd1306_SetCursor(24, 20);
	 		 ssd1306_WriteString(TIME, Font_11x18, White);
	 	  }
	 	  else if(count  ==3){
	 		  if(button_4 == GPIO_PIN_RESET){
	 			 hour = (hour+1)%24;
	 		  }
	 		  else if(button_5 == GPIO_PIN_RESET){
	 			 hour = (hour > 0) ? hour - 1 : 0;
	 		  }
	 		  ssd1306_Fill(Black);
		 	  ssd1306_SetCursor(24, 40);
		 	  ssd1306_WriteString("^", Font_11x18, White);
		      ssd1306_SetCursor(78, 20);
		 	  ssd1306_WriteString(":", Font_11x18, White);
		 	  ssd1306_SetCursor(46, 20);
		 	  ssd1306_WriteString(":", Font_11x18, White);
		 	  ssd1306_SetCursor(12, 50);
		 	  ssd1306_WriteString("HOURS", Font_7x10, White);
		 	  snprintf(TIME, sizeof(TIME), "%u", sec);
		 	  ssd1306_SetCursor(85, 20);
		 	  ssd1306_WriteString(TIME, Font_11x18, White);
		 	  snprintf(TIME, sizeof(TIME), "%lu", min);
		 	  ssd1306_SetCursor(54, 20);
		 	  ssd1306_WriteString(TIME, Font_11x18, White);
		 	  snprintf(TIME, sizeof(TIME), "%lu", hour);
		 	  ssd1306_SetCursor(24, 20);
		 	  ssd1306_WriteString(TIME, Font_11x18, White);
	 	  }
	 	  if (button_6 != timer_previous) {
	 		  if (button_6 == GPIO_PIN_RESET) {
	 			  total_secs  = (uint32_t)hour * 3600u + (uint32_t)min  * 60u + (uint8_t)sec;
	 			  if (total_secs > 0) {
	 				  updated_GetTick = HAL_GetTick() + (total_secs * 1000u);
	 				  timer_running = 1;
	 			  }
	 		  }
	 		  timer_previous = button_6;
	 	  }
	 	  if(timer_running==1){
	 		  if(HAL_GetTick() >= updated_GetTick){
	 			  timer_running = 0;
	 		      hour = min = sec = 0;
	              ssd1306_Fill(Black);
	              ssd1306_SetCursor(20, 20);
	 		      ssd1306_WriteString("timer completed", Font_11x18, White);
	 		      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, SET);
	 		      HAL_Delay(100);
	 		      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, RESET);
	 		  }
	 	  }
	 	 ssd1306_UpdateScreen();   // ← ONE call per loop
	 	 HAL_Delay(20);
	  }
	  else if(choose == 2){
		  ssd1306_Fill(Black);
		  ssd1306_SetCursor(12, 20);
          ssd1306_WriteString("stopWatch", Font_11x18, White);
          ssd1306_SetCursor(0, 20);
          ssd1306_WriteString("<", Font_11x18, White);
      	  ssd1306_SetCursor(115, 20);
      	  ssd1306_WriteString(">", Font_11x18, White);
          if(button_2 == GPIO_PIN_RESET){
        	  choose = 1;
        	  HAL_Delay(50);
          }
          if (button_4 != stop_previous) {
        	  if(button_4 == GPIO_PIN_RESET){
        		  tick = HAL_GetTick();
        		  ssd1306_Fill(Black);
        		  ssd1306_SetCursor(90,10);
        		  ssd1306_WriteString("RESET", Font_7x10,  White);
        	  }
        	  stop_previous = button_4;
          }
          total = (HAL_GetTick() -  tick)/1000 ;
          ss = total % 60;               //converting millisec to sec 0 to 59
          mm = (total/60) % 60;                    // to min 0 to 59
          hh = (total/3600) % 24;                  // to hours 0 to 23
          snprintf(TIME, sizeof(TIME), "%02lu:%02lu:%02lu", hh, mm, ss);
          ssd1306_SetCursor(20, 40);
          ssd1306_WriteString(TIME, Font_11x18,  White);
	      if(button_5 != stop){
	    	  if(button_5 == GPIO_PIN_RESET){
	    		  lap_hh = hh;
	    		  lap_mm = mm;
	    		  lap_ss = ss;
	    	  }
	    	  stop = button_5;
	    	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, SET);
	    	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, RESET);
	      }
	      snprintf(TIME, sizeof(TIME), "%02u:%02u:%02u", lap_hh, lap_mm, lap_ss);
	      ssd1306_SetCursor(0, 0);    // to see the updated variable on screen keep this outside loop and update variable inside the loop.
	      ssd1306_WriteString(TIME, Font_11x18,  White);
	  }
	  ssd1306_UpdateScreen();   // ← ONE call per loop
	  HAL_Delay(100);
  }
}
  /* USER CODE END 3 */

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef DateToUpdate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 21;
  sTime.Minutes = 57;
  sTime.Seconds = 0;

  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  DateToUpdate.WeekDay = 2;
  DateToUpdate.Month = 10;
  DateToUpdate.Date = 20;
  DateToUpdate.Year = 26;

  if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7199;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin : PB13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB14 PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PA8 PA9 PA10 PA11 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
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
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
