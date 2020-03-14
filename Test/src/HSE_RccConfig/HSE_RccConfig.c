/*
 * HES_RccConfig.c
 *
 *  Created on: 2020年2月18日
 *      Author: king
 */

#include "HSE_RccConfig.h"

void HSE_SetSysClk(uint32_t RCC_PLLMul_x)
{

	ErrorStatus HSEStatus;
	//把RCC寄存器复位成复位值
	RCC_DeInit();

	//使能HSE
	RCC_HSEConfig(RCC_HSE_ON);

	HSEStatus = RCC_WaitForHSEStartUp();
	if(HSEStatus == SUCCESS)
	{
		//使能预取址
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
		FLASH_SetLatency(FLASH_Latency_2);
		//配置HCKL分频系数
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		//配置PCLK1分频系数
		RCC_PCLK1Config(RCC_HCLK_Div2);
		//配置PCLK2分频系数
		RCC_PCLK2Config(RCC_HCLK_Div1);

		//配置锁相环倍频因子  PLLCLK = HSE * RCC_PLLMul_x
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_x);
		//使能PLL
		RCC_PLLCmd(ENABLE);

		//等待PLL时钟稳定
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

		//选择系统时钟
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

		while(RCC_GetSYSCLKSource() != 0x08 );
	}
	else
	{
		//如果启动失败
	}

}


