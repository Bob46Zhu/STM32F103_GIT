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

//获取缓冲区的长度
#define TxBufferSize1	(countof(TxBuffer1) - 1)
#define RxBufferSize1	(countof(RxBuffer1) - 1)
#define countof(a)		(sizeof(a) / sizeof(*(a)))
#define BufferSize	(countof(Tx_Buffer) - 1)

#define FLASH_WriteAddress		0x00000
#define FLASH_ReadAddress		FLASH_WriteAddress
#define FLASH_SectorToErase		FLASH_WriteAddress

//发送缓冲区初始化
uint8_t Tx_Buffer[]	 = "感谢您选用野火stm32开发板\r\n";
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
	//LED端口初始化
	LED_GPIO_Config();
	LED1_ON;

	//按键端口初始化
	KEY_GPIO_Config();

	//轮询按键状态，若按键按下则反转LED
	while(1)
	{
		if(Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON)
		{
			//LED1反转
			LED1_TOGGLE;
		}

		if(Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON)
		{
			//LED2反转
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
	//LED端口初始化
	LED_GPIO_Config();

	//配置SysTick 为10us中断一次
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
 //初始化USART配置模式为115200 8-N-1，中断接收
	USART_Config();

	//发送一个字符串
	Usart_SendString(DEBUG_USARTx,"这是一个串口中断接收回显实验\n");
	while(1)
	{

	}

#endif

#if CUSTORMER == USART_CTR_LED
	char ch = '0';
	char ch_buf = '0';
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
	//初始化LED

	LED_GPIO_Config();

	//初始化USART配置模式为115200 8-N-1
	USART_Config();
	//打印指令输入提示信息
	Usart_SendString(DEBUG_USARTx,"这是一个通过串口通信指令控制RGB彩灯实验\n");
	Usart_SendString(DEBUG_USARTx,"使用 USART 参数为：115200 8-N -1 \n");
	Usart_SendString(DEBUG_USARTx,"开发板接到指令后控制RGB彩灯颜色，指令对应如下：\n");
	Usart_SendString(DEBUG_USARTx,"指令 ------ 彩灯颜色 \n");
	Usart_SendString(DEBUG_USARTx,"  1  ------    红 \n");
	Usart_SendString(DEBUG_USARTx,"  2  ------    绿 \n");
	Usart_SendString(DEBUG_USARTx,"  3  ------    蓝 \n");
	Usart_SendString(DEBUG_USARTx,"  4  ------    黄 \n");
	Usart_SendString(DEBUG_USARTx,"  5  ------    紫 \n");
	Usart_SendString(DEBUG_USARTx,"  6  ------    青 \n");
	Usart_SendString(DEBUG_USARTx,"  7  ------    白 \n");
	Usart_SendString(DEBUG_USARTx,"  8  ------    灭 \n");

	while(1)
	{
		//获取字符指令
		if(DEBUG_USARTx->SR & 1<<5)
		{
			ch = DEBUG_USARTx->DR & 0xFF;
		}
		//trace_printf("接收到字符：%c\n",ch);
		//Usart_SendByte(DEBUG_USARTx,ch);
		//根据字符指令的控制RGB彩灯颜色
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
				//如果不是指定指令字符，打印提示信息
//				Usart_SendString(DEBUG_USARTx,"这是一个通过串口通信指令控制RGB彩灯实验\n");
//				Usart_SendString(DEBUG_USARTx,"使用 USART 参数为：115200 8-N -1 \n");
//				Usart_SendString(DEBUG_USARTx,"开发板接到指令后控制RGB彩灯颜色，指令对应如下：\n");
//				Usart_SendString(DEBUG_USARTx,"指令 ------ 彩灯颜色 \n");
//				Usart_SendString(DEBUG_USARTx,"  1  ------    红 \n");
//				Usart_SendString(DEBUG_USARTx,"  2  ------    绿 \n");
//				Usart_SendString(DEBUG_USARTx,"  3  ------    蓝 \n");
//				Usart_SendString(DEBUG_USARTx,"  4  ------    黄 \n");
//				Usart_SendString(DEBUG_USARTx,"  5  ------    紫 \n");
//				Usart_SendString(DEBUG_USARTx,"  6  ------    青 \n");
//				Usart_SendString(DEBUG_USARTx,"  7  ------    白 \n");
//				Usart_SendString(DEBUG_USARTx,"  8  ------    灭 \n");

				printf("hello\r\n");
				break;
			}
		}
		ch_buf = ch;
	}
