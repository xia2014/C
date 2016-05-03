#ifndef __BSP_SPI_FLASH_H
#define __BSP_SPI_FLASH_H
#include "stm32f10x.h"
#include "bsp_gpio.h"
//通过宏定义表示片选的高低电平
#define FLASH_CS_HIGH GPIOA->BSRR = GPIO_Pin_4
#define FLASH_CS_LOW GPIOA->BRR = GPIO_Pin_4

#define FLASH_WriteAddress   0x00000
#define FLASH_ReadAddress    FLASH_WriteAddress
#define FLASH_SectorToErase  FLASH_WriteAddress

#define countof(a) (sizeof(a) / sizeof(*(a)))
#define BufferSize 100

#define Dummy 0xff
#define PAGE_SIZE 256

#define SPI3_CS_HIGH GPIOB->BSRR = GPIO_Pin_2
#define SPI3_CS_LOW GPIOB->BRR = GPIO_Pin_2

void SPI3_Config( void );
void SPI1_Config( void );
u8 SPI_FLASH_SendByte( u8 Byte );
void SPI_FLASH_WriteEnable( void );
void SPI_FLASH_WaitForWriteEnd( void );
u32 SPI_FLASH_ReadDeviceID( void );
u32 SPI_FLASH_ReadID( void );
void SPI_FLASH_SectorErase( u32 SectorAddr );
void SPI_FLASH_BufferWrite( u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite );
void SPI_FLASH_PageWrite( u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite );
void SPI_FLASH_BufferRead( u8 *pBuffer, u32 ReadAddr, u16 NumByteToRead );

#endif
/**************end of file***************/
