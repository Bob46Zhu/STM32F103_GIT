/*
 * SysTick_test.h
 *
 *  Created on: 2020年2月21日
 *      Author: king
 */

#ifndef SYSTICK_TEST_SYSTICK_TEST_H_
#define SYSTICK_TEST_SYSTICK_TEST_H_

#include "stm32f10x.h"

void SysTick_Init(void);
void Delay_us(__IO u32 nTime);
#define Delay_ms(x) Delay_us(100*x) //单位ms

void SysTick_Delay_Us(__IO uint32_t us);
void SysTick_Delay_Ms(__IO uint32_t ms);



#endif /* SYSTICK_TEST_SYSTICK_TEST_H_ */
