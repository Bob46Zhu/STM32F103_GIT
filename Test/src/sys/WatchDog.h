/*
 * WatchDog.h
 *
 *  Created on: 2015��10��17��
 *      Author: yhh
 */

#ifndef SYS_WATCHDOG_H_
#define SYS_WATCHDOG_H_


#include "stm32f10x_iwdg.h"

#define KR_KEY_Reload IWDG->KR = ((u16)0xAAAA)  //ι�����

void WatchDog_Init();

#endif /* SYS_WATCHDOG_H_ */
