/*
 * sys.c
 *
 *  Created on: 2015Äê9ÔÂ1ÈÕ
 *      Author: yhh
 */

#include "stm32f10x.h"
#include "sys.h"

void SystemReset(void)
{
	asm volatile(
		      " MOV R0, #1 \n"
		      " MSR FAULTMASK,R0 \n"
		      " LDR R0, =0xE000ED0C    \n"
		      " LDR R1, =0x05FA0004    \n"
		      " STR R1, [R0]       \n"
			);

	while(1);
}
