/*
 * LED_Line.c
 *
 *  Created on: 2020��4��7��
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
unsigned char PixelBuffer[LED_COUNT*24+310] = {0};//���͵�����
void RGB_LED_Init(void)
{
	//�Ĵ������λ����һ�������������ٸ�ֵ���ȿ��Խ�Լ����ռ䣬���ܱ��ִ���Ŀ��Ķ���
		//RCC->APB2ENR|=1<<2;   //ʹ��PORTA��ʱ��
		//RCC->APB2ENR|=1<<3;//ʹ������PORTBʱ��
		//RCC->APB2ENR|=1<<4;//ʹ������PORTCʱ��
		//RCC->APB2ENR|=1<<5;//ʹ������PORTDʱ��
		//RCC->APB2ENR |= 1<<0; //��������ʱ��


//		const u32 APB2ENR = (1<<2) | (1<<3) | (1<<4) | (1<<5) | 1;
//		const u32 APB1ENR = 1<<15;
//		RCC->APB2ENR |= APB2ENR;
//		RCC->APB1ENR |= APB1ENR;

/******************** IOֱ��������� **************************************
		GPIOB->CRL &= 0xFFFFFFF0;
		GPIOB->CRL |= 0x00000003;//ͨ���������
***********************************************************/

/*********************************************************
 * 					SPI+DMA���� �Ĵ���
 ******************************************************* */

//		GPIOB->CRL &= 0xFF0FFFFF;
//		GPIOB->CRL |= 0x00B00000;//���ù����������

//		SPI3->CR1 &= 0x00000000;
//		SPI3->CR1 |= 2<<14;	//���߷���ģʽ
//		SPI3->CR1 |= 1<<11; //8λ���ݷ���
//		SPI3->CR1 |= 1<<2;	//����ģʽ
//		SPI3->CR1 &= 0<<1;	//����ʱ�����ֵ͵�ƽ
//		SPI3->CR1 |= 1<<0;  //���ݲ����ӵڶ���ʱ�ӱ��ؿ�ʼ
//		SPI3->CR1 |= 1<<3;  //4��Ƶ 36/4 = 9M
//		SPI3->CR1 &= 0<<7;	//�ȷ�MSB
//		SPI3->CR1 |= 1<<6;	//ʹ��SPI3
//
//		RCC->AHBENR |= 1<<1; //����DMA2ʱ��
//		DMA2_Channel2->CPAR = &SPI3->DR;
//		DMA2_Channel2->CMAR = (uint32_t)PixelBuffer;
//		DMA2_Channel2->CNDTR = 0;//���������������ʼ��ִ��
//		DMA2_Channel2->CCR = 0x00000000; //��λ
//		DMA2_Channel2->CCR &= ~(1<<4);	//�������
//		DMA2_Channel2->CCR|=0<<5;  		//��ͨģʽ
//		DMA2_Channel2->CCR|=0<<6; 		//�����ַ������ģʽ
//		DMA2_Channel2->CCR|=1<<7; 	 	//�洢������ģʽ
//		DMA2_Channel2->CCR|=0<<10; 		//�洢�����ݿ��8λ
//		DMA2_Channel2->CCR|=1<<12; 		//�е����ȼ�
//		DMA2_Channel2->CCR|=0<<14; 		//�Ǵ洢�����洢��ģʽ
//		DMA2_Channel2->CCR|=1<<1;        //ʹ���ж�
//		DMA2->IFCR|=1<<5;//���ͨ��2������ɱ�־
//
//		DMA2_Channel2->CCR |=   1<<0;          //����DMA����

/*********************************************************
 * 					SPI+DMA���� �⺯��
 ******************************************************* */
		GPIO_InitTypeDef GPIO_InitStructure;
		SPI_InitTypeDef SPI_InitStructure;
		DMA_InitTypeDef DMA_InitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
		//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB,&GPIO_InitStructure);	//��ʼ��SPI����IO��


		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,ENABLE);
		SPI_I2S_DeInit(SPI3);
		SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;	//SPI���߷���
		//SPI3->CR1 |= 2<<14;//SPI���߷���
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
		//SPI3->CR1 |= 1<<2;//���豸ģʽ
		//SPI3->CR1 |= 1<<8;//SSI�ڲ�������ѡ��ֻ��SSMλΪ'1'�������壬������NSS�ϵĵ�ƽ����NSS�����ϵ�IO������Ч
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	//�������ݿ��8bit
		//SPI3->CR1 &= ~(1<<11);//��������֡��� 8Bit
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
//		SPI3->CR1 &= ~(1<<1);	//�ռ�״̬ʱ��SCK���ֵ͵�ƽ
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
//		SPI3->CR1 |= 1<<0;//���ݲ����ӵڶ���ʱ�ӱ��ؿ�ʼ
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
//		SPI3->CR1 |= 1<<9;//SSM��������豸����
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;	//36/4=9M������1Bitʱ��=111ns
//		SPI3->CR1 |= 1<<3;//36/4=9M������1Bitʱ��=111ns
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
//		SPI3->CR1 &= ~(1<<7);//�ȷ���MSB
		SPI_InitStructure.SPI_CRCPolynomial = 10;
