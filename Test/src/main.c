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
//#include "../sys/sys.h"
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
#include "SPI_FLASH/spi_flash.h"
#include "FSMC_LCD/bsp_ili9341_lcd.h"


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

#if CUSTORMER == SPI_FLASH
typedef enum {FAILED =0, PASSED = !FAILED} TestStatus;

//��ȡ�������ĳ���
#define TxBufferSize1	(countof(TxBuffer1) - 1)
#define RxBufferSize1	(countof(RxBuffer1) - 1)
#define countof(a)		(sizeof(a) / sizeof(*(a)))
#define BufferSize	(countof(Tx_Buffer) - 1)

#define FLASH_WriteAddress		0x00000
#define FLASH_ReadAddress		FLASH_WriteAddress
#define FLASH_SectorToErase		FLASH_WriteAddress

//���ͻ�������ʼ��
uint8_t Tx_Buffer[]	 = "��л��ѡ��Ұ��stm32������\r\n";
uint8_t Rx_Buffer[BufferSize];

__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;
__IO TestStatus TransferStaus1 = FAILED;


#endif

#if CUSTORMER == FSMC_LCD
static void LCD_Test(void);
//static void Delay(__IO uint32_t nCount);
void printf_Charater(void);
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
//				Usart_SendString(DEBUG_USARTx,"����һ��ͨ������ͨ��ָ�����RGB�ʵ�ʵ��\n");
//				Usart_SendString(DEBUG_USARTx,"ʹ�� USART ����Ϊ��115200 8-N -1 \n");
//				Usart_SendString(DEBUG_USARTx,"������ӵ�ָ������RGB�ʵ���ɫ��ָ���Ӧ���£�\n");
//				Usart_SendString(DEBUG_USARTx,"ָ�� ------ �ʵ���ɫ \n");
//				Usart_SendString(DEBUG_USARTx,"  1  ------    �� \n");
//				Usart_SendString(DEBUG_USARTx,"  2  ------    �� \n");
//				Usart_SendString(DEBUG_USARTx,"  3  ------    �� \n");
//				Usart_SendString(DEBUG_USARTx,"  4  ------    �� \n");
//				Usart_SendString(DEBUG_USARTx,"  5  ------    �� \n");
//				Usart_SendString(DEBUG_USARTx,"  6  ------    �� \n");
//				Usart_SendString(DEBUG_USARTx,"  7  ------    �� \n");
//				Usart_SendString(DEBUG_USARTx,"  8  ------    �� \n");

				printf("hello\r\n");
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

#if CUSTORMER == SPI_FLASH
	LED_GPIO_Config();
	LED_BLUE;

	//���ô���Ϊ��115200 8-N-1
	USART_Config();

	//8M����flash W25Q64��ʼ��
	SPI_FLASH_Init();

	//��ȡflash Device ID
	DeviceID = SPI_FLASH_ReadDeviceID();
	Delay(200);

	//��ȡSPI Flash ID
	FlashID = SPI_FLASH_ReadID();

	Usart_SendString(DEBUG_USARTx,"��ȡ��FLASH_ID:");
	Usart_SendHalfWord(DEBUG_USARTx,(FlashID&0xFFFF0000)>16);
	Usart_SendHalfWord(DEBUG_USARTx,(FlashID&0x0000FFFF));
	Usart_SendString(DEBUG_USARTx,"\n");
	Usart_SendString(DEBUG_USARTx,"��ȡ��FLASH_Device_ID:");
	Usart_SendHalfWord(DEBUG_USARTx,(DeviceID&0xFFFF0000)>16);
	Usart_SendHalfWord(DEBUG_USARTx,(DeviceID&0x0000FFFF));
	Usart_SendString(DEBUG_USARTx,"\n");

	//����SPI Flash ID
	if(FlashID == sFLASH_ID)
	{

	}

#endif

#if CUSTORMER == FSMC_LCD
	ILI9341_Init();	//LCD��ʼ��
	USART_Config();
	printf("\r\n*************** Һ����Ӣ����ʾ���� ****************\r\n");
	printf("\r\n ������֧�����ģ���ʾ���ĵĳ�����ѧϰ��һ�� \r\n");

	//����0��3��5��6ģʽ�ʺϴ���������ʾ����
	//���Ƽ�ʹ������ģʽ��ʾ���֣�����ģʽ��ʾ���ֻ��о���Ч��
	//����6ģʽΪ�󲿷�Һ�����̵�Ĭ����ʾ����
	ILI9341_GramScan(6);
	while(1)
	{
		LCD_Test();
	}

