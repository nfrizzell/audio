#include "common.h"
#include "init.h"

void system_clock_init(void)
{
	LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);
	while (LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_1);

	LL_RCC_HSE_EnableBypass();
	LL_RCC_HSE_Enable();
	while(LL_RCC_HSE_IsReady() != 1);

	LL_RCC_HSI_Enable();
	while(LL_RCC_HSI_IsReady() != 1);

	LL_RCC_HSI_SetCalibTrimming(16);
	LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_6);
	LL_RCC_PLL_Enable();
	while(LL_RCC_PLL_IsReady() != 1);

	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  	LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
	LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
	while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL);

	LL_SetSystemCoreClock(CORE_CLOCK);

	LL_RCC_SetUSBClockSource(LL_RCC_USB_CLKSOURCE_PLL);

	/* GPIO Ports Clock Enable */
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE);
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOF);
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
}

/* Autogenetated, to-do: rewrite & remove bloat */
void gpio_init(void)
{
	LL_EXTI_InitTypeDef EXTI_InitStruct = {0};
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	LL_GPIO_ResetOutputPin(GPIOE, /*CS_I2C_SPI_Pin|*/LD3_Pin|LD7_Pin|LD9_Pin
				|LD10_Pin|LD8_Pin|LD6_Pin);

	LL_GPIO_SetOutputPin(GPIOE, LD3_Pin);

	GPIO_InitStruct.Pin = /*CS_I2C_SPI_Pin|*/LD4_Pin|LD3_Pin|LD5_Pin
				|LD7_Pin|LD9_Pin|LD10_Pin|LD8_Pin
				|LD6_Pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}
