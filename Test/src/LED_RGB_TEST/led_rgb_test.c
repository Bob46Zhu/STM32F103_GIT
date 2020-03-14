#include "led_rgb_test.h"

#define SOFT_DELAY Delay(0xFFFFFF)
void Delay(__IO uint32_t nCount);

void LED_GPIO_Config(void)
{
	//定义一个GPIO_InitTypeDef类型的结构体
	GPIO_InitTypeDef GPIO_InitStructure;

	//开启LEdge相关的GPIO外设时钟
	RCC_APB2PeriphClockCmd(LED1_GPIO_CLK | LED2_GPIO_CLK | LED3_GPIO_CLK,ENABLE);
	//选择要控制的GPIO引脚

	//LED1
	GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN;
	//设置引脚模式为通用推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	//设置引脚速率为50MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//初始化GPIO
	GPIO_Init(LED1_GPIO_PORT,&GPIO_InitStructure);

	//LED2
	GPIO_InitStructure.GPIO_Pin = LED2_GPIO_PIN;
	//设置引脚模式为通用推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	//设置引脚速率为50MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//初始化GPIO
	GPIO_Init(LED2_GPIO_PORT,&GPIO_InitStructure);

	//LED3
	GPIO_InitStructure.GPIO_Pin = LED3_GPIO_PIN;
	//设置引脚模式为通用推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	//设置引脚速率为50MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//初始化GPIO
	GPIO_Init(LED3_GPIO_PORT,&GPIO_InitStructure);

	//关闭所有LED灯
	GPIO_SetBits(LED1_GPIO_PORT,LED1_GPIO_PIN);
	GPIO_SetBits(LED2_GPIO_PORT,LED2_GPIO_PIN);
	GPIO_SetBits(LED3_GPIO_PORT,LED3_GPIO_PIN);

}

void LED_Corlor_test(void)
{

	//LED 测试
	LED1_ON;
	SOFT_DELAY;
	LED1_OFF;

	LED2_ON;
	SOFT_DELAY;
	LED2_OFF;

	LED3_ON;
	SOFT_DELAY;
	LED3_OFF;

	LED1_OFF;
	LED2_OFF;
	SOFT_DELAY;

	LED_YELLOW;
	SOFT_DELAY;

	LED_PURPLE;
	SOFT_DELAY;

	LED_CYAN;
	SOFT_DELAY;

	LED_WHITE;
	SOFT_DELAY;

	LED_RGBOFF;
	SOFT_DELAY;

}

void assert_failed(uint8_t* file, uint32_t line)
{
	// 断言错误时执行的代码
	LED1_ON;
}

void Delay(__IO uint32_t nCount)
{
	for(;nCount !=0;nCount--);
}
