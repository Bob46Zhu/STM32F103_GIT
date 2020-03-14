/*
 * DMA_M2M.h
 *
 *  Created on: 2020��2��28��
 *      Author: king
 */

#ifndef DMA_M2M_DMA_M2M_H_
#define DMA_M2M_DMA_M2M_H_


#include "stm32f10x.h"

//��ʹ�ô洢�����洢��ģʽʱ��ͨ���������ѡ��û��Ӳ�ԵĹ涨
#define DMA_CHANNEL		DMA1_Channel6
#define DMA_CLK			RCC_AHBPeriph_DMA1

//������ɱ�־
#define DMA_FLAG_TC		DMA1_FLAG_TC6

//Ҫ�������ݴ�С
#define BUFFER_SIZE 	32

extern const uint32_t aSRC_Const_Buffer[BUFFER_SIZE];
extern uint32_t aDST_Buffer[BUFFER_SIZE];

void DMA_Config(void);

uint8_t Buffercmp(const uint32_t* pBuffer,uint32_t* pBuffer1,uint16_t BufferLength);


#endif /* DMA_M2M_DMA_M2M_H_ */
