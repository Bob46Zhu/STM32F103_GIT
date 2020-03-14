/*
 * key.h
 *
 *  Created on: 2020��2��21��
 *      Author: king
 */

#ifndef KEY_TEST_KEY_H_
#define KEY_TEST_KEY_H_

#include "stm32f10x.h"

//���Ŷ���
#define KEY1_GPIO_CLK		RCC_APB2Periph_GPIOA
#define KEY1_GPIO_PORT		GPIOA
#define KEY1_GPIO_PIN		GPIO_Pin_0

#define KEY2_GPIO_CLK		RCC_APB2Periph_GPIOC
#define KEY2_GPIO_PORT		GPIOC
#define KEY2_GPIO_PIN		GPIO_Pin_13

/**
 * �������±�־��
 * ��������Ϊ�ߵ�ƽ������ KEY_ON = 1��KEY_OFF = 0;
 * ����������Ϊ�͵�ƽ���Ѻ����ó�KEY_ON=0,KEY_OFF=1;����
 * */
#define KEY_ON 1
#define KEY_OFF 0

void KEY_GPIO_Config(void);
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);



#endif /* KEY_TEST_KEY_H_ */
