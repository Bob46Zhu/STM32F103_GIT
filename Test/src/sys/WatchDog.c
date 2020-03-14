/*
 * WatchDog.c
 *
 *  Created on: 2015年10月17日
 *      Author: yhh
 */


#include "sys.h"
#include "stm32f10x.h"

/**
 * 初始化独立看门狗
 * prer:分频数:0~7(只有低 3 位有效!)
 * 分频因子=4*2^prer.但最大值只能是 256!
 * rlr:重装载寄存器值:低 11 位有效.
 * 时间计算(大概):Tout=((4*2^prer)*rlr)/40 (ms).
 */
static void IWDG_Init(u8 prer,u16 rlr)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); /* 使能对寄存器IWDG_PR和IWDG_RLR的写操作*/
    IWDG_SetPrescaler(prer);    /*设置IWDG预分频值:设置IWDG预分频值*/
    IWDG_SetReload(rlr);     /*设置IWDG重装载值*/
    IWDG_ReloadCounter();    /*按照IWDG重装载寄存器的值重装载IWDG计数器*/
    DBGMCU->CR |= 0x100;  //调试时看门狗关闭
    IWDG_Enable();        /*使能IWDG*/

}


void WatchDog_Init()
{
	IWDG_Init(5,1000); //约3秒，考虑到eeprom的读写操作比较费时
}
