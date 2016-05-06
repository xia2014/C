/**
  ******************************************************************************
  * @file    wifi_config.c
  * @author  fire
  * @version V1.0
  * @date    2014-xx-xx
  * @brief   WiFiģ��ӿ���������
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� ISO-STM32 ������
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
	*/

#include "wifi_config.h"
#include "bsp_gpio.h"
#include "bsp_usart.h"

struct  STRUCT_USARTx_Fram strEsp8266_Fram_Record = { 0 };

/**
  * @brief  WiFi_Config wifi ��ʼ��
  * @param  ��
  * @retval ��
  */
void WiFi_Init( void )
{
	//GPIO_WIFI_Config();
	GPIO_USART1_Config();
	GPIO_USART2_Config();
	USART1_Config();
	USART2_Config();
	NVIC_USART_Config();
	//DMA_USART1_Config();
	DMA_USART2_Config();
}
/*********************************************************end of file**************************************************/
