/*
 * Copyright (c) 2022 STMicroelectronics.
 * Copyright (c) 2022 nfrizzell
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 */

#include <stdlib.h>
#include <stdio.h>

#include "stm32f3xx_ll_gpio.h"

#include "main.h"

static void system_clock_init(void);
static void gpio_init(void);

int main(void)
{
	system_clock_init();
	gpio_init();

	while (1)
	{

	}
}

static void system_clock_init(void)
{
	LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);
	while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_1) {};

	LL_RCC_HSE_EnableBypass();
	LL_RCC_HSE_Enable();
	while(LL_RCC_HSE_IsReady() != 1) {};

	LL_RCC_HSI_Enable();
	while(LL_RCC_HSI_IsReady() != 1) {};

	LL_RCC_HSI_SetCalibTrimming(16);
	LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_6);
	LL_RCC_PLL_Enable();
	while(LL_RCC_PLL_IsReady() != 1) {};
	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  	LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
	LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
	while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) {};

	LL_SetSystemCoreClock(48000000);

	/* Unused currently */
	/*
	if (HAL_InitTick(TICK_INT_PRIORITY) != HAL_OK)
	{
		Error_Handler();
	}

	LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_HSI);
	LL_RCC_SetUSBClockSource(LL_RCC_USB_CLKSOURCE_PLL);
	*/
}

/* Autogenetated, to-do: rewrite & remove bloat */
static void gpio_init(void)
{
	LL_EXTI_InitTypeDef EXTI_InitStruct = {0};
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE);
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOF);
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

	LL_GPIO_ResetOutputPin(GPIOE, /*CS_I2C_SPI_Pin|*/LD3_Pin|LD7_Pin|LD9_Pin
				|LD10_Pin|LD8_Pin|LD6_Pin);

	LL_GPIO_SetOutputPin(GPIOE, LD3_Pin);

	/*
	LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTE, LL_SYSCFG_EXTI_LINE2);
	LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTE, LL_SYSCFG_EXTI_LINE4);
	LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTE, LL_SYSCFG_EXTI_LINE5);
	LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTE, LL_SYSCFG_EXTI_LINE0);
	LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTE, LL_SYSCFG_EXTI_LINE1);

	LL_GPIO_SetPinPull(DRDY_GPIO_Port, DRDY_Pin, LL_GPIO_PULL_NO);
	LL_GPIO_SetPinPull(MEMS_INT3_GPIO_Port, MEMS_INT3_Pin, LL_GPIO_PULL_NO);
	LL_GPIO_SetPinPull(MEMS_INT4_GPIO_Port, MEMS_INT4_Pin, LL_GPIO_PULL_NO);
	LL_GPIO_SetPinPull(MEMS_INT1_GPIO_Port, MEMS_INT1_Pin, LL_GPIO_PULL_NO);
	LL_GPIO_SetPinPull(MEMS_INT2_GPIO_Port, MEMS_INT2_Pin, LL_GPIO_PULL_NO);

	LL_GPIO_SetPinMode(DRDY_GPIO_Port, DRDY_Pin, LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinMode(MEMS_INT3_GPIO_Port, MEMS_INT3_Pin, LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinMode(MEMS_INT4_GPIO_Port, MEMS_INT4_Pin, LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinMode(MEMS_INT1_GPIO_Port, MEMS_INT1_Pin, LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinMode(MEMS_INT2_GPIO_Port, MEMS_INT2_Pin, LL_GPIO_MODE_INPUT);

	EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_2;
	EXTI_InitStruct.Line_32_63 = LL_EXTI_LINE_NONE;
	EXTI_InitStruct.LineCommand = ENABLE;
	EXTI_InitStruct.Mode = LL_EXTI_MODE_EVENT;
	EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
	LL_EXTI_Init(&EXTI_InitStruct);

	EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_4;
	EXTI_InitStruct.Line_32_63 = LL_EXTI_LINE_NONE;
	EXTI_InitStruct.LineCommand = ENABLE;
	EXTI_InitStruct.Mode = LL_EXTI_MODE_EVENT;
	EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
	LL_EXTI_Init(&EXTI_InitStruct);

	EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_5;
	EXTI_InitStruct.Line_32_63 = LL_EXTI_LINE_NONE;
	EXTI_InitStruct.LineCommand = ENABLE;
	EXTI_InitStruct.Mode = LL_EXTI_MODE_EVENT;
	EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
	LL_EXTI_Init(&EXTI_InitStruct);

	EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_0;
	EXTI_InitStruct.Line_32_63 = LL_EXTI_LINE_NONE;
	EXTI_InitStruct.LineCommand = ENABLE;
	EXTI_InitStruct.Mode = LL_EXTI_MODE_EVENT;
	EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
	LL_EXTI_Init(&EXTI_InitStruct);

	EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_1;
	EXTI_InitStruct.Line_32_63 = LL_EXTI_LINE_NONE;
	EXTI_InitStruct.LineCommand = ENABLE;
	EXTI_InitStruct.Mode = LL_EXTI_MODE_EVENT;
	EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
	LL_EXTI_Init(&EXTI_InitStruct);
	*/

	GPIO_InitStruct.Pin = /*CS_I2C_SPI_Pin|*/LD4_Pin|LD3_Pin|LD5_Pin
				|LD7_Pin|LD9_Pin|LD10_Pin|LD8_Pin
				|LD6_Pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	/*
	GPIO_InitStruct.Pin = B1_Pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);
	*/
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
	printf("Wrong parameters value: file %s on line %d\r\n", file, line);
}
#endif /* USE_FULL_ASSERT */
