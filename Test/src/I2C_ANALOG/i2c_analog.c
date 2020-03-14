/*
 * i2c_analog.c
 *
 *  Created on: 2020��3��1��
 *      Author: king
 */

#include "i2c_analog.h"
#include "i2c_gpio.h"
#include "../USART_Test/Usart_test.h"

//�ж� ����EEPROM�Ƿ�����

uint8_t ee_CheckOk(void)
{
	if(i2c_CheckDevice(EEPROM_DEV_ADDR) == 0)
	{
		return 1;
	}
	else
	{
		//ʧ�ܺ��мǷ���I2C����ֹͣ�ź�
		i2c_Stop();
		return 0;
	}
}

//�Ӵ���EEPROMָ����ַ����ʼ��ȡ��������
uint8_t ee_ReadBytes(uint8_t *_pReadBuf,uint16_t _usAddress,uint16_t _usSize)
{
	uint16_t i;
	/*���ô���EEPROM�漴��ȡָ�����У�����Ӯȡ�����ֽ�*/
	//��һ��������I2C���������ź�
	i2c_Start();
	//�ڶ�������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ��
	i2c_SendByte(EEPROM_DEV_ADDR | EEPROM_I2C_WR); //�˴���дָ��
	//���������ȴ�ACK
	if(i2c_WaitAck() != 0)
	{
		goto cmd_fail; //EEPROM������Ӧ��
	}
	//���Ĳ��������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ
	i2c_SendByte((uint8_t)_usAddress);
	//���岽���ȴ�ACK
	if(i2c_WaitAck() != 0)
	{
		goto cmd_fail;	//EEPROM������Ӧ��
	}
	//����������������I2C���ߣ�ǰ��Ĵ����Ŀ����EEPROM���͵�ַ�����濪ʼ��ȡ����
	i2c_Start();
	//���߲�����������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ��
	i2c_SendByte(EEPROM_DEV_ADDR | EEPROM_I2C_RD); //�˴��Ƕ�ָ��
	//�ڰ˲�������ACK
	if(i2c_WaitAck() != 0)
	{
		goto cmd_fail; //EEPROM������Ӧ��
	}
	//�ھŲ���ѭ����ȡ����
	for(i = 0; i < _usSize; i++)
	{
		_pReadBuf[i] = i2c_ReadByte();	//��1���ֽ�
		//ÿ����1���ֽں���Ҫ����Ack�����һ���ֽڲ���ҪAck����Nack
		if(i != _usSize - 1)
		{
			i2c_Ack();	//�м��ֽڶ����CPU����ACK�ź�(���� SDA = 0)
		}
		else
		{
			i2c_NAck();	//���1���ֽڶ����CPU����NACK�ź�(���� SDA=1)
		}
	}
	//����I2C����ֹͣ�ź�
	i2c_Stop();
	return 1; //ִ�гɹ�

cmd_fail:	//����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸
	//����I2C����ֹͣ�ź�
	i2c_Stop();
	return 0;
}

