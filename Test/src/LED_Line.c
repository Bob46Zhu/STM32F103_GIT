/*
 * LED_Line.c
 *
 *  Created on: 2020年4月7日
 *      Author: king
 */
#include "LED_Line.h"

static void delay_led_line(uint8_t time)
{
	while(time>0)
	{
		time--;
	}
}
unsigned char PixelBuffer[LED_COUNT*24+310] = {0};//发送的数据
void RGB_LED_Init(void)
{
	//寄存器多个位先用一个变量合起来再赋值，既可以节约代码空间，又能保持代码的可阅读性
		//RCC->APB2ENR|=1<<2;   //使能PORTA口时钟
		//RCC->APB2ENR|=1<<3;//使能外设PORTB时钟
		//RCC->APB2ENR|=1<<4;//使能外设PORTC时钟
		//RCC->APB2ENR|=1<<5;//使能外设PORTD时钟
		//RCC->APB2ENR |= 1<<0; //开启辅助时钟


//		const u32 APB2ENR = (1<<2) | (1<<3) | (1<<4) | (1<<5) | 1;
//		const u32 APB1ENR = 1<<15;
//		RCC->APB2ENR |= APB2ENR;
//		RCC->APB1ENR |= APB1ENR;

/******************** IO直接输出控制 **************************************
		GPIOB->CRL &= 0xFFFFFFF0;
		GPIOB->CRL |= 0x00000003;//通用推挽输出
***********************************************************/

/*********************************************************
 * 					SPI+DMA控制 寄存器
 ******************************************************* */

//		GPIOB->CRL &= 0xFF0FFFFF;
//		GPIOB->CRL |= 0x00B00000;//复用功能推挽输出

//		SPI3->CR1 &= 0x00000000;
//		SPI3->CR1 |= 2<<14;	//单线发送模式
//		SPI3->CR1 |= 1<<11; //8位数据发送
//		SPI3->CR1 |= 1<<2;	//主机模式
//		SPI3->CR1 &= 0<<1;	//空闲时，保持低电平
//		SPI3->CR1 |= 1<<0;  //数据采样从第二个时钟边沿开始
//		SPI3->CR1 |= 1<<3;  //4分频 36/4 = 9M
//		SPI3->CR1 &= 0<<7;	//先发MSB
//		SPI3->CR1 |= 1<<6;	//使能SPI3
//
//		RCC->AHBENR |= 1<<1; //开启DMA2时钟
//		DMA2_Channel2->CPAR = &SPI3->DR;
//		DMA2_Channel2->CMAR = (uint32_t)PixelBuffer;
//		DMA2_Channel2->CNDTR = 0;//传输的数据量，初始不执行
//		DMA2_Channel2->CCR = 0x00000000; //复位
//		DMA2_Channel2->CCR &= ~(1<<4);	//从外设读
//		DMA2_Channel2->CCR|=0<<5;  		//普通模式
//		DMA2_Channel2->CCR|=0<<6; 		//外设地址非增量模式
//		DMA2_Channel2->CCR|=1<<7; 	 	//存储器增量模式
//		DMA2_Channel2->CCR|=0<<10; 		//存储器数据宽度8位
//		DMA2_Channel2->CCR|=1<<12; 		//中等优先级
//		DMA2_Channel2->CCR|=0<<14; 		//非存储器到存储器模式
//		DMA2_Channel2->CCR|=1<<1;        //使能中断
//		DMA2->IFCR|=1<<5;//清除通道2传输完成标志
//
//		DMA2_Channel2->CCR |=   1<<0;          //开启DMA传输

/*********************************************************
 * 					SPI+DMA控制 库函数
 ******************************************************* */
		GPIO_InitTypeDef GPIO_InitStructure;
		SPI_InitTypeDef SPI_InitStructure;
		DMA_InitTypeDef DMA_InitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
		//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB,&GPIO_InitStructure);	//初始化SPI发送IO口


		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,ENABLE);
		SPI_I2S_DeInit(SPI3);
		SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;	//SPI单线发送
		//SPI3->CR1 |= 2<<14;//SPI单线发送
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
		//SPI3->CR1 |= 1<<2;//主设备模式
		//SPI3->CR1 |= 1<<8;//SSI内部从设置选择，只有SSM位为'1'才有意义，它决定NSS上的电平，在NSS引脚上的IO操作无效
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	//发送数据宽度8bit
		//SPI3->CR1 &= ~(1<<11);//发送数据帧宽度 8Bit
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
//		SPI3->CR1 &= ~(1<<1);	//空间状态时，SCK保持低电平
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
//		SPI3->CR1 |= 1<<0;//数据采样从第二个时钟边沿开始
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
//		SPI3->CR1 |= 1<<9;//SSM，软件从设备管理
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;	//36/4=9M，则传输1Bit时间=111ns
//		SPI3->CR1 |= 1<<3;//36/4=9M，则传输1Bit时间=111ns
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
//		SPI3->CR1 &= ~(1<<7);//先发送MSB
		SPI_InitStructure.SPI_CRCPolynomial = 10;
