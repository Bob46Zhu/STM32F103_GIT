/*
 * MyMath.c
 *
 *  Created on: 2015年12月21日
 *      Author: yhh
 */

#include "stdbool.h"
#include "stm32f10x.h"


//判断一个点是否落在一个矩形中
bool PointInRect(u16 x,u16 y,u16 rx,u16 ry,u16 rw,u16 rh)
{
	return rx <= x && ry <= y && rx + rw > x && ry + rh > y;
}
