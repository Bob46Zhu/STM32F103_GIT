

#ifndef MYMATH_H_
#define MYMATH_H_

#include "../ADC.h"
#include "stdbool.h"
#include "stm32f10x.h"

//取符号
#define SIGN(A) ((A) > 0 ? 1 : ( (A) < 0 ? - 1 : 0))

//取最大值
#define MAX(A,B) ((A) > (B) ? (A) : (B))

//取最小值
#define MIN(A,B) ((A) > (B) ? (B) : (A))

//平方
#define POWER(A) ((A)*(A))

//10的n次幂,计算10的 0 到 4 次幂，由于系统的pow函数是double类型的，计算比较占资源，
//而10的整数次幂是很简单的运算，故用这个函数简单处理，节省资源
#define POWER10_3(A) ((A) == 3 ? 1000 : 10000)
#define POWER10_2(A) ((A) == 2 ? 100 : POWER10_3(A))
#define POWER10_1(A) ((A) == 1 ? 10 : POWER10_2(A))
#define POWER10(A) ((A) == 0 ? 1 : POWER10_1(A))

//正弦、余弦120度、240度的IQ30的值，用于三相电机计算
#define SIN120_IQ30 929887696L
#define SIN240_IQ30 -929887696L
#define COS120_IQ30 -536870912L
#define COS240_IQ30 -536870912L

#define SIN120f 0.86602540378443864676372317075294f
#define SIN240f -0.86602540378443864676372317075294f
#define COS120f -0.5f
#define COS240f -0.5f
//开尔文和摄氏度的转换用
#define KELVIN 273u
#define KELVIN_FLOAT 273.15f



#define RND_SEED ADC[ADC_CPU_TEMP]


#define GET_RND_U8(A) \
	A <<= 3;\
	A |= (RND_SEED & 0x7);\
	A &= 0xFF;

#define GET_RND_IQ15(A) \
	A <<= 2;\
	A |= (RND_SEED & 0x3);\
	if(A > 32768) A -= 32768;


extern bool PointInRect(u16 x,u16 y,u16 rx,u16 ry,u16 rw,u16 rh);

#endif /* MATH_H_ */
