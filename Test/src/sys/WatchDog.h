/*
 * WatchDog.h
 *
 *  Created on: 2015Äê10ÔÂ17ÈÕ
 *      Author: yhh
 */

#ifndef SYS_WATCHDOG_H_
#define SYS_WATCHDOG_H_


#include "stm32f10x_iwdg.h"

#define KR_KEY_Reload IWDG->KR = ((u16)0xAAAA)  //Î¹¹·Óï¾ä

void WatchDog_Init();

#endif /* SYS_WATCHDOG_H_ */