#endif

#if CUSTORMER == USART_DMA_TEST
	uint16_t i;
	//初始化USART
	USART_Config();
	//配置DMA模式
	USARTx_DMA_Config();
	//配置RGB彩色灯
	LED_GPIO_Config();

	//填充将要发送的数据
	for(i = 0; i < SENDBUFF_SIZE;i++)
	{
		SendBuff[i] = 'P';
	}

	//为演示DMA持续运行而CPU还能处理其它事情，持续使用DMA发送数据，量非常大，
	//长时间运行可能会导致电脑端串口调试助手会卡死，鼠标乱飞的情况，
	//或把DMA配置中的循环模式改为单次模式

	//USART1向DMA发送Tx请求
	USART_DMACmd(DEBUG_USARTx,USART_DMAReq_Tx,ENABLE);

	//此时CPU是空闲的，可以干其他事情
	//例如同时控制LED灯
	while(1)
	{
		LED1_TOGGLE;
		Delay(0xFFFFF);
	}
#endif

#if CUSTORMER == DMA_M2M_TEST

	//定义存放比较结果的变量
	u8 TransferStatus;

	//LED端口初始化
	LED_GPIO_Config();

	//设置RGB彩色灯为紫色
	LED_PURPLE;

	//简单延时函数
	Delay(0xFFFFFF);

	//DMA传输配置
	DMA_Config();

	//等待DMA传输完成
	while(DMA_GetFlagStatus(DMA_FLAG_TC) == RESET)
	{

	}

	//比较源数据与传输后数据
	TransferStatus = Buffercmp(aSRC_Const_Buffer,aDST_Buffer,BUFFER_SIZE);

	//判断源数据与传输后数据比较结果
	if(TransferStatus == 0)
	{
		//源数据与传输后数据不相等时RGB彩灯显示红色
		LED_RED;
	}
	else
	{
		//源数据与传输后数据相等时RGB彩色灯显示蓝色
		LED_BLUE;
	}

	while(1)
	{

	}


#endif

#if CUSTORMER == I2C_ANALOG
	LED_GPIO_Config();

	LED_BLUE;
	//串口初始化
	USART_Config();
	//printf("EEPROM软件模拟i2c测试例程\r\n");
	Usart_SendString(DEBUG_USARTx,"EEPROM软件模拟i2c测试例程");
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

	//配置串口为：115200 8-N-1
	USART_Config();

	//8M串行flash W25Q64初始化
	SPI_FLASH_Init();

	//获取flash Device ID
	DeviceID = SPI_FLASH_ReadDeviceID();
	Delay(200);

	//获取SPI Flash ID
	FlashID = SPI_FLASH_ReadID();

	Usart_SendString(DEBUG_USARTx,"读取到FLASH_ID:");
	Usart_SendHalfWord(DEBUG_USARTx,(FlashID&0xFFFF0000)>16);
	Usart_SendHalfWord(DEBUG_USARTx,(FlashID&0x0000FFFF));
	Usart_SendString(DEBUG_USARTx,"\n");
	Usart_SendString(DEBUG_USARTx,"读取到FLASH_Device_ID:");
	Usart_SendHalfWord(DEBUG_USARTx,(DeviceID&0xFFFF0000)>16);
	Usart_SendHalfWord(DEBUG_USARTx,(DeviceID&0x0000FFFF));
	Usart_SendString(DEBUG_USARTx,"\n");

	//检验SPI Flash ID
	if(FlashID == sFLASH_ID)
	{

	}