//		SPI3->CR1 &= ~(1<<13);//��ֹCRC����
		SPI_Init(SPI3,&SPI_InitStructure);
		SPI3->CR1 &= ((uint16_t)0xDFFF);	//��ֹCRC����


		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,ENABLE);
		DMA_DeInit(DMA2_Channel2);
		DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&SPI3->DR;	//�����ַ
//		DMA2_Channel2->CPAR = (u32)&SPI3->DR;//�����ַ
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;		//DMA���䷽��
//		DMA2_Channel2->CCR |= 1<<4;//DMA���䷽�򣬴Ӵ洢����
		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)PixelBuffer;
//		DMA2_Channel2->CMAR = (u32)PixelBuffer;
		DMA_InitStructure.DMA_BufferSize = 0;	//��Ҫ���͵Ĵ�СΪ0����ʼ��ִ�з��Ͳ���
//		DMA2_Channel2->CNDTR = 0;// ���������Ĵ���
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//		DMA2_Channel2->CCR &= ~(1<<6);//��ִ�������ַ��������
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//		DMA2_Channel2->CCR |= 1<<7;//ִ�д洢����ַ��������
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//����Ϊ��������8bit��
//		DMA2_Channel2->CCR &= ~(3<<8);//�������ݿ��8bit
		DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;	//RAM�洢����8Bit��
//		DMA2_Channel2->CCR &= ~(3<<10);//�洢�����ݿ��8bit
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;

		DMA_InitStructure.DMA_Priority = DMA_Priority_High;
//		DMA2_Channel2->CCR |= 2<<12;//ͨ�����ȼ�����
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		DMA2_Channel2->CCR &= ~(1<<14);//�Ǵ洢�����洢��
		DMA_Init(DMA2_Channel2,&DMA_InitStructure);

		SPI_I2S_DMACmd(SPI3,SPI_I2S_DMAReq_Tx,ENABLE);	//ʹ��SPIʹ��DMAͨ������
//		SPI3->CR2 |= 1<<1;//ʹ��SPIʹ��DMAͨ�����ͣ�
		SPI_Cmd(SPI3,ENABLE);		//ʹ��SPI������
//		SPI3->CR1 |= 1<<6;//ʹ��SPI



}

void DMA2_Start_SPI_Tx()
{
	DMA2_Channel2->CNDTR = (LED_COUNT*24+310);
	DMA2_Channel2->CMAR = (uint32_t)PixelBuffer;

	DMA_Cmd(DMA2_Channel2,ENABLE);	//ʹ��SPI3 DMA����
//	DMA2_Channel2->CCR = 1<<0; //ʹ��SPI3 DMA����

	while(!DMA_GetFlagStatus(DMA2_FLAG_TC2));	//ѭ���ȴ�������ɣ���ʱ������жϴ�ϣ�����Ӱ�췢��
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
	unsigned int positionin= position*3;	//һ������3����ɫ
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

	positionin = position * 8;	//һ�������ϵ�һ����ɫ����Ҫ8λ����ʾ����

	//��������bits�Ǳ�ʾ��ɫ��ȵ�8λ�������磺��������һ����ɫ�����0x55 ����0101 0101
	//��ÿһ����ɫλ��תΪ0���1�����PixelBuffer�����С���һ����ɫ����Ȼ�ռ�����鳤��Ϊ8��һ������3����ɫ��ռ�����鳤��24��
	//��64�����飬������Ҫ���鳤��Ϊ64*24 + 310�� 0��

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
	RGB_LED_HIGH;   //ʾ����ʵ�⣬200ns����
	//delay_led_line(1); //1/72M=13.8888ns
	RGB_LED_LOW;	//ʾ����ʵ�⣬780ns����
	delay_led_line(1);
}

static void RGB_LED_Write_1(void)
{
	RGB_LED_HIGH;	//ʾ����ʵ�⣬780ns����
	delay_led_line(1);
	RGB_LED_LOW;	//ʾ����ʵ�⣬200ns����
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

//������ɫ�趨��������ɫ�Դ�����
void RGB_LED_Red(void)
{
	uint8_t i;
	//8LEDȫ�ʵ�
	for(i=0;i<16;i++)
	{
		RGB_LED_Write_24Bits(0,0xff,0);
	}
}

void RGB_LED_Green(void)
{
	uint8_t i;
	//8LEDȫ�ʵ�
	for(i=0;i<16;i++)
	{
		RGB_LED_Write_24Bits(0xff,0,0);
	}
}

void RGB_LED_Blue(void)
{
	uint8_t i;
	//8LEDȫ�ʵ�
	for(i=0;i<60;i++)
	{
		RGB_LED_Write_24Bits(0,0,0xff);
	}
}
