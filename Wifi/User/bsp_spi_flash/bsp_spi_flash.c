#include "bsp_spi_flash.h"
#include "stm32f10x.h"

void SPI3_Config( void )
{
	SPI_InitTypeDef SPI_InitStructure;
	//����Ƭѡ������SPI
	SPI3_CS_HIGH;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//�������ؼ��
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//�ߵ�ƽΪ����״̬
	SPI_InitStructure.SPI_CRCPolynomial = 7;//���⸳ֵ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//˫��ȫ˫��
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//��λ������ǰ
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//stm32��Ϊ���豸
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//Ƭѡ�ź����������
	SPI_Init( SPI3, &SPI_InitStructure );
	//ʹ��SPI1����
	SPI_Cmd( SPI3, ENABLE );
}
/*
�ܽ�:FLASH_CS->PA4,FLASH_CLK->PA5,FLASH_DIO->PA7,FLASH_DO->PA6
*/
void SPI1_Config( void )
{
	SPI_InitTypeDef SPI_InitStructure;
	//����Ƭѡ������SPI
	FLASH_CS_HIGH;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//�������ؼ��
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//�ߵ�ƽΪ����״̬
	SPI_InitStructure.SPI_CRCPolynomial = 7;//���⸳ֵ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//˫��ȫ˫��
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//��λ������ǰ
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//stm32��Ϊ���豸
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//Ƭѡ�ź����������
	SPI_Init( SPI1, &SPI_InitStructure );
	//ʹ��SPI1����
	SPI_Cmd( SPI1, ENABLE );
}