//����EEPROMָ����ַд���������ݣ�����ҳд�������Ч��
uint8_t ee_WriteBytes(uint8_t *_pWriteBuf,uint16_t _usAddress,uint16_t _usSize)
{
	uint16_t i,m;
	uint16_t usAddr;

	/*
	 * д����EEPROM�������������������ȡ�ܶ��ֽڣ�ÿ��д����ֻ����ͬһ��page��
	 * ����24xx02��page size = 8
	 * �򵥵Ĵ�����Ϊ�����ֽ�д����ģʽ��ûд1���ֽڣ������͵�ַ
	 * Ϊ���������д��Ч�ʣ�����������page write������
	 * */
	usAddr = _usAddress;
	for(i = 0; i < _usSize; i++)
	{
		//�����͵�1���ֽڻ���ҳ���ܵ�ַʱ����Ҫ���·��������źź͵�ַ
		if((i == 0) || (usAddr & (EEPROM_PAGE_SIZE -1)) == 0)
		{
			//��0������ֹͣ�źţ������ڲ�д����
			i2c_Stop();

			//ͨ���������Ӧ��ķ�ʽ���ж��ڲ�д�����Ƿ���ɣ�һ��С��10ms
			//CLKƵ��Ϊ200KHzʱ����ѯ����Ϊ30������
			for(m = 0; m < 1000; m++)
			{
				//��1��������I2C���������ź�
				i2c_Start();

				//�ڶ�������������ֽڣ���7λ�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ��
				i2c_SendByte(EEPROM_DEV_ADDR | EEPROM_I2C_WR); //�˴���дָ��

				//������������һ��ʱ�ӣ��ж������Ƿ���ȷӦ��
				if(i2c_WaitAck() == 0)
				{
					break;
				}
			}
			if(m == 1000)
			{
				goto cmd_fail; //EEPROM����д��ʱ
			}
			//���Ĳ��������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ
			i2c_SendByte((uint8_t)usAddr);
			//���岽���ȴ�ACK
			if(i2c_WaitAck() != 0)
			{
				goto cmd_fail;	//EEPROM������Ӧ��
			}
		}
		//����������ʼд������
		i2c_SendByte(_pWriteBuf[i]);
		//���߲�������ACK
		if(i2c_WaitAck() != 0)
		{
			goto cmd_fail;	//EEPROM������Ӧ��
		}
		usAddr++;	//��ַ��1
	}
	//����ִ�гɹ�������I2C����ֹͣ�ź�
	i2c_Stop();
	return 1;

cmd_fail:	//����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸
	//����I2C����ֹͣ�ź�
	i2c_Stop();
	return 0;
}

void ee_Erase(void)
{
	uint16_t i;
	uint8_t buf[EEPROM_SIZE];

	//��仺����
	for(i = 0; i < EEPROM_SIZE; i++)
	{
		buf[i] = 0xFF;
	}

	//дEEPROM����ʼ��ַ = 0�����ݳ���Ϊ256
	if(ee_WriteBytes(buf,0,EEPROM_SIZE) == 0)
	{
		Usart_SendString(DEBUG_USARTx,"����EEPROM����");
		return ;
	}
	else
	{
		Usart_SendString(DEBUG_USARTx,"����EEPROM�ɹ�!");
	}
}

static void ee_Delay(__IO uint32_t nCount) //�򵥵���ʱ����
{
	for(;nCount != 0;nCount--);
}

//EEPROM AT24C02 ��д����
//��������1���쳣����0
uint8_t ee_Test(void)
{
	uint16_t i;
	uint8_t write_buf[EEPROM_SIZE];
	uint8_t read_buf[EEPROM_SIZE];

	if(ee_CheckOk() == 0)
	{
		//û�м�⵽EEPROM
		Usart_SendString(DEBUG_USARTx,"û�м�⵽����EEPROM!");
		return 0;
	}

	//�����Ի�����
	for(i = 0; i < EEPROM_SIZE; i++)
	{
		write_buf[i] = i;
	}

	if(ee_WriteBytes(write_buf,0,EEPROM_SIZE) == 0)
	{
		Usart_SendString(DEBUG_USARTx,"дEEPROM����");
		return 0;
	}
	else
	{
		Usart_SendString(DEBUG_USARTx,"дEEPROM�ɹ���");
	}

	//д��֮����Ҫ�ʵ�����ʱ��ȥ������Ȼ�����
	ee_Delay(0x0FFFFF);

	if(ee_ReadBytes(read_buf,0,EEPROM_SIZE) == 0)
	{
		Usart_SendString(DEBUG_USARTx,"��EEPROM����");
		return 0;
	}
	else
	{
		Usart_SendString(DEBUG_USARTx,"��EEPROM�ɹ����������£�\r\n");
	}

	for(i = 0; i < EEPROM_SIZE; i++)
	{
		if(read_buf[i] != write_buf[i])
		{
			Usart_SendByte(DEBUG_USARTx,read_buf[i]);
			Usart_SendString(DEBUG_USARTx,"����EEPROM������д������ݲ�һ��");
			return 0;
		}
		Usart_SendByte(DEBUG_USARTx,read_buf[i]);
		if((i & 15) == 15)
		{
			Usart_SendByte(DEBUG_USARTx,"\n");
		}
	}
	Usart_SendString(DEBUG_USARTx,"EEPROM��д���Գɹ�\r\n");
	return 1;
}

