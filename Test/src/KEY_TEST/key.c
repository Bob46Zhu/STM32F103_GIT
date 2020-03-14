/*
 * key.c
 *
 *  Created on: 2020年2月21日
 *      Author: king
 */

#include "key.h"

void KEY_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//开启按键端口GPIO时钟
	RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK | KEY2_GPIO_CLK,ENABLE);

	//选择按键的引脚
	GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN;
	//设置按键的引脚为浮空输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	//初始化按键
	GPIO_Init(KEY1_GPIO_PORT,&GPIO_InitStructure);

	//选择按键引脚
	GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN;
	//设置按键的引脚为浮空输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	//初始化按键
	GPIO_Init(KEY2_GPIO_PORT,&GPIO_InitStructure);

}

uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{
	//检测是否有按键按下
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON)
	{
		//等待按键释放
		while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON);
		return KEY_ON;
	}
	else
	{
		return KEY_OFF;
	}
}


