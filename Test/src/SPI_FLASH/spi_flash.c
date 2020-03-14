/*
 * spi_flash.c
 *
 *  Created on: 2020��3��10��
 *      Author: king
 */

#include "spi_flash.h"

static __IO uint32_t SPITimeout = SPIT_LONG_TIMEOUT;
static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);

//SPI_FLASH_��ʼ��
void SPI_FLASH_Init(void)
{
	SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	//ʹ��SPIʱ��
	FLASH_SPI_APBxClock_FUN(FLASH_SPI_CLK,ENABLE);
	//ʹ��SPI�������ʱ��
	FLASH_SPI_CS_APBxClock_FUN(FLASH_SPI_CS_CLK | FLASH_SPI_CS_CLK |
								FLASH_SPI_MISO_CLK | FLASH_SPI_MISO_CLK,ENABLE);

	//����SPI��CS���ţ���ͨ IO����
	GPIO_InitStructure.GPIO_Pin = FLASH_SPI_CS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(FLASH_SPI_CS_PORT,&GPIO_InitStructure);

	//����SPI��SCK����
	GPIO_InitStructure.GPIO_Pin = FLASH_SPI_SCK_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(FLASH_SPI_SCK_PORT,&GPIO_InitStructure);

	//����SPI��MISO����
	GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MISO_PIN;
	GPIO_Init(FLASH_SPI_MISO_PORT,&GPIO_InitStructure);

	//����SPI��MOSI����
	GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MOSI_PIN;
	GPIO_Init(FLASH_SPI_MOSI_PORT,&GPIO_InitStructure);


	//ֹͣ�ź� flash:CS���Ÿߵ�ƽ
	SPI_FLASH_CS_HIGH();

	//SPIģʽ����
	//FLASHоƬ֧��SPIģʽ0��ģʽ3���ݴ�����CPOL CPHA
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(FLASH_SPIx,&SPI_InitStructure);

	//ʹ��SPI
	SPI_Cmd(FLASH_SPIx,ENABLE);

}

//����FLASH����
void SPI_FLASH_SectorErase(u32 SectorAddr)
{
	//����FLASHдʹ������
	SPI_FLASH_WriteEnable();
	SPI_FLASH_WaiteForWriteEnd();

	//��������
	//ѡ��FLASH��CS�͵�ƽ
	SPI_FLASH_CS_LOW();

	//������������ָ��
	SPI_FALSH_SendByte(W25X_SectorErase);
	//���Ͳ���������ַ�ĸ�λ
	SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
	//���Ͳ���������ַ����λ
	SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
	//���Ͳ���������ַ�ĵ�λ
	SPI_FLASH_SendByte(SectorAddr &0xFF);
	//ֹͣ�źţ�FLASH:CS�ߵ�ƽ
	SPI_FLASH_CS_HIGH();
	//�ȴ��������
	SPI_FLASH_WaiteForWriteEnd();

}

//����FLASH��������Ƭ����
void SPI_FLASH_BulkErase(void)
{
	//����SPIдʹ������
	SPI_FLASH_WriteEnable();

	//����Erase
	//ѡ��FLASH��CS�͵�ƽ
	SPI_FLASH_CS_LOW();
	//�����������ָ��
	SPI_FLASH_SendByte(W25X_ChipErase);
	//ֹͣ�ź�FLASH:CS�ߵ�ƽ
	SPI_FLASH_CS_HIGH();

	//�ȴ��������
	SPI_FLASH_WaiteForWriteEnd();
}

/*
 * ��FLASH��ҳд�����ݣ����ñ�����д������ǰ��Ҫ�Ȳ�������
 * pBuffer��Ҫд�����ݵ�ָ��
 * WriteAddr��Ҫд��ĵ�ַ
 * NumByteToWrite��д�����ݳ��ȣ�����С�ڵ���SPI_FLASH_PerWritePageSize
 *
 * */
