/*
 * Usart_Dma.h
 *
 *  Created on: 2020��2��27��
 *      Author: king
 */

#ifndef USART_DMA_USART_DMA_H_
#define USART_DMA_USART_DMA_H_

#include "stm32f10x.h"


//��������������֮ǰ�Ĵ������Ѿ����壬���ﲻ�ٶ���

//���ڶ�Ӧ��DMA����ͨ��
#define USART_TX_DMA_CHANNEL		DMA1_Channel4
//����Ĵ�����ַ
#define USART_DR_ADDRESS			(USART1_BASE+0x04)
//һ�η��͵�������
#define SENDBUFF_SIZE				5000

extern uint8_t SendBuff[SENDBUFF_SIZE];

void USARTx_DMA_Config(void);


#endif /* USART_DMA_USART_DMA_H_ */
