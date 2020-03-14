/*
 * led_test.c
 *
 *  Created on: 2020年2月17日
 *      Author: king
 */




//RCC->APB2ENR |= (1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|1; //开启GPIOA、B、C、D、E和复用功能时钟
//
//	//PA3//推挽，用于485芯片的方向控制
//	GPIOB->CRL&=0XFF0F0FF0;//IO状态设置
//	GPIOB->CRL|=0X00101001;//通用推挽输出，10M
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