//		SPI3->CR1 &= ~(1<<13);//禁止CRC计算
		SPI_Init(SPI3,&SPI_InitStructure);
		SPI3->CR1 &= ((uint16_t)0xDFFF);	//禁止CRC发送


		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,ENABLE);
		DMA_DeInit(DMA2_Channel2);
		DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&SPI3->DR;	//外设地址
//		DMA2_Channel2->CPAR = (u32)&SPI3->DR;//外设地址
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;		//DMA传输方向
//		DMA2_Channel2->CCR |= 1<<4;//DMA传输方向，从存储器读
		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)PixelBuffer;
//		DMA2_Channel2->CMAR = (u32)PixelBuffer;
		DMA_InitStructure.DMA_BufferSize = 0;	//需要发送的大小为0，初始不执行发送操作
//		DMA2_Channel2->CNDTR = 0;// 传输数量寄存器
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//		DMA2_Channel2->CCR &= ~(1<<6);//不执行外设地址增量操作
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//		DMA2_Channel2->CCR |= 1<<7;//执行存储器地址增量操作
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//外设为发送数据8bit宽
//		DMA2_Channel2->CCR &= ~(3<<8);//外设数据宽度8bit
		DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;	//RAM存储数据8Bit宽
//		DMA2_Channel2->CCR &= ~(3<<10);//存储器数据宽度8bit
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;

		DMA_InitStructure.DMA_Priority = DMA_Priority_High;
//		DMA2_Channel2->CCR |= 2<<12;//通道优先级：高
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		DMA2_Channel2->CCR &= ~(1<<14);//非存储器到存储器
		DMA_Init(DMA2_Channel2,&DMA_InitStructure);

		SPI_I2S_DMACmd(SPI3,SPI_I2S_DMAReq_Tx,ENABLE);	//使能SPI使用DMA通道发送
//		SPI3->CR2 |= 1<<1;//使能SPI使用DMA通道发送；
		SPI_Cmd(SPI3,ENABLE);		//使能SPI控制器
//		SPI3->CR1 |= 1<<6;//使能SPI



}

void DMA2_Start_SPI_Tx()
{
	DMA2_Channel2->CNDTR = (LED_COUNT*24+310);
	DMA2_Channel2->CMAR = (uint32_t)PixelBuffer;

	DMA_Cmd(DMA2_Channel2,ENABLE);	//使能SPI3 DMA发送
//	DMA2_Channel2->CCR = 1<<0; //使能SPI3 DMA发送

	while(!DMA_GetFlagStatus(DMA2_FLAG_TC2));	//循环等待发送完成，此时如果被中断打断，并不影响发送
	DMA_Cmd(DMA2_Channel2,DISABLE);
	DMA_ClearFlag(DMA2_FLAG_TC2);
	return;
}