void SPI_FLASH_PageWrite(u8 *pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
	//����FLASHдʹ������
	SPI_FLASH_WriteEnable();

	//ѡ��FLASH��CS�͵�ƽ
	SPI_FLASH_CS_LOW();
	//дҳдָ��
	SPI_FLASH_SendByte(W25X_PageProgram);
	//����д��ַ�ĸ�λ
	SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
	//����д��ַ����λ
	SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
	//����д��ַ�ĵ�λ
	SPI_FLASH_SendByte(WriteAddr & 0xFF);

	if(NumByteToWrite > SPI_FLASH_PerWritePageSize)
	{
		NumByteToWrite = SPI_FLASH_PerWritePageSize;
		FLASH_ERROR("SPI_FLASH_PaeWrite too large!");
	}

	//д������
	while(NumByteToWrite--)
	{
		//���͵�ǰҪд����ֽ�����
		SPI_FLASH_SendByte(*pBuffer);
		//ָ�����ֽ�����
		pBuffer++;
	}
	//ֹͣ�ź�FLASH��CS�ߵ�ƽ
	SPI_FLASH_CS_HIGH();

	//�ȴ�д�����
	SPI_FLASH_WaiteForWriteEnd();
}

/**
 * ��FLASHд�����ݣ����ñ�����д������ǰ��Ҫ�Ȳ�������
 * pBuffer:Ҫд�����ݵ�ָ��
 * WriteAddr��д���ַ
 * NumByteToWrite��д�����ݳ���
 *
 * */
void SPI_FLASH_BufferWrite(u8 *pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
	u8 NumOfPage = 0, NumOfSingle = 0,Addr = 0, count = 0, temp = 0;
	//mod�������࣬��writeAddr��SPI_FLASH_PageSize��������������AddrֵΪ0
	Addr = WriteAddr % SPI_FLASH_PageSize;
	//��count������ֵ���պÿ��Զ��뵽ҳ��ַ
	count = SPI_FLASH_PageSize - Addr;
	//�����Ҫд��������ҳ
	NumOfPage = NumByteToWrite / SPI_FLASH_PageSize;
	//mod�������࣬�����ʣ�಻��һҳ���ֽ���
	NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

	//Addr=0,��WriteAddr��ð�ҳ����aligned
	if(Addr == 0)
	{
		//NumByteToWrite < SPI_FLASH_PageSize
		if(NumOfPage == 0)
		{
			SPI_FLASH_PageWrite(pBuffer,WriteAddr,NumByteToWrite);
		}
		else //NumByteToWrite > SPI_FLASH_PageSize
		{
			//�Ȱ�����ҳ��д��
			while(NumOfPage--)
			{
				SPI_FLASH_PageWrite(pBuffer,WriteAddr,SPI_FLASH_PageSize);
				WriteAddr += SPI_FLASH_PageSize;
				pBuffer += SPI_FLASH_PageSize;
			}
			//������Ĳ���һҳ�����ݣ�����д��
			SPI_FLASH_PageWrite(pBuffer,WriteAddr,NumOfSingle);
		}
	} //����ַ��SPI_FLASH_PageSize������
	else
	{
		//NumByteToWrite < SPI_FLASH_PageSize
		if(NumOfPage == 0)
		{
			//��ǰҳʣ���count��λ�ñ�NumOfSingleС��һҳд����
			if(NumOfSingle > count)
			{
				temp = NumOfSingle - count;
				//��д����ǰҳ
				SPI_FLASH_PageWrite(pBuffer,WriteAddr,count);
				WriteAddr += count;
				pBuffer += count;
				//��дʣ�������
				SPI_FLASH_PageWrite(pBuffer,WriteAddr,temp);
			}
			else //��ǰʣ���count��λ����д��NumOfSingle������
			{
				SPI_FLASH_PageWrite(pBuffer,WriteAddr,NumByteToWrite);
			}
		}
		else //NumByteToWrite > SPI_FLASH_PageSize
		{
			//��ַ����������count�ֿ������������������
			NumByteToWrite -= count;
			NumOfPage = NumByteToWrite / SPI_FLASH_PageSize;
			NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

			//��д��count�����ݣ�Ϊ��������һ��Ҫд�ĵ�ַ����
			SPI_FLASH_PageWrite(pBuffer,WriteAddr,count);

			//���������ظ���ַ�����ǵ����
			WriteAddr += count;
			pBuffer += count;
			//������ҳ��д��
			while(NumOfPage--)
			{
				SPI_FLASH_PageWrite(pBuffer,WriteAddr,SPI_FLASH_PageSize);
				WriteAddr += SPI_FLASH_PageSize;
				pBuffer += SPI_FLASH_PageSize;
			}
			//���ж���Ĳ���һҳ�����ݣ�����д��
			if(NumOfSingle != 0)
			{
				SPI_FLASH_PageWrite(pBuffer,WriteAddr,NumOfSingle);
			}
		}
	}
}

