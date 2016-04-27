#include "bsp_usart.h"
#include <stdarg.h>
#include "wifi_config.h"
 /**
  * @brief  USART1 GPIO ����,����ģʽ���á�115200 8-N-1
  * @param  ��
  * @retval ��
  */
void USART1_Config(void)
{
	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	USART_Cmd(USART1, ENABLE);
}


/// �ض���c�⺯��printf��USART1
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ�USART1 */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

/// �ض���c�⺯��scanf��USART1
int fgetc(FILE *f)
{
		/* �ȴ�����1�������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}
void NVIC_USART_Config( void )
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* Enable the USART2 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
}
/*
 * ��������USART2_Config
 * ����  ��USART2 GPIO ����,����ģʽ����
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����
 */
void USART2_Config( void )
{
	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate = 115200;               
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure); 

	//USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	/* ʹ�ܴ���2���߿����ж� */
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
	USART_ITConfig(USART2, USART_IT_ORE, ENABLE);
	USART_Cmd(USART2, ENABLE);
}


/*
 * ��������itoa
 * ����  ������������ת�����ַ���
 * ����  ��-radix =10 ��ʾ10���ƣ��������Ϊ0
 *         -value Ҫת����������
 *         -buf ת������ַ���
 *         -radix = 10
 * ���  ����
 * ����  ����
 * ����  ����USART2_printf()����
 */
static char *itoa( int value, char *string, int radix )
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */


/*
 * ��������USART2_printf
 * ����  ����ʽ�������������C���е�printf��������û���õ�C��
 * ����  ��-USARTx ����ͨ��������ֻ�õ��˴���2����USART2
 *		     -Data   Ҫ���͵����ڵ����ݵ�ָ��
 *			   -...    ��������
 * ���  ����
 * ����  ���� 
 * ����  ���ⲿ����
 *         ����Ӧ��USART2_printf( USART2, "\r\n this is a demo \r\n" );
 *            		 USART2_printf( USART2, "\r\n %d \r\n", i );
 *            		 USART2_printf( USART2, "\r\n %s \r\n", j );
 */
void USART2_printf( USART_TypeDef* USARTx, char *Data, ... )
{
	const char *s;
  int d;   
  char buf[16];

  va_list ap;
  va_start(ap, Data);

	while ( *Data != 0)     // �ж��Ƿ񵽴��ַ���������
	{				                          
		if ( *Data == 0x5c )  //'\'
		{									  
			switch ( *++Data )
			{
				case 'r':							          //�س���
					USART_SendData(USARTx, 0x0d);
					Data ++;
					break;

				case 'n':							          //���з�
					USART_SendData(USARTx, 0x0a);	
					Data ++;
					break;
				
				default:
					Data ++;
				    break;
			}			 
		}
		else if ( *Data == '%')
		{									  //
			switch ( *++Data )
			{				
				case 's':										  //�ַ���
					s = va_arg(ap, const char *);
					for ( ; *s; s++) 
					{
						USART_SendData(USARTx,*s);
						while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
					}
					Data++;
					break;

				case 'd':										//ʮ����
					d = va_arg(ap, int);
					itoa(d, buf, 10);
					for (s = buf; *s; s++) 
					{
						USART_SendData(USARTx,*s);
						while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
					}
					Data++;
					break;
				 default:
					Data++;
					break;
			}		 
		} /* end of else if */
		else USART_SendData(USARTx, *Data++);
		while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
	}
}
/*
USART1,TX��DMA,RX�õ�����
USART2,TX��DMA,RX��DMA�Ϳ������ж�
*/
void DMA_USART1_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	/*����DMAʱ��*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	

	/*����DMAԴ���������ݼĴ�����ַ*/
	DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_DR_Base;	   

	/*�ڴ��ַ(Ҫ����ı�����ָ��)*/
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)strEsp8266_Fram_Record.Data_RX_BUF;//(u32)SendBuff;

	/*���򣺴��ڴ浽����*/		
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;	

	/*�����СDMA_BufferSize=SENDBUFF_SIZE*/	
	DMA_InitStructure.DMA_BufferSize = RX_BUF_MAX_LEN;//SENDBUFF_SIZE;

	/*�����ַ����*/	    
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 

	/*�ڴ��ַ����*/
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	

	/*�������ݵ�λ*/	
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;

	/*�ڴ����ݵ�λ 8bit*/
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 

	/*DMAģʽ����ͨģʽ*/
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;	 

	/*���ȼ�����*/	
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;  

	/*��ֹ�ڴ浽�ڴ�Ĵ���	*/
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	/*����DMA1��4ͨ��*/		   
	DMA_Init(DMA1_Channel4, &DMA_InitStructure); 	   
	
	/*ʧ��DMA*/
	DMA_Cmd (DMA1_Channel4,DISABLE);
	
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
}

void DMA_USART2_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	/*����DMAʱ��*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	

	/*����DMAԴ���������ݼĴ�����ַ*/
	DMA_InitStructure.DMA_PeripheralBaseAddr = USART2_DR_Base;

	/*�ڴ��ַ(Ҫ����ı�����ָ��)*/
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)(strEsp8266_Fram_Record.Data_RX_BUF);//(u32)SendBuff;

	/*���򣺴��ڴ浽����*/		
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;	

	/*�����СDMA_BufferSize=SENDBUFF_SIZE*/	
	DMA_InitStructure.DMA_BufferSize = RX_BUF_MAX_LEN;//SENDBUFF_SIZE;

	/*�����ַ����*/	    
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

	/*�ڴ��ַ����*/
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	

	/*�������ݵ�λ*/	
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;

	/*�ڴ����ݵ�λ 8bit*/
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 

	/*DMAģʽ����ͨģʽ*/
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;

	/*���ȼ�����*/	
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;  

	/*��ֹ�ڴ浽�ڴ�Ĵ���	*/
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	/*����DMA1��7ͨ��*/
	DMA_Init(DMA1_Channel7, &DMA_InitStructure);
	
	/*ʧ��DMA*/
	DMA_Cmd (DMA1_Channel7,DISABLE);
	
	/*����USART2��TX��DMA��������*/
	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
	
	//����USART2��RX��DMA
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	/*����DMA1��6ͨ��*/
	DMA_Init(DMA1_Channel6, &DMA_InitStructure);
	/*ʧ��DMA*/
	DMA_Cmd (DMA1_Channel6,ENABLE);
	
	/*����USART2��RX��DMA��������*/
	USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
}
/**********end of file**********/