u8 SPI_FLASH_SendByte( u8 Byte )
{
	//һֱ�ȴ����������ݻ���Ĵ������ǿյ�
	while(SPI_I2S_GetFlagStatus( SPI1, SPI_I2S_FLAG_TXE ) == RESET);
	SPI_I2S_SendData( SPI1, Byte );
	//�ȴ����մ��豸���ص���Ϣ�������ջ���Ĵ�����Ϊ�գ�������ѭ��
	while(SPI_I2S_GetFlagStatus( SPI1, SPI_I2S_FLAG_RXNE ) == RESET);
	return SPI_I2S_ReceiveData( SPI1 );
}
/*
��������:ʹ��д
дʹ��ͨ����״̬�Ĵ�����WEL(write enable latch)д1
дʹ�ܱ�����ҳ��̡������������������Ƭ������д״̬�Ĵ���֮ǰ
����:
(1)����Ƭѡ
(2)����ָ�����06h
(3)����Ƭѡ
*/
void SPI_FLASH_WriteEnable( void )
{
	FLASH_CS_LOW;
	SPI_FLASH_SendByte( 0x06 );
	FLASH_CS_HIGH;
}
/*
��������:���flash�Ƿ����
����:
(1)����Ƭѡ
(2)����ָ�����05h��ȡ״̬�Ĵ���1(��״̬λ���Ա������ض�ȡ)
	(��ָ��������κ�ʱ���ͣ����Կ��Լ��busyλ�������Ƿ�Ӧ�ö�flashд��Ͷ�ȡ)
(3)��ͣ�ط���dummy������SPIʱ�ӣ�ʹFLASH��SPIʱ�ӵ���ʹ�·���״̬�Ĵ�������Ϣ
(4)����⵽busyλ��Ϊ1ʱ��ֹͣ����dummy
(5)����Ƭѡ
*/
void SPI_FLASH_WaitForWriteEnd( void )
{
	u8 rStatus = 0;
	FLASH_CS_LOW;
	SPI_FLASH_SendByte( 0x05 );
	do
	{
		rStatus = SPI_FLASH_SendByte( Dummy );
	}while(rStatus&0x01);
	FLASH_CS_HIGH;
}
/*
��������:��ȡFLASHоƬ��ID
����:
����Ƭѡ��ƽ��ѡ��FLASHоƬ
����ABH
���������������ݵ��ֽ�
����һ���������ݵ��ֽڲ�����(��ʱ���κ�����FLASH��ֻ����DEVICE ID)
����Ƭѡ����ȡ����
*/
u32 SPI_FLASH_ReadDeviceID( void )
{
	u32 DeviceID = 0;
	FLASH_CS_LOW;
	//����0xABָ�����
	SPI_FLASH_SendByte( 0xAB );
	//�����������������ֽڣ��Ҷ���Ϊ0xff
	SPI_FLASH_SendByte( Dummy );
	SPI_FLASH_SendByte( Dummy );
	SPI_FLASH_SendByte( Dummy );
	//��η��������ֽ���Ϊ�˽���FLASH���ص�ID
	DeviceID = SPI_FLASH_SendByte( Dummy );
	FLASH_CS_HIGH;
	return DeviceID;
}
/*
��������:��ȡFLASHоƬ�������ҵ�ID
����:
����Ƭѡ
����0x9f
����dummy����ȡ����ID
����dummy����ȡ�洢����
����dummy����ȡ����
�������յ������ֽ������һ��
����Ƭѡ
����
*/
u32 SPI_FLASH_ReadID( void )
{
	u32 ManufactureID = 0,temp0,temp1,temp2;
	FLASH_CS_LOW;
	SPI_FLASH_SendByte(0x9f);
	temp0 = SPI_FLASH_SendByte( Dummy );
	temp1 = SPI_FLASH_SendByte( Dummy );
	temp2 = SPI_FLASH_SendByte( Dummy );
	FLASH_CS_HIGH;
	ManufactureID = (temp0<<16) | (temp1<<8) | temp2;
	return ManufactureID;
}
/*
��������:��������
д��֮ǰһ��Ҫ�ȶ��������в���
block diagram��оƬ�ֲ�ĵ�10ҳ
8M��FLASH��127��64KB�Ŀ���ɣ�ÿ����16��4KB���������
����:
(1)дʹ��
(2)���flash�Ƿ����
(3)����Ƭѡ
(4)���ͱ���ָ��20h����ʾҪ��������
(5)����24λ��ַ
(6)����Ƭѡ
(7)����ڲ�ʱ��д�Ƿ����(�����flash�Ƿ����)
*/
void SPI_FLASH_SectorErase( u32 SectorAddr )
{
	SPI_FLASH_WriteEnable();
	SPI_FLASH_WaitForWriteEnd();
	FLASH_CS_LOW;
	SPI_FLASH_SendByte( 0x20 );
	//��λ����,0x00 ff ff ff
	SPI_FLASH_SendByte( (SectorAddr&0xff0000)>>16 );
	SPI_FLASH_SendByte( (SectorAddr&0xff00)>>8 );
	SPI_FLASH_SendByte( SectorAddr&0xff );
	FLASH_CS_HIGH;
	SPI_FLASH_WaitForWriteEnd();
}
/*
��������:��ȡ����
*/
void SPI_FLASH_BufferRead( u8 *pBuffer, u32 ReadAddr, u16 NumByteToRead )
{
	FLASH_CS_LOW;
	SPI_FLASH_SendByte( 0x03 );
	SPI_FLASH_SendByte( (ReadAddr&0xff0000)>>16 );
	SPI_FLASH_SendByte( (ReadAddr&0xff00)>>8 );
	SPI_FLASH_SendByte( ReadAddr&0xff );
	while( NumByteToRead-- )
	{
		*pBuffer = SPI_FLASH_SendByte( Dummy );
		pBuffer++;
	}
	FLASH_CS_HIGH;
}
/*
��������:д�벻����һҳ(256�ֽ�)������
����:
(1)дʹ��
(2)����Ƭѡ
(3)����ָ�����02h
(4)����3���ֽڵĵ�ַ
(5)Ҫ������ֽ���������һҳ�����������ֽ���Ϊһҳ���ֽ�����256�ֽ�
(6)ѭ��д������
(7)����Ƭѡ
(8)����Ƿ�д���
*/
void SPI_FLASH_PageWrite( u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite )
{
	SPI_FLASH_WriteEnable();
	FLASH_CS_LOW;
	SPI_FLASH_SendByte( 0x02 );
	//��λ����,0x00 ff ff ff
	SPI_FLASH_SendByte( (WriteAddr&0xff0000)>>16 );
	SPI_FLASH_SendByte( (WriteAddr&0xff00)>>8 );
	SPI_FLASH_SendByte( WriteAddr&0xff );
	if( NumByteToWrite > PAGE_SIZE);
		NumByteToWrite = PAGE_SIZE;
	while( NumByteToWrite-- )
	{
		SPI_FLASH_SendByte( *pBuffer );
		pBuffer++;
	}
	FLASH_CS_HIGH;
	SPI_FLASH_WaitForWriteEnd();
}
/*
��������:����page_writeд������
����:
(1)����ҳ��NumOfPage��������ʱ������ֽ���NumOfSingle��
д���ַ��ҳ�ı��ز�����ʱ����Ĳ���Addr��������ʱ��ҳ����д����ֽ���count
(2)����Addr == 0����д���ַ��ҳ����
	a.ҳ������1
		ֱ�Ӱ�Ҫ�����ֽ�����д��
	b.ҳ������1����Ҫд����ֽ�������һҳ
		д������ҳ
		���µ����µ�һҳд��
(3)����д���ַ��ҳ������
	a.ҳ������1
		�������ֽ���>��һҳ����д����ֽ���
			д��count���ֽڣ�������һҳ��������һҳд��ʣ���ֽ�
		����Ҫ������ֽ��������һҳʣ��ռ�
			��Ҫ�����ֽ�����д������
	b.ҳ�����ڻ����1
		�Ȱѵ�һҳʣ��ռ�����
		��д������ҳ
		�������µ��ֽ����µ�һҳд��
*/
void SPI_FLASH_BufferWrite( u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite )
{
	u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	NumOfPage = NumByteToWrite / PAGE_SIZE;
	NumOfSingle = NumByteToWrite % PAGE_SIZE;
	Addr = WriteAddr % PAGE_SIZE;
	count = PAGE_SIZE - Addr;
	//������
	if( Addr == 0 )
	{
		//��ҳ������һҳ
		if( NumOfPage == 0 )
		{
			SPI_FLASH_PageWrite( pBuffer, WriteAddr, NumByteToWrite );
		}
		else
		{
			//�Ȱ���һҳ�Ķ�д��
			while( NumOfPage-- )
			{
				SPI_FLASH_PageWrite( pBuffer, WriteAddr, PAGE_SIZE );
				WriteAddr += PAGE_SIZE;
				pBuffer += PAGE_SIZE;
			}
			SPI_FLASH_PageWrite( pBuffer, WriteAddr, NumOfSingle );
		}
	}
	else
	{
		if( NumOfPage == 0 )
		{
			if( NumOfSingle > count )
			{
				temp = NumOfSingle - count;
				SPI_FLASH_PageWrite( pBuffer, WriteAddr, count );
				WriteAddr += count;
				pBuffer += count;
				SPI_FLASH_PageWrite( pBuffer, WriteAddr, temp );
			}
			else
				SPI_FLASH_PageWrite( pBuffer, WriteAddr, NumByteToWrite );
		}
		else
		{
			NumOfSingle = NumByteToWrite - NumOfPage*PAGE_SIZE - count;
			SPI_FLASH_PageWrite( pBuffer, WriteAddr, count );
			WriteAddr += count;
			pBuffer += count;
			while( NumOfPage-- )
			{
				SPI_FLASH_PageWrite( pBuffer, WriteAddr, PAGE_SIZE);
				WriteAddr += PAGE_SIZE;
				pBuffer += PAGE_SIZE;
			}
			if( NumOfSingle != 0 )
			{
				SPI_FLASH_PageWrite( pBuffer, WriteAddr, NumOfSingle );
			}
		}
	}
}
/**********end of file**********/
