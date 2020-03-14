/*
 * SysTick_test.c
 *
 *  Created on: 2020年2月21日
 *      Author: king
 */

#include "SysTick_test.h"
#include "core_cm3.h"
#include "misc.h"

static __IO u32 TimingDelay;

//启动系统滴答定时器SysTick

void SysTick_Init(void)
{
	// SystemFrequency / 1000		1ms中断一次
	// SystemFrequency / 100000		10us中断一次
	// SystemFrequency / 1000000	1us中断一次
	if(SysTick_Config(SystemCoreClock/100000)) //
	{
		while(1);
	}
}

//us延时程序，10us为一个单位
void Delay_us(__IO u32 nTime)
{
	TimingDelay = nTime;

	//使能滴答定时器
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

	while(TimingDelay != 0);

}

//获取节拍程序
void TimingDelay_Decrement(void)
{
	if(TimingDelay != 0x00)
	{
		TimingDelay--;
	}
}

#if 0
// 这个固件库函数在Core_cm3.中
static __INLINE uint32_t SysTick_Config(uint32_t ticks)
{
	//reload寄存器为24bit,最大值为2^24
	if(ticks > SysTick_LOAD_RELOAD_Msk)
		return (1);

	//配置reload寄存器的初始值
	SysTick->LOAD = (ticks & SysTick_LOAD_RELOAD_Msk) - 1;

	//配置中断优先级为 1<<4-1 = 15,优先级为最低
	NVIC_SetPriority(SysTick_IRQn,(1<<__NVIC_PRIO_BITS) - 1);

	//配置 counter计数器的值
	SysTick->VAL = 0;

	//配置systick 的时钟为72M
	//使能中断
	//使能systick
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
					SysTick_CTRL_TICKINT_Msk   |
					SysTick_CTRL_ENABLE_Msk;
	return (0);
}

#endif

// counter减1的时间 等于1/systick_clk
// 当counter从reload的值减小到0的时候，为一个循环，如果开启了中断则执行中断服务程序
// 同时CTRL的countflag位会置1
// 这一个循环的时间为reload * (1/systick_clk)

void SysTick_Delay_Us(__IO uint32_t us)
{
	uint32_t i;
	SysTick_Config(SystemCoreClock/1000000);

	for(i=0;i<us;i++)
	{
		//当计数器的值减小到0的时候，CTRL寄存器的位16置1
		while(!((SysTick->CTRL)&(1<<16)));
	}
	//关闭SysTick定时器
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;

}

void SysTick_Delay_Ms(__IO uint32_t ms)
{
	uint32_t i;
	SysTick_Config(SystemCoreClock/1000);

	for(i=0;i<ms;i++)
	{
		//当计数器的值 减小到0的时候，CTRL寄存器的位16会置1
		//当置1时，读取该位会清0
		while(!((SysTick->CTRL)&(1<<16)));
	}
	//关闭SysTick定时器
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}


