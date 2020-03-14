/*
 * HES_RccConfig.c
 *
 *  Created on: 2020��2��18��
 *      Author: king
 */

#include "HSE_RccConfig.h"

void HSE_SetSysClk(uint32_t RCC_PLLMul_x)
{

	ErrorStatus HSEStatus;
	//��RCC�Ĵ�����λ�ɸ�λֵ
	RCC_DeInit();

	//ʹ��HSE
	RCC_HSEConfig(RCC_HSE_ON);

	HSEStatus = RCC_WaitForHSEStartUp();
	if(HSEStatus == SUCCESS)
	{
		//ʹ��Ԥȡַ
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
		FLASH_SetLatency(FLASH_Latency_2);
		//����HCKL��Ƶϵ��
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		//����PCLK1��Ƶϵ��
		RCC_PCLK1Config(RCC_HCLK_Div2);
		//����PCLK2��Ƶϵ��
		RCC_PCLK2Config(RCC_HCLK_Div1);

		//�������໷��Ƶ����  PLLCLK = HSE * RCC_PLLMul_x
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_x);
		//ʹ��PLL
		RCC_PLLCmd(ENABLE);

		//�ȴ�PLLʱ���ȶ�
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

		//ѡ��ϵͳʱ��
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

		while(RCC_GetSYSCLKSource() != 0x08 );
	}
	else
	{
		//�������ʧ��
	}

}


