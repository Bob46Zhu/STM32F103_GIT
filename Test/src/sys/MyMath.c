/*
 * MyMath.c
 *
 *  Created on: 2015��12��21��
 *      Author: yhh
 */

#include "stdbool.h"
#include "stm32f10x.h"


//�ж�һ�����Ƿ�����һ��������
bool PointInRect(u16 x,u16 y,u16 rx,u16 ry,u16 rw,u16 rh)
{
	return rx <= x && ry <= y && rx + rw > x && ry + rh > y;
}
