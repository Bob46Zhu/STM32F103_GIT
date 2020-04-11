/*
 * LED_Line.h
 *
 *  Created on: 2020年4月7日
 *      Author: king
 */

#ifndef LED_LINE_H_
#define LED_LINE_H_

#include "stm32f10x.h"
#include "sys/sys.h"
#include "SysTick_test/SysTick_test.h"

#define RGB_LED_HIGH	PBout(0) = 1
#define RGB_LED_LOW		PBout(0) = 0

#define LED_COUNT 32 //需要控制LED的个数

void RGB_LED_Init(void);
void RGB_LED_Reset(void);
void RGB_LED_Write_Byte(uint8_t byte);
void RGB_LED_Write_24Bits(uint8_t green,uint8_t red,uint8_t blue);
void RGB_LED_Red(void);
void RGB_LED_Green(void);
void RGB_LED_Blue(void);

//库函数版本
void DMA2_Start_SPI_Tx();
void Set_All_Pixel_Color(uint8_t r,uint8_t g,uint8_t b);
void Set_Flow_Pixel(uint8_t r,uint8_t g,uint8_t b);
void Ws281x_Set_Pixel(uint32_t color,uint32_t position);
void Ws281x_Set_Bits(uint8_t bits,uint32_t position);
uint32_t Color_Show(uint8_t r,uint8_t g,uint8_t b);


#endif /* LED_LINE_H_ */
