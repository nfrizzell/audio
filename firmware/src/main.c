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

#include "stm32f3xx_ll_gpio.h"

#include "main.h"

void system_clock_config(void);
GPIO_TypeDef *gpio_init(void);

int main(void)
{
	/* LL_GPIO_InitTypeDef pins; */
	system_clock_config();

	GPIO_TypeDef *port = gpio_init();
	LL_GPIO_SetOutputPin(port, LL_GPIO_PIN_11);

	while (1)
	{

	}
}

void system_clock_config(void)
{
	/* Set FLASH latency */
	LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);

	/* Enable HSE and wait for activation*/
	LL_RCC_HSE_Enable();
	while(LL_RCC_HSE_IsReady() != 1)
	{

	};

	/* Main PLL configuration and activation */
	LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9);

	LL_RCC_PLL_Enable();
	while(LL_RCC_PLL_IsReady() != 1)
	{

	};

	/* Sysclk activation on the main PLL */
	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
	while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
	{

	};

	/* Set APB1 & APB2 prescaler*/
	LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
	LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

	/* Set systick to 1ms in using frequency set to 72MHz */
	/* This frequency can be calculated through LL RCC macro */
	/* ex: __LL_RCC_CALC_PLLCLK_FREQ (HSE_VALUE, LL_RCC_PLL_MUL_9) */
	LL_Init1msTick(72000000);

	/* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
	LL_SetSystemCoreClock(72000000);
}

GPIO_TypeDef *gpio_init(void)
{
	LL_GPIO_InitTypeDef gpio_config;
	GPIO_TypeDef * port = malloc(sizeof(GPIO_TypeDef));
	
	LL_GPIO_StructInit(&gpio_config);
	gpio_config.Pin = LL_GPIO_PIN_11;
	gpio_config.Mode = LL_GPIO_MODE_OUTPUT;

	if (LL_GPIO_Init(port, &gpio_config) != SUCCESS)
	{
		
	}

	return port;
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
	printf("Wrong parameters value: file %s on line %d\r\n", file, line);
}
#endif /* USE_FULL_ASSERT */
