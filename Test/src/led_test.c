/*
 * led_test.c
 *
 *  Created on: 2020��2��17��
 *      Author: king
 */




//RCC->APB2ENR |= (1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|1; //����GPIOA��B��C��D��E�͸��ù���ʱ��
//
//	//PA3//���죬����485оƬ�ķ������
//	GPIOB->CRL&=0XFF0F0FF0;//IO״̬����
//	GPIOB->CRL|=0X00101001;//ͨ�����������10M
//
//
//	while (1)
//	{
//		  PBout(1) = 0;
//		  timer_sleep(500);
//		  PBout(0) = 1;
//		  PBout(1) = 1;
//		  PBout(5) = 1;
//		  timer_sleep(500);
//		  PBout(0) = 0;
//		  timer_sleep(500);
//		  PBout(0) = 1;
//		  PBout(1) = 1;
//		  PBout(5) = 1;
//		  timer_sleep(500);
//		  PBout(5) = 0;
//		  timer_sleep(500);
//		  PBout(0) = 1;
//		  PBout(1) = 1;
//		  PBout(5) = 1;
//		  timer_sleep(500);
//	}
