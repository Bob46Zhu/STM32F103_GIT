/*
 * WatchDog.c
 *
 *  Created on: 2015��10��17��
 *      Author: yhh
 */


#include "sys.h"
#include "stm32f10x.h"

/**
 * ��ʼ���������Ź�
 * prer:��Ƶ��:0~7(ֻ�е� 3 λ��Ч!)
 * ��Ƶ����=4*2^prer.�����ֵֻ���� 256!
 * rlr:��װ�ؼĴ���ֵ:�� 11 λ��Ч.
 * ʱ�����(���):Tout=((4*2^prer)*rlr)/40 (ms).
 */
static void IWDG_Init(u8 prer,u16 rlr)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); /* ʹ�ܶԼĴ���IWDG_PR��IWDG_RLR��д����*/
    IWDG_SetPrescaler(prer);    /*����IWDGԤ��Ƶֵ:����IWDGԤ��Ƶֵ*/
    IWDG_SetReload(rlr);     /*����IWDG��װ��ֵ*/
    IWDG_ReloadCounter();    /*����IWDG��װ�ؼĴ�����ֵ��װ��IWDG������*/
    DBGMCU->CR |= 0x100;  //����ʱ���Ź��ر�
    IWDG_Enable();        /*ʹ��IWDG*/

}


void WatchDog_Init()
{
	IWDG_Init(5,1000); //Լ3�룬���ǵ�eeprom�Ķ�д�����ȽϷ�ʱ
}
