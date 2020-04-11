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
#include "LED_RGB_TEST/led_rgb_test.h"
#include "CustomerSetting.h"
#include "SysTick_test/SysTick_test.h"
#include "USART_Test/Usart_test.h"
#include "USART_DMA/Usart_Dma.h"
#include "LED_Line.h"


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



int main(int argc, char* argv[])
{
	//timer_start();
	//HSE_SetSysClk(RCC_CFGR_PLLMULL16);
//	RGB_LED_Init();
//	RGB_LED_Reset();
//	while(1)
//	{
//		RGB_LED_Blue();
//		SysTick_Delay_Us(100000);
//		RGB_LED_Red();
//		SysTick_Delay_Us(100000);
//		RGB_LED_Green();
//		SysTick_Delay_Us(100000);
//	}

/*****************************************************
 * 			SPI + DMA �⺯���汾
 * ****************************************/
	int i = 0;
	unsigned int PixColorDa = 0;
	uint8_t Red,Green,Blue;

	RGB_LED_Init();
	while(1)
	{
//		Set_Flow_Pixel(0xFF,0x00,0x00);
//		DMA2_Start_SPI_Tx();
		for(i=0;i<3;i++)
		{
			PixColorDa = (0xFF<<(8*i));	//��ε���RGB

			Blue = (PixColorDa >> 16)&0xFF;
			Green = (PixColorDa >> 8)&0xFF;
			Red = (PixColorDa)&0xFF;

			Set_All_Pixel_Color(Red,Green,Blue);	//���õƴ����еƵ� ��ɫΪ��ɫ
			DMA2_Start_SPI_Tx();	//��������
//			sleep(1);	//˯��1�룬�Լ�ʵ�ִ˺���
			SysTick_Delay_Ms(1000);
		}
	}

}
#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
