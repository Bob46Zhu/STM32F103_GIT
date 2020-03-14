/*
 * Usart_test.c
 *
 *  Created on: 2020��2��22��
 *      Author: king
 */

#include "Usart_test.h"
#include "misc.h"

// ����Ƕ�������жϿ�����NVIC
static void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	//Ƕ�������жϿ�������ѡ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//����USARTΪ�ж�Դ
	NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
	//�������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	//ʹ���ж�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//��ʼ������NVIC
	NVIC_Init(&NVIC_InitStructure);
}

void USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	//�򿪴��������ʱ��
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK,ENABLE);
	//�򿪴��������ʱ��
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK,ENABLE);
	//��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT,&GPIO_InitStructure);

	//��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT,&GPIO_InitStructure);

	//���ô��ڵĹ�������
	//���ò�����
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
	//����֡�����ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	//����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	//����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No;
	//����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	//���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//��ɴ��ڵĳ�ʼ������
	USART_Init(DEBUG_USARTx,&USART_InitStructure);

	//�����ж���������
//	NVIC_Configuration();

	//ʹ�ܴ��ڽ����ж�
//	USART_ITConfig(DEBUG_USARTx,USART_IT_RXNE,ENABLE);

	//ʹ�ܴ���
	USART_Cmd(DEBUG_USARTx,ENABLE);

}

/****************** ����һ���ֽ� *******************/
void Usart_SendByte(USART_TypeDef *pUSARTx,uint8_t ch)
{
	//����һ���ֽ����ݵ�USART
	USART_SendData(pUSARTx,ch);

	//�ȴ��������ݼĴ���Ϊ��
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE) == RESET);
}

/******************* ����8λ������ ******************/
void Usart_SendArray(USART_TypeDef *pUSARTx,uint8_t *array,uint16_t num)
{
	uint8_t i;
	for(i=0;i<num;i++)
	{
		//����һ���˽����ݵ�USART
		Usart_SendByte(pUSARTx,array[i]);
	}
	//�ȴ���������
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC) == RESET);
}

/******************* �����ַ��� ******************/
void Usart_SendString(USART_TypeDef *pUSARTx,char *str)
{
	unsigned int k =0;
	do
	{
		Usart_SendByte(pUSARTx,*(str + k));
		k++;
	}while(*(str + k) != '\0');

	//�ȴ��������
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC) == RESET);
}

/******************* ����һ��16λ�� ******************/
void Usart_SendHalfWord(USART_TypeDef *pUSART,uint16_t ch)
{
	uint8_t temp_h,temp_l;
	//ȡ���߰�λ
	temp_h = (ch&0xFF00)>>8;

	//ȡ���Ͱ�λ
	temp_l = (ch&0x00FF);

	//���͸߰�λ
	USART_SendData(pUSART,temp_h);
	while(USART_GetFlagStatus(pUSART,USART_FLAG_TXE) == RESET);

	//���͵Ͱ�λ
	USART_SendData(pUSART,temp_l);
	while(USART_GetFlagStatus(pUSART,USART_FLAG_TXE) == RESET);
}

//�ض���C�⺯�� scanf�����ڣ��ض�����ʹ��scanf,getchar�Ⱥ���
int fgetc(FILE *f)
{
	//�ȴ�������������
	while(USART_GetFlagStatus(DEBUG_USARTx,USART_FLAG_RXNE) == RESET);
	return (int)USART_ReceiveData(DEBUG_USARTx);
}