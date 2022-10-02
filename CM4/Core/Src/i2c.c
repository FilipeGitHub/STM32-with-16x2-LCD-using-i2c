/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.c
  * @brief   This file provides code for the configuration
  *          of the I2C instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "i2c.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

I2C_HandleTypeDef hi2c1;

/* I2C1 init function */
void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00000E14;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspInit 0 */

  /* USER CODE END I2C1_MspInit 0 */

  /** Initializes the peripherals clocks
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
    PeriphClkInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration
    PB7     ------> I2C1_SDA
    PB8     ------> I2C1_SCL
    */
    GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* I2C1 clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();
  /* USER CODE BEGIN I2C1_MspInit 1 */

  /* USER CODE END I2C1_MspInit 1 */
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspDeInit 0 */

  /* USER CODE END I2C1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C1_CLK_DISABLE();

    /**I2C1 GPIO Configuration
    PB7     ------> I2C1_SDA
    PB8     ------> I2C1_SCL
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_7);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8);

  /* USER CODE BEGIN I2C1_MspDeInit 1 */

  /* USER CODE END I2C1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void LCD_Init(void){
	/*Function Set - As per HD44780U*/
	LCD_Send_Cmd(LCD_FUNCTION_SET1);

	/*Function Set -As per HD44780U*/
	LCD_Send_Cmd(LCD_FUNCTION_SET2);

	/*Set 4bit mode and 2 lines */
	LCD_Send_Cmd(LCD_4BIT_2LINE_MODE);

	/*Display on, cursor off*/
	LCD_Send_Cmd(0x0C);

	/* SET Row1 and Col1 (1st Line) */
	LCD_Send_Cmd(0x80);

	/*Clear Display*/
	LCD_Send_Cmd(LCD_CLEAR_DISPLAY);
}

void LCD_Send_Cmd(uint8_t cmd){
	uint8_t data_u, data_l;
	uint8_t data_u_tx[2], data_l_tx[2];

	/*Store upper nibble*/
	data_u = (cmd & 0xF0);

	/*Store lower nibble*/
	data_l = ((cmd << 4) & 0xF0);

	/* Construct upper byte-> compatible for LCD*/
	data_u_tx[0] = data_u|BACKLIGHT|PIN_EN;
	data_u_tx[1] = data_u|BACKLIGHT;

	/* Construct lower byte-> compatible for LCD*/
	data_l_tx[0] = data_l|BACKLIGHT|PIN_EN;
	data_l_tx[1] = data_l|BACKLIGHT;

	/* Transmit upper nibble using I2C APIs*/
	if(HAL_I2C_IsDeviceReady(&hi2c1, I2C_SLAVE_ADDRESS, 1, 10) == HAL_OK)
		HAL_I2C_Master_Transmit(&hi2c1,I2C_SLAVE_ADDRESS, data_u_tx, 2, 100);

	/*Provide a delay */
	HAL_Delay(5);

	/* Transmit lower nibble using I2C APIs*/
	if(HAL_I2C_IsDeviceReady(&hi2c1, I2C_SLAVE_ADDRESS, 1, 10) == HAL_OK)
		HAL_I2C_Master_Transmit(&hi2c1, I2C_SLAVE_ADDRESS, data_l_tx, 2, 100);

	/*Provide a delay */
	HAL_Delay(5);
}

void LCD_Send_Data(uint8_t data){
	uint8_t data_u, data_l;
	uint8_t data_u_tx[2], data_l_tx[2];

	/*Store upper nibble*/
	data_u = (data & 0xF0);

	/*Store lower nibble*/
	data_l = ((data << 4) & 0xF0);

	/* Construct upper byte-> compatible for LCD*/
	data_u_tx[0] = data_u|BACKLIGHT|PIN_EN|PIN_RS;
	data_u_tx[1] = data_u|BACKLIGHT|PIN_RS;

	/* Construct lower byte-> compatible for LCD*/
	data_l_tx[0] = data_l|BACKLIGHT|PIN_EN|PIN_RS;
	data_l_tx[1] = data_l|BACKLIGHT|PIN_RS;

	/* Transmit upper nibble using I2C APIs*/
	if(HAL_I2C_IsDeviceReady(&hi2c1, I2C_SLAVE_ADDRESS, 1, 10) == HAL_OK)
		HAL_I2C_Master_Transmit(&hi2c1,I2C_SLAVE_ADDRESS, data_u_tx, 2, 100);

	/*Provide a delay */
	//HAL_Delay(1);

	/* Transmit lower nibble using I2C APIs*/
	if(HAL_I2C_IsDeviceReady(&hi2c1, I2C_SLAVE_ADDRESS, 1, 10) == HAL_OK)
		HAL_I2C_Master_Transmit(&hi2c1, I2C_SLAVE_ADDRESS, data_l_tx, 2, 100);

    /*Provide a delay */
    HAL_Delay(5);
}

void LCD_Send_String(char *str){
	while (*str)
		{
			LCD_Send_Data(*str++);
		}
}

void LCD_Clear_Then_Display(char *str){
	LCD_Send_Cmd(LCD_CLEAR_DISPLAY);
	LCD_Send_String(str);
}

void LCD_Send_String_On_Line1(char *str){
	LCD_Send_Cmd(LCD_SET_ROW1_COL1);
	LCD_Send_String(str);
}

void LCD_Send_String_On_Line2(char *str){
	LCD_Send_Cmd(LCD_SET_ROW2_COL1);
	LCD_Send_String(str);
}

void LCD_Display_Long_Message(char *string){
	int i =0, count =1, j=0;
	/*Clear display and Set position to Line1 start*/
	LCD_Send_Cmd(LCD_CLEAR_DISPLAY);
	LCD_Send_Cmd(LCD_SET_ROW1_COL1);
	while(string[i] != '\0')
	{
		LCD_Send_Data(string[i]);

		/*If we reach 1st Line end, then goto 2nd line start*/
		if(j>=15 && (count%2 == 1))
		{
			count++;
			LCD_Send_Cmd(LCD_SET_ROW2_COL1);
		}

		/*If we reach second line end, clear display start from line1 again*/
		if(j>=31 && (count%2 == 0))
		{
			count++;
			j=0;
			LCD_Send_Cmd(LCD_CLEAR_DISPLAY);
			LCD_Send_Cmd(LCD_SET_ROW1_COL1);
		}

		HAL_Delay(100);
		i++;
		j++;
	}
}
/* USER CODE END 1 */
