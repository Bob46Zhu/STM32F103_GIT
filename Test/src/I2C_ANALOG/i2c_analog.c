/*
 * i2c_analog.c
 *
 *  Created on: 2020年3月1日
 *      Author: king
 */

#include "i2c_analog.h"
#include "i2c_gpio.h"
#include "../USART_Test/Usart_test.h"

//判断 串行EEPROM是否正常

uint8_t ee_CheckOk(void)
{
	if(i2c_CheckDevice(EEPROM_DEV_ADDR) == 0)
	{
		return 1;
	}
	else
	{
		//失败后，切记发送I2C总线停止信号
		i2c_Stop();
		return 0;
	}
}

//从串行EEPROM指定地址处开始读取若干数据
uint8_t ee_ReadBytes(uint8_t *_pReadBuf,uint16_t _usAddress,uint16_t _usSize)
{
	uint16_t i;
	/*采用串行EEPROM随即读取指令序列，连续赢取若干字节*/
	//第一步：发起I2C总线启动信号
	i2c_Start();
	//第二步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读
	i2c_SendByte(EEPROM_DEV_ADDR | EEPROM_I2C_WR); //此处是写指令
	//第三步：等待ACK
	if(i2c_WaitAck() != 0)
	{
		goto cmd_fail; //EEPROM器件无应答
	}
	//第四步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址
	i2c_SendByte((uint8_t)_usAddress);
	//第五步：等待ACK
	if(i2c_WaitAck() != 0)
	{
		goto cmd_fail;	//EEPROM器件无应答
	}
	//第六步：重新启动I2C总线，前面的代码的目的身EEPROM传送地址，下面开始读取数据
	i2c_Start();
	//第七步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读
	i2c_SendByte(EEPROM_DEV_ADDR | EEPROM_I2C_RD); //此处是读指令
	//第八步：发送ACK
	if(i2c_WaitAck() != 0)
	{
		goto cmd_fail; //EEPROM器件无应答
	}
	//第九步：循环读取数据
	for(i = 0; i < _usSize; i++)
	{
		_pReadBuf[i] = i2c_ReadByte();	//读1个字节
		//每读完1个字节后，需要发送Ack，最后一个字节不需要Ack，发Nack
		if(i != _usSize - 1)
		{
			i2c_Ack();	//中间字节读完后，CPU产生ACK信号(驱动 SDA = 0)
		}
		else
		{
			i2c_NAck();	//最后1个字节读完后，CPU产生NACK信号(驱动 SDA=1)
		}
	}
	//发送I2C总线停止信号
	i2c_Stop();
	return 1; //执行成功

cmd_fail:	//命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备
	//发送I2C总线停止信号
	i2c_Stop();
	return 0;
}

//向串行EEPROM指定地址写入若干数据，采用页写操作提高效率
uint8_t ee_WriteBytes(uint8_t *_pWriteBuf,uint16_t _usAddress,uint16_t _usSize)
{
	uint16_t i,m;
	uint16_t usAddr;

	/*
	 * 写串行EEPROM不像读操作可以连续读取很多字节，每次写操作只能在同一个page。
	 * 对于24xx02，page size = 8
	 * 简单的处理方法为：按字节写操作模式，没写1个字节，都发送地址
	 * 为了提高连接写的效率：本函数彩用page write操作。
	 * */
	usAddr = _usAddress;
	for(i = 0; i < _usSize; i++)
	{
		//当发送第1个字节或是页面总地址时，需要重新发起启动信号和地址
		if((i == 0) || (usAddr & (EEPROM_PAGE_SIZE -1)) == 0)
		{
			//第0步：发停止信号，启动内部写操作
			i2c_Stop();

			//通过检查器件应答的方式，判断内部写操作是否完成，一般小于10ms
			//CLK频率为200KHz时，查询次数为30次左右
			for(m = 0; m < 1000; m++)
			{
				//第1步：发起I2C总线启动信号
				i2c_Start();

				//第二步：发起控制字节，高7位是地址，bit0是读写控制位，0表示写，1表示读
				i2c_SendByte(EEPROM_DEV_ADDR | EEPROM_I2C_WR); //此处是写指令

				//第三步：发送一个时钟，判断器件是否正确应答
				if(i2c_WaitAck() == 0)
				{
					break;
				}
			}
			if(m == 1000)
			{
				goto cmd_fail; //EEPROM器件写超时
			}
			//第四步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址
			i2c_SendByte((uint8_t)usAddr);
			//第五步：等待ACK
			if(i2c_WaitAck() != 0)
			{
				goto cmd_fail;	//EEPROM器件无应答
			}
		}
		//第六步：开始写入数据
		i2c_SendByte(_pWriteBuf[i]);
		//第七步：发送ACK
		if(i2c_WaitAck() != 0)
		{
			goto cmd_fail;	//EEPROM器件无应答
		}
		usAddr++;	//地址增1
	}
	//命令执行成功，发送I2C总线停止信号
	i2c_Stop();
	return 1;

cmd_fail:	//命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备
	//发送I2C总线停止信号
	i2c_Stop();
	return 0;
}