#endif

#if CUSTORMER == FSMC_LCD
	ILI9341_Init();	//LCD初始化
	USART_Config();
	printf("\r\n*************** 液晶屏英文显示程序 ****************\r\n");
	printf("\r\n 本程序不支持中文，显示中文的程序请学习下一章 \r\n");

	//其中0、3、5、6模式适合从左至右显示文字
	//不推荐使用其他模式显示文字，其它模式显示文字会有镜像效果
	//其中6模式为大部分液晶例程的默认显示方向
	ILI9341_GramScan(6);
	while(1)
	{
		LCD_Test();
	}

#endif

}

#if CUSTORMER == FSMC_LCD
//用于测试各种液晶的函数
void LCD_Test(void)
{
	//演示显示变量
	static uint8_t testCNT = 0;
	char dispBuff[100];

	testCNT++;

	LCD_SetFont(&Font8x16);
	LCD_SetColors(RED,BLACK);

	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);//清屏，显示全黑
	/* 显示字符串示例 */
	ILI9341_DispStringLine_EN(LINE(0),"BH 3.2 inch LCD para:");
	ILI9341_DispStringLine_EN(LINE(1),"Image resolution:240x320 px");
	ILI9341_DispStringLine_EN(LINE(2),"ILI9341 LCD Driver");
	ILI9341_DispStringLine_EN(LINE(3),"XPT2046 Touch Pad Driver");

	/* 显示变量示例 */
	LCD_SetFont(&Font16x24);
	LCD_SetTextColor(GREEN);

	//使用C标准库把变量转化成字符串
	sprintf(dispBuff,"Count:%d",testCNT);
	LCD_ClearLine(LINE(4));//清除单行文字

	//然后显示该字符串即可，其他变量也是这样处理
	ILI9341_DispStringLine_EN(LINE(4),dispBuff);

	//显示图形示例
	LCD_SetFont(&Font24x32);

	//画直线
	LCD_ClearLine(LINE(4));//清除单行文字
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

	ILI9341_Clear(0,16*8,LCD_X_LENGTH,LCD_Y_LENGTH-16*8); //清屏，显示全黑

	//画矩形
	LCD_ClearLine(LINE(4)); //清除单行文字
	LCD_SetTextColor(BLUE);

	ILI9341_DispStringLine_EN(LINE(4),"Draw Rect:");

	LCD_SetTextColor(RED);
	ILI9341_DrawRectangle(50,200,100,30,1);

	LCD_SetTextColor(GREEN);
	ILI9341_DrawRectangle(160,200,20,40,0);

	LCD_SetTextColor(BLUE);
	ILI9341_DrawRectangle(170,200,50,20,1);

	Delay(0xFFFFFF);

	ILI9341_Clear(0,16*8,LCD_X_LENGTH,LCD_Y_LENGTH-16*8); //清屏，显示全黑

	//画圆
	LCD_ClearLine(LINE(4)); //清除单行文字
	LCD_SetTextColor(BLUE);

	ILI9341_DispStringLine_EN(LINE(4),"Draw Cir:");

	LCD_SetTextColor(RED);
	ILI9341_DrawCircle(100,200,20,0);

	LCD_SetTextColor(GREEN);
	ILI9341_DrawCircle(100,200,10,1);

	LCD_SetTextColor(BLUE);
	ILI9341_DrawCircle(140,200,20,0);

	Delay(0xFFFFFF);

	ILI9341_Clear(0,16*8,LCD_X_LENGTH,LCD_Y_LENGTH-16*8); //清屏，显示全黑

}

/*
 * 简单延时函数
 * nCount：延时计数值
 * */
//static void Delay(__IO uint32_t nCount)
//{
//	for(;nCount != 0;nCount--);
//}

#endif


#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