/**
 * ��ȡFLASH����
 * pBuffer���洢�������ݵ�ָ��
 * ReadAddr����ȡ��ַ
 * NumByteToRead����ȡ���ݳ���
 * */
void SPI_FLASH_BufferRead(u8 *pBuffer,u32 ReadAddr,u16 NumByteToRead)
{
	//ѡ��FLASH��CS�͵�ƽ
	SPI_FLASH_CS_LOW();

	//���Ͷ�ָ��
	SPI_FLASH_SendByte(W25X_ReadData);

	//���Ͷ���ַ��λ
	SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
	//���Ͷ���ַ��λ
	SPI_FLASH_SendByte((ReadAddr & 0xFF00) >> 8);
	//���Ͷ���ַ��λ
	SPI_FLASH_SendByte(ReadAddr & 0xFF);

	//��ȡ����
	while(NumByteToRead--)
	{
		//��ȡһ���ֽ�
		*pBuffer = SPI_FLASH_SendByte(Dummy_Byte);
		//ָ����һ���ֽڻ�����
		pBuffer++;
	}
	//ֹͣ�ź�FLASH��CS�ߵ�ƽ
	SPI_FLASH_CS_HIGH();
}

/*
 * ��ȡFLASH ID
 * */
u32 SPI_FLASH_ReadID(void)
{
	u32 Temp = 0,Temp0 = 0,Temp1 = 0,Temp2 = 0;
	//��ʼͨѶ��CS�͵�ƽ
	SPI_FLASH_CS_LOW();
	//����JEDECָ���ȡID
	SPI_FLASH_SendByte(W25X_JedecDeviceID);
	//��ȡһ���ֽ�����
	Temp0 = SPI_FLASH_SendByte(Dummy_Byte);
	//��ȡһ���ֽ�����
	Temp1 = SPI_FLASH_SendByte(Dummy_Byte);
	//��ȡһ���ֽ�����
	Temp2 = SPI_FLASH_SendByte(Dummy_Byte);

	//ֹͣͨѶ��CS�ߵ�ƽ
	SPI_FLASH_CS_HIGH();

	//�����������������Ϊ�����ķ���ֵ
	Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

	return Temp;
}

/*
 * ��ȡFLASH Device ID
 * */
u32 SPI_FLASH_ReadDeviceID(void)
{
	u32 Temp = 0;
	//ѡ��FLASH��CS�͵�ƽ
	SPI_FLASH_CS_LOW();

	//���͡�RDID��ָ��
	SPI_FLASH_SendByte(W25X_DeviceID);
	SPI_FLASH_SendByte(Dummy_Byte);
	SPI_FLASH_SendByte(Dummy_Byte);
	SPI_FLASH_SendByte(Dummy_Byte);

	//��FLASH�ж�ȡһ���ֽ�
	Temp = SPI_FLASH_SendByte(Dummy_Byte);

	//ֹͣͨѶ��CS�ߵ�ƽ
	SPI_FLASH_CS_HIGH();

	return Temp;
}

/*
 * SPI_FLASH_StartReadSequence
 * */
void SPI_FLASH_StartReadSequence(u32 ReadAddr)
{
	//ѡ��FLASH��CS�͵�ƽ
	SPI_FLASH_CS_LOW();

	//���Ͷ�ȡ�ڴ�ָ��
	SPI_FLASH_SendByte(W25X_ReadData);
	//���͸ߵ�ַ
	SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
	//�����е�ַ
	SPI_FLASH_SendByte((ReadAddr & 0xFF00 ) >> 8);
	//���͵͵�ַ
	SPI_FLASH_SendByte(ReadAddr & 0xFF);
}

/**
 * ʹ��SPI��ȡһ���ֽڵ�����
 * */
u8 SPI_FLASH_ReadByte(viod)
{
	return (SPI_FLASH_SendByte(Dummy_Byte));
}

/**
 * ʹ��SPI����һ���ֽڵ�����
 * byte��Ҫ���͵�����
 * ���ؽ��յ�������
 * */