void ee_Erase(void)
{
	uint16_t i;
	uint8_t buf[EEPROM_SIZE];

	//填充缓冲区
	for(i = 0; i < EEPROM_SIZE; i++)
	{
		buf[i] = 0xFF;
	}

	//写EEPROM，起始地址 = 0，数据长度为256
	if(ee_WriteBytes(buf,0,EEPROM_SIZE) == 0)
	{
		Usart_SendString(DEBUG_USARTx,"擦除EEPROM出错！");
		return ;
	}
	else
	{
		Usart_SendString(DEBUG_USARTx,"擦除EEPROM成功!");
	}
}

static void ee_Delay(__IO uint32_t nCount) //简单的延时函数
{
	for(;nCount != 0;nCount--);
}

//EEPROM AT24C02 读写测试
//正常返回1，异常返回0
uint8_t ee_Test(void)
{
	uint16_t i;
	uint8_t write_buf[EEPROM_SIZE];
	uint8_t read_buf[EEPROM_SIZE];

	if(ee_CheckOk() == 0)
	{
		//没有检测到EEPROM
		Usart_SendString(DEBUG_USARTx,"没有检测到串行EEPROM!");
		return 0;
	}

	//填充测试缓冲区
	for(i = 0; i < EEPROM_SIZE; i++)
	{
		write_buf[i] = i;
	}

	if(ee_WriteBytes(write_buf,0,EEPROM_SIZE) == 0)
	{
		Usart_SendString(DEBUG_USARTx,"写EEPROM出错！");
		return 0;
	}
	else
	{
		Usart_SendString(DEBUG_USARTx,"写EEPROM成功！");
	}

	//写完之后需要适当的延时再去读，不然会出错
	ee_Delay(0x0FFFFF);

	if(ee_ReadBytes(read_buf,0,EEPROM_SIZE) == 0)
	{
		Usart_SendString(DEBUG_USARTx,"读EEPROM出错！");
		return 0;
	}
	else
	{
		Usart_SendString(DEBUG_USARTx,"读EEPROM成功，数据如下：\r\n");
	}

	for(i = 0; i < EEPROM_SIZE; i++)
	{
		if(read_buf[i] != write_buf[i])
		{
			Usart_SendByte(DEBUG_USARTx,read_buf[i]);
			Usart_SendString(DEBUG_USARTx,"错误：EEPROM读出与写入的数据不一致");
			return 0;
		}
		Usart_SendByte(DEBUG_USARTx,read_buf[i]);
		if((i & 15) == 15)
		{
			Usart_SendByte(DEBUG_USARTx,"\n");
		}
	}
	Usart_SendString(DEBUG_USARTx,"EEPROM读写测试成功\r\n");
	return 1;
}