#endif

}

#if CUSTORMER == FSMC_LCD
//���ڲ��Ը���Һ���ĺ���
void LCD_Test(void)
{
	//��ʾ��ʾ����
	static uint8_t testCNT = 0;
	char dispBuff[100];

	testCNT++;

	LCD_SetFont(&Font8x16);
	LCD_SetColors(RED,BLACK);

	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);//��������ʾȫ��
	/* ��ʾ�ַ���ʾ�� */
	ILI9341_DispStringLine_EN(LINE(0),"BH 3.2 inch LCD para:");
	ILI9341_DispStringLine_EN(LINE(1),"Image resolution:240x320 px");
	ILI9341_DispStringLine_EN(LINE(2),"ILI9341 LCD Driver");
	ILI9341_DispStringLine_EN(LINE(3),"XPT2046 Touch Pad Driver");

	/* ��ʾ����ʾ�� */
	LCD_SetFont(&Font16x24);
	LCD_SetTextColor(GREEN);

	//ʹ��C��׼��ѱ���ת�����ַ���
	sprintf(dispBuff,"Count:%d",testCNT);
	LCD_ClearLine(LINE(4));//�����������

	//Ȼ����ʾ���ַ������ɣ���������Ҳ����������
	ILI9341_DispStringLine_EN(LINE(4),dispBuff);

	//��ʾͼ��ʾ��
	LCD_SetFont(&Font24x32);

	//��ֱ��
	LCD_ClearLine(LINE(4));//�����������
	LCD_SetTextColor(BLUE);

	ILI9341_DispStringLine_EN(LINE(4),"Draw Line");

	LCD_SetTextColor(RED);
	ILI9341_DrawLine(50,170,210,230);
	ILI9341_DrawLine(50,200,210,240);

	LCD_SetTextColor(GREEN);
	ILI9341_DrawLine(100,170,200,230);
	ILI9341_DrawLine(200,200,220,240);

	LCD_SetTextColor(BLUE);
	ILI9341_DrawLine(110,170,110,230);
	ILI9341_DrawLine(130,200,220,240);

	Delay(0xFFFFFF);

	ILI9341_Clear(0,16*8,LCD_X_LENGTH,LCD_Y_LENGTH-16*8); //��������ʾȫ��

	//������
	LCD_ClearLine(LINE(4)); //�����������
	LCD_SetTextColor(BLUE);

	ILI9341_DispStringLine_EN(LINE(4),"Draw Rect:");

	LCD_SetTextColor(RED);
	ILI9341_DrawRectangle(50,200,100,30,1);

	LCD_SetTextColor(GREEN);
	ILI9341_DrawRectangle(160,200,20,40,0);

	LCD_SetTextColor(BLUE);
	ILI9341_DrawRectangle(170,200,50,20,1);

	Delay(0xFFFFFF);

	ILI9341_Clear(0,16*8,LCD_X_LENGTH,LCD_Y_LENGTH-16*8); //��������ʾȫ��

	//��Բ
	LCD_ClearLine(LINE(4)); //�����������
	LCD_SetTextColor(BLUE);

	ILI9341_DispStringLine_EN(LINE(4),"Draw Cir:");

	LCD_SetTextColor(RED);
	ILI9341_DrawCircle(100,200,20,0);

	LCD_SetTextColor(GREEN);
	ILI9341_DrawCircle(100,200,10,1);

	LCD_SetTextColor(BLUE);
	ILI9341_DrawCircle(140,200,20,0);

	Delay(0xFFFFFF);

	ILI9341_Clear(0,16*8,LCD_X_LENGTH,LCD_Y_LENGTH-16*8); //��������ʾȫ��

}

/*
 * ����ʱ����
 * nCount����ʱ����ֵ
 * */
//static void Delay(__IO uint32_t nCount)
//{
//	for(;nCount != 0;nCount--);
//}

#endif


#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
