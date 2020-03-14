//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"
#include "stm32f10x.h"
#include "Timer.h"
#include "BlinkLed.h"
#include "sys.h"
#include "HSE_RccConfig/HSE_RccConfig.h"
#include "LED_RGB_TEST/led_rgb_test.h"
#include "CustomerSetting.h"
#include "KEY_TEST/key.h"
#include "Bit_Operation/Bit_Operation.h"
#include "EXTI_test/EXTI_test.h"
#include "SysTick_test/SysTick_test.h"
#include "USART_Test/Usart_test.h"
#include "USART_DMA/Usart_Dma.h"
#include "DMA_M2M/DMA_M2M.h"
#include "I2C_ANALOG/i2c_gpio.h"
#include "I2C_ANALOG/i2c_analog.h"
// ----------------------------------------------------------------------------
//
// Standalone STM32F1 led blink sample (trace via DEBUG).
//
// In debug configurations, demonstrate how to print a greeting message
// on the trace device. In release configurations the message is
// simply discarded.
//
// Then demonstrates how to blink a led with 1 Hz, using a
// continuous loop and SysTick delays.
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the DEBUG output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//
// The external clock frequency is specified as a preprocessor definition
// passed to the compiler via a command line option (see the 'C/C++ General' ->
// 'Paths and Symbols' -> the 'Symbols' tab, if you want to change it).
// The value selected during project creation was HSE_VALUE=8000000.
//
// Note: The default clock settings take the user defined HSE_VALUE and try
// to reach the maximum possible system clock. For the default 8 MHz input
// the result is guaranteed, but for other values it might not be possible,
// so please adjust the PLL settings in system/src/cmsis/system_stm32f10x.c
//

// ----- Timing definitions -------------------------------------------------

// Keep the LED on for 2/3 of a second.
#define BLINK_ON_TICKS  (TIMER_FREQUENCY_HZ * 3 / 4)
#define BLINK_OFF_TICKS (TIMER_FREQUENCY_HZ - BLINK_ON_TICKS)

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"


#if CUSTORMER == USART_DMA_TEST
extern uint8_t SendBuff[SENDBUFF_SIZE];
#endif

#if CUSTORMER == DMA_M2M_TEST
extern const uint32_t aSRC_Const_Buffer[BUFFER_SIZE];
extern uint32_t aDST_Buffer[BUFFER_SIZE];
#endif

