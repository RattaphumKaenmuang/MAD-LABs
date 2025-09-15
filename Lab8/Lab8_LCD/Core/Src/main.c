/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "rng.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <5x5_font.h>
#include <ILI9341_GFX.h>
#include <ILI9341_STM32_Driver.h>
#include <ILI9341_Touchscreen.h>
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MARGIN_X 50
#define MARGIN_Y 30
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
const uint16_t TEXT_SIZE = 2;
const uint16_t BG_COLOR = WHITE;
const uint16_t TEXT_COLOR = BLACK;

extern volatile uint16_t LCD_HEIGHT;
extern volatile uint16_t LCD_WIDTH;

const uint16_t TOP_CIRCLE_RADIUS = 20;
uint16_t topCircleX;
uint16_t topCircleY;

uint16_t redCircleX;
uint16_t redCircleY;
uint16_t greenCircleX;
uint16_t greenCircleY;
uint16_t blueCircleX;
uint16_t blueCircleY;

const uint16_t RGB_CIRCLE_RADIUS = 15;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void drawTopBar(float tempPercentage, uint16_t circleColor, float humidityPercentage);
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

  /* Enable the CPU Cache */

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

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
  MX_SPI5_Init();
  MX_TIM1_Init();
  MX_RNG_Init();
  /* USER CODE BEGIN 2 */
  ILI9341_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
  ILI9341_Fill_Screen(WHITE);
  topCircleX = LCD_WIDTH/2;
  topCircleY = MARGIN_Y + CHAR_HEIGHT * TEXT_SIZE/2;
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  drawTopBar(32.6, 0x6969, 54.1);
	  drawRGBBar(10, 20, 30);
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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 200;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  RCC_OscInitStruct.PLL.PLLR = 2;
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

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void drawTopBar(float tempPercentage, uint16_t circleColor, float humidityPercentage){
    char tempText[10];
    sprintf(tempText, "%.1fC", tempPercentage);

    char humidText[10];
    sprintf(humidText, "%.1f%%RH", humidityPercentage);

    int tempWidth = strlen(tempText) * CHAR_WIDTH * TEXT_SIZE;
    int tempX = MARGIN_X;
    int tempY = MARGIN_Y;

    ILI9341_Draw_Text(tempText, tempX, tempY, TEXT_COLOR, TEXT_SIZE, BG_COLOR);

    int margin = 15;
    topCircleX = tempX + tempWidth + TOP_CIRCLE_RADIUS + margin;

    ILI9341_Draw_Filled_Circle(topCircleX, topCircleY, TOP_CIRCLE_RADIUS, circleColor);

    int humidX = topCircleX + TOP_CIRCLE_RADIUS + margin;
    int humidY = MARGIN_Y;

    ILI9341_Draw_Text(humidText, humidX, humidY, TEXT_COLOR, TEXT_SIZE, BG_COLOR);
}

void drawRGBBar(int redPercentage, int greenPercentage, int bluePercentage){
	char redPercentageText[10];
	char greenPercentageText[10];
	char bluePercentageText[10];

	sprintf(redPercentageText, "%d%%", redPercentage);
	sprintf(greenPercentageText, "%d%%", greenPercentage);
	sprintf(bluePercentageText, "%d%%", bluePercentage);

	const int TOP_MARGIN = 30;
	const int START_Y = topCircleY + TOP_CIRCLE_RADIUS + TOP_MARGIN;
	const int PADDING_X = 10;
	const int PADDING_Y = 20;
	const int BAR_WIDTH = 150;
	const int BAR_HEIGHT = 30;
	const int Y_DIFF = PADDING_Y + BAR_HEIGHT;
	const int BAR_WIDTH_STEP = BAR_WIDTH / 10;

	const uint16_t RED_BACK = (31 << 11) | (31 << 5) | 15;
	const uint16_t GREEN_BACK = (15 << 11) | (63 << 5) | 15;
	const uint16_t BLUE_BACK = (15 << 11) | (31 << 5) | 31;

	redCircleX = MARGIN_X;
	redCircleY = START_Y + RGB_CIRCLE_RADIUS;

	int redBarStartX = redCircleX + RGB_CIRCLE_RADIUS + PADDING_X;
	int redBarStartY = START_Y;
	int redFrontWidth = redPercentage/10 * BAR_WIDTH_STEP;

	greenCircleX = MARGIN_X;
	greenCircleY = redCircleY + Y_DIFF;

	int greenBarStartX = greenCircleX + RGB_CIRCLE_RADIUS + PADDING_X;
	int greenBarStartY = redBarStartY + Y_DIFF;
	int greenFrontWidth = greenPercentage/10 * BAR_WIDTH_STEP;

	blueCircleX = MARGIN_X;
	blueCircleY = greenCircleY + Y_DIFF;

	int blueBarStartX = blueCircleX + RGB_CIRCLE_RADIUS + PADDING_X;
	int blueBarStartY = greenBarStartY + Y_DIFF;
	int blueFrontWidth = bluePercentage/10 * BAR_WIDTH_STEP;

	ILI9341_Draw_Filled_Circle(redCircleX, redCircleY, RGB_CIRCLE_RADIUS, RED);
	ILI9341_Draw_Filled_Rectangle_Coord(redBarStartX, redBarStartY, redBarStartX + BAR_WIDTH, redBarStartY + BAR_HEIGHT, RED_BACK);
	ILI9341_Draw_Filled_Rectangle_Coord(redBarStartX, redBarStartY, redBarStartX + redFrontWidth, redBarStartY + BAR_HEIGHT, RED);

	ILI9341_Draw_Filled_Circle(greenCircleX, greenCircleY, RGB_CIRCLE_RADIUS, GREEN);
	ILI9341_Draw_Filled_Rectangle_Coord(greenBarStartX, greenBarStartY, greenBarStartX + BAR_WIDTH, greenBarStartY + BAR_HEIGHT, GREEN_BACK);
	ILI9341_Draw_Filled_Rectangle_Coord(greenBarStartX, greenBarStartY, greenBarStartX + greenFrontWidth, greenBarStartY + BAR_HEIGHT, GREEN);

	ILI9341_Draw_Filled_Circle(blueCircleX, blueCircleY, RGB_CIRCLE_RADIUS, BLUE);
	ILI9341_Draw_Filled_Rectangle_Coord(blueBarStartX, blueBarStartY, blueBarStartX + BAR_WIDTH, blueBarStartY + BAR_HEIGHT, BLUE_BACK);
	ILI9341_Draw_Filled_Rectangle_Coord(blueBarStartX, blueBarStartY, blueBarStartX + blueFrontWidth, blueBarStartY + BAR_HEIGHT, BLUE);
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
