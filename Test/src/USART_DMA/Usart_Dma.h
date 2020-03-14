/*
 * Usart_Dma.h
 *
 *  Created on: 2020年2月27日
 *      Author: king
 */

#ifndef USART_DMA_USART_DMA_H_
#define USART_DMA_USART_DMA_H_

#include "stm32f10x.h"


//串口引脚配置在之前的串口中已经定义，这里不再定义

//串口对应的DMA请求通道
#define USART_TX_DMA_CHANNEL		DMA1_Channel4
//外设寄存器地址
#define USART_DR_ADDRESS			(USART1_BASE+0x04)
//一次发送的数据量
#define SENDBUFF_SIZE				5000

extern uint8_t SendBuff[SENDBUFF_SIZE];

void USARTx_DMA_Config(void);


#endif /* USART_DMA_USART_DMA_H_ */
