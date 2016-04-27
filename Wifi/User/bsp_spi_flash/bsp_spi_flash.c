#include "bsp_spi_flash.h"
#include "stm32f10x.h"

void SPI3_Config( void )
{
	SPI_InitTypeDef SPI_InitStructure;
	//拉高片选，配置SPI
	SPI3_CS_HIGH;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//奇数边沿检测
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//高电平为空闲状态
	SPI_InitStructure.SPI_CRCPolynomial = 7;//随意赋值
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//双线全双工
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//高位数据在前
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//stm32作为主设备
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//片选信号由软件控制
	SPI_Init( SPI3, &SPI_InitStructure );
	//使能SPI1外设
	SPI_Cmd( SPI3, ENABLE );
}
/*
管脚:FLASH_CS->PA4,FLASH_CLK->PA5,FLASH_DIO->PA7,FLASH_DO->PA6
*/
void SPI1_Config( void )
{
	SPI_InitTypeDef SPI_InitStructure;
	//拉高片选，配置SPI
	FLASH_CS_HIGH;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//奇数边沿检测
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//高电平为空闲状态
	SPI_InitStructure.SPI_CRCPolynomial = 7;//随意赋值
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//双线全双工
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//高位数据在前
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//stm32作为主设备
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//片选信号由软件控制
	SPI_Init( SPI1, &SPI_InitStructure );
	//使能SPI1外设
	SPI_Cmd( SPI1, ENABLE );
}

u8 SPI_FLASH_SendByte( u8 Byte )
{
	//一直等待到发送数据缓冲寄存器不是空的
	while(SPI_I2S_GetFlagStatus( SPI1, SPI_I2S_FLAG_TXE ) == RESET);
	SPI_I2S_SendData( SPI1, Byte );
	//等待接收从设备返回的信息，若接收缓冲寄存器不为空，则跳出循环
	while(SPI_I2S_GetFlagStatus( SPI1, SPI_I2S_FLAG_RXNE ) == RESET);
	return SPI_I2S_ReceiveData( SPI1 );
}
/*
函数功能:使能写
写使能通过向状态寄存器的WEL(write enable latch)写1
写使能必须在页编程、扇区擦除、块擦除、片擦除和写状态寄存器之前
流程:
(1)拉低片选
(2)发送指令编码06h
(3)拉高片选
*/
void SPI_FLASH_WriteEnable( void )
{
	FLASH_CS_LOW;
	SPI_FLASH_SendByte( 0x06 );
	FLASH_CS_HIGH;
}
/*
函数功能:检测flash是否空闲
流程:
(1)拉低片选
(2)发送指令编码05h读取状态寄存器1(该状态位可以被持续地读取)
	(该指令可以在任何时候发送，所以可以检查busy位来决定是否应该对flash写入和读取)
(3)不停地发送dummy来驱动SPI时钟，使FLASH在SPI时钟的驱使下返回状态寄存器的信息
(4)当检测到busy位不为1时，停止发送dummy
(5)拉高片选
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
函数功能:读取FLASH芯片的ID
流程:
拉低片选电平，选中FLASH芯片
发送ABH
发送三个任意内容的字节
发送一个任意内容的字节并接收(这时发任何内容FLASH都只返回DEVICE ID)
拉高片选，读取结束
*/
u32 SPI_FLASH_ReadDeviceID( void )
{
	u32 DeviceID = 0;
	FLASH_CS_LOW;
	//发送0xAB指令编码
	SPI_FLASH_SendByte( 0xAB );
	//发送三个任意内容字节，我定义为0xff
	SPI_FLASH_SendByte( Dummy );
	SPI_FLASH_SendByte( Dummy );
	SPI_FLASH_SendByte( Dummy );
	//这次发送任意字节是为了接收FLASH返回的ID
	DeviceID = SPI_FLASH_SendByte( Dummy );
	FLASH_CS_HIGH;
	return DeviceID;
}
/*
函数功能:读取FLASH芯片生产厂家的ID
流程:
拉低片选
发送0x9f
发送dummy，获取厂家ID
发送dummy，获取存储类型
发送dummy，获取容量
把上面收到的三字节组合在一起
拉高片选
返回
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
函数功能:扇区擦除
写入之前一定要先对扇区进行擦除
block diagram在芯片手册的第10页
8M的FLASH由127个64KB的块组成，每个块16个4KB的扇区组成
流程:
(1)写使能
(2)检查flash是否空闲
(3)拉低片选
(4)发送编码指令20h，表示要扇区擦除
(5)发送24位地址
(6)拉高片选
(7)检测内部时序写是否完成(即检查flash是否空闲)
*/
void SPI_FLASH_SectorErase( u32 SectorAddr )
{
	SPI_FLASH_WriteEnable();
	SPI_FLASH_WaitForWriteEnd();
	FLASH_CS_LOW;
	SPI_FLASH_SendByte( 0x20 );
	//高位先行,0x00 ff ff ff
	SPI_FLASH_SendByte( (SectorAddr&0xff0000)>>16 );
	SPI_FLASH_SendByte( (SectorAddr&0xff00)>>8 );
	SPI_FLASH_SendByte( SectorAddr&0xff );
	FLASH_CS_HIGH;
	SPI_FLASH_WaitForWriteEnd();
}
/*
函数功能:读取数据
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
函数功能:写入不大于一页(256字节)的数据
流程:
(1)写使能
(2)拉低片选
(3)发送指令编码02h
(4)发送3个字节的地址
(5)要输入的字节数若大于一页，则另输入字节数为一页的字节数，256字节
(6)循环写入数据
(7)拉高片选
(8)检测是否写完成
*/
void SPI_FLASH_PageWrite( u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite )
{
	SPI_FLASH_WriteEnable();
	FLASH_CS_LOW;
	SPI_FLASH_SendByte( 0x02 );
	//高位先行,0x00 ff ff ff
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
函数功能:调用page_write写入数据
流程:
(1)计算页数NumOfPage、不对齐时多余的字节数NumOfSingle、
写入地址与页的边沿不对齐时多出的部分Addr、不对齐时该页可以写入的字节数count
(2)假如Addr == 0，即写入地址与页对齐
	a.页数不足1
		直接按要输入字节数来写入
	b.页数多于1，即要写入的字节数大于一页
		写够整数页
		余下的在新的一页写入
(3)假如写入地址与页不对齐
	a.页数不足1
		若多余字节数>第一页可以写入的字节数
			写入count个字节，填满第一页，再在另一页写入剩余字节
		否则，要填入的字节数不足第一页剩余空间
			按要填入字节数来写入数据
	b.页数大于或等于1
		先把第一页剩余空间填满
		再写够整数页
		最后把余下的字节在新的一页写入
*/
void SPI_FLASH_BufferWrite( u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite )
{
	u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	NumOfPage = NumByteToWrite / PAGE_SIZE;
	NumOfSingle = NumByteToWrite % PAGE_SIZE;
	Addr = WriteAddr % PAGE_SIZE;
	count = PAGE_SIZE - Addr;
	//若对齐
	if( Addr == 0 )
	{
		//若页数不足一页
		if( NumOfPage == 0 )
		{
			SPI_FLASH_PageWrite( pBuffer, WriteAddr, NumByteToWrite );
		}
		else
		{
			//先把满一页的都写入
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