int main(int argc, char* argv[])
{
	timer_start();
	//HSE_SetSysClk(RCC_CFGR_PLLMULL16);
#if CUSTORMER == LED_RGB_TEST
	LED_GPIO_Config();
	while(1)
	{
	  LED_Corlor_test();
	}
#endif

#if CUSTORMER == KEY_TEST
	//LED�˿ڳ�ʼ��
	LED_GPIO_Config();
	LED1_ON;

	//�����˿ڳ�ʼ��
	KEY_GPIO_Config();

	//��ѯ����״̬��������������תLED
	while(1)
	{
		if(Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON)
		{
			//LED1��ת
			LED1_TOGGLE;
		}

		if(Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON)
		{
			//LED2��ת
			LED2_TOGGLE;
		}
	}

#endif

#if CUSTORMER == EXTI_TEST
	LED_GPIO_Config();
	EXTI_Key_Config();


	while(1)
	{

	}
#endif

#if CUSTORMER == SYSTICK_TEST
	//LED�˿ڳ�ʼ��
	LED_GPIO_Config();

	//����SysTick Ϊ10us�ж�һ��
	SysTick_Init();
#if 0
	while(1)
	{
		LED1(ON);
		Delay_us(100000);	//100000 * 10us = 1000ms
		//Delay_ms(100);
		LED1(OFF);

		LED2(ON);
		Delay_us(100000);	//100000 * 10us = 1000ms
		//Delay_ms(100);
		LED2(OFF);

		LED3(ON);
		Delay_us(100000);	//100000 * 10us = 1000ms
		//Delay_ms(100);
		LED3(OFF);

	}
#else
	while(1)
	{
//		LED_RED;
//		SysTick_Delay_Ms(1000);
//		LED_GREEN;
//		SysTick_Delay_Ms(1000);
//		LED_BLUE;
//		SysTick_Delay_Ms(1000);

		LED1(ON);
		SysTick_Delay_Ms(200);
		LED1(OFF);

		LED2(ON);
		SysTick_Delay_Ms(200);
		LED2(OFF);

		LED3(ON);
		SysTick_Delay_Ms(200);
		LED3(OFF);
	}

#endif
#endif

#if CUSTORMER == USART_TEST
 //��ʼ��USART����ģʽΪ115200 8-N-1���жϽ���
	USART_Config();

	//����һ���ַ���
	Usart_SendString(DEBUG_USARTx,"����һ�������жϽ��ջ���ʵ��\n");
	while(1)
	{

	}

#endif

#if CUSTORMER == USART_CTR_LED
	char ch = '0';
	char ch_buf = '0';
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
	//��ʼ��LED

	LED_GPIO_Config();

	//��ʼ��USART����ģʽΪ115200 8-N-1
	USART_Config();
	//��ӡָ��������ʾ��Ϣ
	Usart_SendString(DEBUG_USARTx,"����һ��ͨ������ͨ��ָ�����RGB�ʵ�ʵ��\n");
	Usart_SendString(DEBUG_USARTx,"ʹ�� USART ����Ϊ��115200 8-N -1 \n");
	Usart_SendString(DEBUG_USARTx,"������ӵ�ָ������RGB�ʵ���ɫ��ָ���Ӧ���£�\n");
	Usart_SendString(DEBUG_USARTx,"ָ�� ------ �ʵ���ɫ \n");
	Usart_SendString(DEBUG_USARTx,"  1  ------    �� \n");
	Usart_SendString(DEBUG_USARTx,"  2  ------    �� \n");
	Usart_SendString(DEBUG_USARTx,"  3  ------    �� \n");
	Usart_SendString(DEBUG_USARTx,"  4  ------    �� \n");
	Usart_SendString(DEBUG_USARTx,"  5  ------    �� \n");
	Usart_SendString(DEBUG_USARTx,"  6  ------    �� \n");
	Usart_SendString(DEBUG_USARTx,"  7  ------    �� \n");
	Usart_SendString(DEBUG_USARTx,"  8  ------    �� \n");

	while(1)
	{
		//��ȡ�ַ�ָ��
		if(DEBUG_USARTx->SR & 1<<5)
		{
			ch = DEBUG_USARTx->DR & 0xFF;
		}
		//trace_printf("���յ��ַ���%c\n",ch);
		//Usart_SendByte(DEBUG_USARTx,ch);
		//�����ַ�ָ��Ŀ���RGB�ʵ���ɫ
		if(ch != ch_buf)
		{
			switch(ch)
			{
			case '1':
				LED_RED;
				break;
			case '2':
				LED_GREEN;
				break;
			case '3':
				LED_BLUE;
				break;
			case '4':
				LED_YELLOW;
				break;
			case '5':
				LED_PURPLE;
				break;
			case '6':
				LED_CYAN;
				break;
			case '7':
				LED_WHITE;
				break;
			case '8':
				LED_RGBOFF;
				break;
			default:
				//�������ָ��ָ���ַ�����ӡ��ʾ��Ϣ
				Usart_SendString(DEBUG_USARTx,"����һ��ͨ������ͨ��ָ�����RGB�ʵ�ʵ��\n");
				Usart_SendString(DEBUG_USARTx,"ʹ�� USART ����Ϊ��115200 8-N -1 \n");
				Usart_SendString(DEBUG_USARTx,"������ӵ�ָ������RGB�ʵ���ɫ��ָ���Ӧ���£�\n");
				Usart_SendString(DEBUG_USARTx,"ָ�� ------ �ʵ���ɫ \n");
				Usart_SendString(DEBUG_USARTx,"  1  ------    �� \n");
				Usart_SendString(DEBUG_USARTx,"  2  ------    �� \n");
				Usart_SendString(DEBUG_USARTx,"  3  ------    �� \n");
				Usart_SendString(DEBUG_USARTx,"  4  ------    �� \n");
				Usart_SendString(DEBUG_USARTx,"  5  ------    �� \n");
				Usart_SendString(DEBUG_USARTx,"  6  ------    �� \n");
				Usart_SendString(DEBUG_USARTx,"  7  ------    �� \n");
				Usart_SendString(DEBUG_USARTx,"  8  ------    �� \n");
				break;
			}
		}
		ch_buf = ch;
	}
#endif

#if CUSTORMER == USART_DMA_TEST
	uint16_t i;
	//��ʼ��USART
	USART_Config();
	//����DMAģʽ
	USARTx_DMA_Config();
	//����RGB��ɫ��
	LED_GPIO_Config();

	//��佫Ҫ���͵�����
	for(i = 0; i < SENDBUFF_SIZE;i++)
	{
		SendBuff[i] = 'P';
	}

	//Ϊ��ʾDMA�������ж�CPU���ܴ����������飬����ʹ��DMA�������ݣ����ǳ���
	//��ʱ�����п��ܻᵼ�µ��Զ˴��ڵ������ֻῨ��������ҷɵ������
	//���DMA�����е�ѭ��ģʽ��Ϊ����ģʽ

	//USART1��DMA����Tx����
	USART_DMACmd(DEBUG_USARTx,USART_DMAReq_Tx,ENABLE);

	//��ʱCPU�ǿ��еģ����Ը���������
	//����ͬʱ����LED��
	while(1)
	{
		LED1_TOGGLE;
		Delay(0xFFFFF);
	}
#endif

#if CUSTORMER == DMA_M2M_TEST

	//�����űȽϽ���ı���
	u8 TransferStatus;

	//LED�˿ڳ�ʼ��
	LED_GPIO_Config();

	//����RGB��ɫ��Ϊ��ɫ
	LED_PURPLE;

	//����ʱ����
	Delay(0xFFFFFF);

	//DMA��������
	DMA_Config();

	//�ȴ�DMA�������
	while(DMA_GetFlagStatus(DMA_FLAG_TC) == RESET)
	{

	}

	//�Ƚ�Դ�����봫�������
	TransferStatus = Buffercmp(aSRC_Const_Buffer,aDST_Buffer,BUFFER_SIZE);

	//�ж�Դ�����봫������ݱȽϽ��
	if(TransferStatus == 0)
	{
		//Դ�����봫������ݲ����ʱRGB�ʵ���ʾ��ɫ
		LED_RED;
	}
	else
	{
		//Դ�����봫����������ʱRGB��ɫ����ʾ��ɫ
		LED_BLUE;
	}

	while(1)
	{

	}


#endif

#if CUSTORMER == I2C_ANALOG
	LED_GPIO_Config();

	LED_BLUE;
	//���ڳ�ʼ��
	USART_Config();
	//printf("EEPROM���ģ��i2c��������\r\n");
	Usart_SendString(DEBUG_USARTx,"EEPROM���ģ��i2c��������");
	Usart_SendString(DEBUG_USARTx,"\n");

	if(ee_Test() == 1)
	{
		LED_GREEN;
	}
	else
	{
		LED_RED;
	}
	while(1);

#endif

}


#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
