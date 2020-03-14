/*
 * DMA_M2M.h
 *
 *  Created on: 2020年2月28日
 *      Author: king
 */

#ifndef DMA_M2M_DMA_M2M_H_
#define DMA_M2M_DMA_M2M_H_


#include "stm32f10x.h"

//当使用存储器到存储器模式时，通道可以随便选择，没有硬性的规定
#define DMA_CHANNEL		DMA1_Channel6
#define DMA_CLK			RCC_AHBPeriph_DMA1

//传输完成标志
#define DMA_FLAG_TC		DMA1_FLAG_TC6

//要发送数据大小
#define BUFFER_SIZE 	32

extern const uint32_t aSRC_Const_Buffer[BUFFER_SIZE];
extern uint32_t aDST_Buffer[BUFFER_SIZE];

void DMA_Config(void);

uint8_t Buffercmp(const uint32_t* pBuffer,uint32_t* pBuffer1,uint16_t BufferLength);


#endif /* DMA_M2M_DMA_M2M_H_ */
