/**
  ******************************************************************************
  * @file    wifi_config.c
  * @author  fire
  * @version V1.0
  * @date    2014-xx-xx
  * @brief   WiFi模块接口配置驱动
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 ISO-STM32 开发板
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
	*/

#include "wifi_config.h"
#include "bsp_gpio.h"
#include "bsp_usart.h"

struct  STRUCT_USARTx_Fram strEsp8266_Fram_Record = { 0 };

/**
  * @brief  WiFi_Config wifi 初始化
  * @param  无
  * @retval 无
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
