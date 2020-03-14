#include "led_rgb_test.h"

#define SOFT_DELAY Delay(0xFFFFFF)
void Delay(__IO uint32_t nCount);

void LED_GPIO_Config(void)
{
	//����һ��GPIO_InitTypeDef���͵Ľṹ��
	GPIO_InitTypeDef GPIO_InitStructure;

	//����LEdge��ص�GPIO����ʱ��
	RCC_APB2PeriphClockCmd(LED1_GPIO_CLK | LED2_GPIO_CLK | LED3_GPIO_CLK,ENABLE);
	//ѡ��Ҫ���Ƶ�GPIO����

	//LED1
	GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN;
	//��������ģʽΪͨ���������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	//������������Ϊ50MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//��ʼ��GPIO
	GPIO_Init(LED1_GPIO_PORT,&GPIO_InitStructure);

	//LED2
	GPIO_InitStructure.GPIO_Pin = LED2_GPIO_PIN;
	//��������ģʽΪͨ���������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	//������������Ϊ50MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//��ʼ��GPIO
	GPIO_Init(LED2_GPIO_PORT,&GPIO_InitStructure);

	//LED3
	GPIO_InitStructure.GPIO_Pin = LED3_GPIO_PIN;
	//��������ģʽΪͨ���������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	//������������Ϊ50MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//��ʼ��GPIO
	GPIO_Init(LED3_GPIO_PORT,&GPIO_InitStructure);

	//�ر�����LED��
	GPIO_SetBits(LED1_GPIO_PORT,LED1_GPIO_PIN);
	GPIO_SetBits(LED2_GPIO_PORT,LED2_GPIO_PIN);
	GPIO_SetBits(LED3_GPIO_PORT,LED3_GPIO_PIN);

}

void LED_Corlor_test(void)
{

	//LED ����
	LED1_ON;
	SOFT_DELAY;
	LED1_OFF;

	LED2_ON;
	SOFT_DELAY;
	LED2_OFF;

	LED3_ON;
	SOFT_DELAY;
	LED3_OFF;

	LED1_OFF;
	LED2_OFF;
	SOFT_DELAY;

	LED_YELLOW;
	SOFT_DELAY;

	LED_PURPLE;
	SOFT_DELAY;

	LED_CYAN;
	SOFT_DELAY;

	LED_WHITE;
	SOFT_DELAY;

	LED_RGBOFF;
	SOFT_DELAY;

}

void assert_failed(uint8_t* file, uint32_t line)
{
	// ���Դ���ʱִ�еĴ���
	LED1_ON;
}

void Delay(__IO uint32_t nCount)
{
	for(;nCount !=0;nCount--);
}
