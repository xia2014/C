#ifndef __BSP_USART_H
#define	__BSP_USART_H

#include "stm32f10x.h"
#include <stdio.h>

#define USART1_DR_Base  0x40013804
#define USART2_DR_Base  0x40004404		// 0x40004400 + 0x04 = 0x40004404

#define DEBUG_PRINTF(FORMAT,...)  printf(FORMAT,##__VA_ARGS__)

void USART1_Config(void);
int fputc(int ch, FILE *f);
int fgetc(FILE *f);

void USART2_Config( void );
extern void USART2_printf(USART_TypeDef* USARTx, char *Data, ...);
void NVIC_USART_Config( void );

void DMA_USART1_Config(void);
void DMA_USART2_Config(void);
#endif /* __USART1_H */