u8 SPI_FLASH_SendByte(u8 byte)
{
	SPITimeout = SPIT_FLAG_TIMEOUT;
	//�ȴ����ͻ�����Ϊ�գ�TXE�¼�
	while(SPI_I2S_GetFlagStatus(FLASH_SPIx,SPI_I2S_FLAG_TXE) == RESET)
	{
		if((SPITimeout--) == 0)
		{
			return SPI_TIMEOUT_UserCallback(0);
		}
	}

	//д�����ݼĴ�������Ҫд�������д�뷢�ͻ�����
	SPI_I2S_SendData(FLASH_SPIx,byte);
	SPITimeout = SPIT_FLAG_TIMEOUT;
	//�ȴ����ջ������ǿգ�RXNE�¼�
	while(SPI_I2S_GetFlagStatus(FLASH_SPIx,SPI_I2S_FLAG_RXNE) == RESET)
	{
		if((SPITimeout--) == 0)
		{
			return SPI_TIMEOUT_UserCallback(1);
		}
	}
	//��ȡ���ݼĴ�������ȡ���ջ���������
	return SPI_I2S_ReceiveData(FLASH_SPIx);
}

/*
 * ʹ��SPI���������ֽڵ�����
 * byte:Ҫ���͵�����
 * */
u16 SPI_FLASH_SendHalfWord(u16 HalfWord)
{
	SPITimeout = SPIT_FLAG_TIMEOUT;
	//�ȴ����ͻ�����Ϊ�գ�TXE�¼�
	while(SPI_I2S_GetFlagStatus(FLASH_SPIx,SPI_I2S_FLAG_TXE) == RESET)
	{
		if((SPITimeout--) == 0)
		{
			return SPI_TIMEOUT_UserCallback(2);
		}
	}

	//д�����ݼĴ�������Ҫд�������д�뷢�ͻ�����
	SPI_I2S_SendData(FLASH_SPIx,HalfWord);

	SPITimeout = SPIT_FLAG_TIMEOUT;
	//�ȴ����ջ������ǿգ�RXNE�¼�
	while(SPI_I2S_GetFlagStatus(FLASH_SPIx,SPI_I2S_FLAG_RXNE) == RESET)
	{
		if((SPITimeout--) == 0)
		{
			return SPI_TIMEOUT_UserCallback(3);
		}
	}
	//��ȡ�Ĵ�������ȡ���ջ���������
	return SPI_I2S_ReceiveData(FLASH_SPIx);
}

/*
 * ��FLASH���� дʹ������
 * */
void SPI_FLASH_WriteEnable(void)
{
	//ͨѶ��ʼ��CS�͵�ƽ
	SPI_FLASH_CS_LOW();
	//����дʹ������
	SPI_FLASH_SendByte(W25X_WriteEnable);

	//ͨѶ������CS�ߵ�ƽ
	SPI_FLASH_CS_HIGH();
}

/* WIP(busy)��־��FLASH�ڲ�����д��*/
#define WIP_Flag 0x01

/*
 * �ȴ�WIP(BUSY)��־����0�����ȴ���FLASH�ڲ�����д�����
 * */
void SPI_FLASH_WaitForWriteEnd(viod)
{
	u8 FLASH_Status = 0;
	//ѡ��FLASH ��CS �͵�ƽ
	SPI_FLASH_CS_LOW();

	//���Ͷ�״̬�Ĵ�������
	SPI_FLASH_SendByte(W25X_ReadStatusReg);
	//��FLASHæµ����ȴ�
	do
	{
		//��ȡFLASHоƬ��״̬�Ĵ���
		FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);
	}
	while((FLASH_Status & WIP_Flag) == SET); //����д���־

	//ֹͣ�ź�FLASH ��CS�ߵ�ƽ
	SPI_FLASH_CS_HIGH();
}

//�������ģʽ
void SPI_Flash_PowerDown(void)
{
	//ͨѶ��ʼ��CS�͵�ƽ
	SPI_FLASH_CS_LOW();

	//���͵�������
	SPI_FLASH_SendByte(W25X_PowerDown);

	//ͨѶ���� ��CS�ߵ�ƽ
	SPI_FLASH_CS_HIGH();
}

//����
void SPI_Flash_WAKEUP(void)
{
	//ѡ��FLASH ��CS�͵�ƽ
	SPI_FLASH_CS_LOW();
	//�����ϵ�����
	SPI_FLASH_SendByte(W25X_ReleasePowerDown);
	//ֹͣ�ź�FLASH ��CS�ߵ�ƽ
	SPI_FLASH_CS_HIGH();
}

//�ȴ���ʱ�ص�����
static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
	//�ȴ���ʱ��Ĵ������������Ϣ
	FLASH_ERROR("SPI�ȴ���ʱ��errorCode = %d",errorCode);
	return 0;
}