void Set_All_Pixel_Color(uint8_t r,uint8_t g,uint8_t b)
{
	int i = 0;
	for(i = 0;i < LED_COUNT; i++) //
	{
		Ws281x_Set_Pixel(Color_Show(r,g,b),i);
	}
}

void Set_Flow_Pixel(uint8_t r,uint8_t g,uint8_t b)
{
	Ws281x_Set_Pixel(Color_Show(r,g,b),2);
}

void Ws281x_Set_Pixel(uint32_t color,uint32_t position)
{
	unsigned int positionin= position*3;	//一个灯珠3种颜色
	uint8_t Red,Green,Blue;
	Red		= color >> 16;
	Green	= color >> 8;
	Blue	= color;

	Ws281x_Set_Bits(Green,positionin);
	Ws281x_Set_Bits(Red,positionin + 1);
	Ws281x_Set_Bits(Blue,positionin + 2);
}

void Ws281x_Set_Bits(uint8_t bits,uint32_t position)
{
	unsigned int positionin = 0;
	int zero = 0xE0;	//1110 0000
	int one  = 0xFC;	//1111 1100
	int i = 0x00;
	int j = 0x00;

	positionin = position * 8;	//一个灯珠上的一种颜色，需要8位数表示亮度

	//传进来的bits是表示颜色深度的8位数。例如：传进来第一个颜色深度是0x55 即：0101 0101
	//将每一个颜色位的转为0码或1码存在PixelBuffer数组中。即一个颜色的深度会占用数组长度为8，一个灯珠3个颜色共占用数组长度24个
	//共64个灯珠，所以需要数组长宽为64*24 + 310个 0码

	for(i = 0x80;i >= 0x01; i >>= 1)
	{
		PixelBuffer[positionin + j] = ((bits & i)?one:zero);
		j++;
	}
}

uint32_t Color_Show(uint8_t r,uint8_t g,uint8_t b)
{
	return ((uint32_t)r<<16) | ((uint32_t)g<<8)|b;
}

static void RGB_LED_Write_0(void)
{
	RGB_LED_HIGH;   //示波器实测，200ns左右
	//delay_led_line(1); //1/72M=13.8888ns
	RGB_LED_LOW;	//示波器实测，780ns左右
	delay_led_line(1);
}

static void RGB_LED_Write_1(void)
{
	RGB_LED_HIGH;	//示波器实测，780ns左右
	delay_led_line(1);
	RGB_LED_LOW;	//示波器实测，200ns左右
	//delay_led_line(1);
}


void RGB_LED_Reset(void)
{
	RGB_LED_LOW;
	SysTick_Delay_Us(10);
}

void RGB_LED_Write_Byte(uint8_t byte)
{
	uint8_t i;
	for(i=0;i<8;i++)
	{
		if(byte & 0x80)
		{
			RGB_LED_Write_1();
		}
		else
		{
			RGB_LED_Write_0();
		}
		byte <<= 1;
	}
}

void RGB_LED_Write_24Bits(uint8_t green,uint8_t red,uint8_t blue)
{
	RGB_LED_Write_Byte(green);
	RGB_LED_Write_Byte(red);
	RGB_LED_Write_Byte(blue);
}

//亮灯颜色设定，其他颜色以此类推
void RGB_LED_Red(void)
{
	uint8_t i;
	//8LED全彩灯
	for(i=0;i<16;i++)
	{
		RGB_LED_Write_24Bits(0,0xff,0);
	}
}

void RGB_LED_Green(void)
{
	uint8_t i;
	//8LED全彩灯
	for(i=0;i<16;i++)
	{
		RGB_LED_Write_24Bits(0xff,0,0);
	}
}

void RGB_LED_Blue(void)
{
	uint8_t i;
	//8LED全彩灯
	for(i=0;i<60;i++)
	{
		RGB_LED_Write_24Bits(0,0,0xff);
	}
}
