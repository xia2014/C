/**
  ******************************************************************************
  * @file    main.c
  * @author  Alex Xia
  * @version V2.0
  * @date    2016-02-28
  * @brief   The car can mov and send photo to server by wifi.
  ******************************************************************************
  */ 
#include "stm32f10x.h"
#include "bsp_gpio.h"
#include "bsp_usart.h"
#include "bsp_SysTick.h"

#include "wifi_config.h"
#include "wifi_function.h"
#include "bsp_ov7725.h"
#include "bsp_timer.h"

extern volatile uint8_t Ov7725_vsync ;
volatile uint8_t screen_flag;
/*
DMAʹ��
USART2�������߽���DMA
USART2������DMA
*/
void Scan_Command(void);

int main(void)
{
	SysTick_Init();
	WiFi_Config();
	CameraInit();
	Moving_Init();
	ESP8266_Connect_Server();
	while(1)
	{
		Scan_Command();
//		Camera();
	}
}

void Scan_Command(void)
{
	switch( strEsp8266_Fram_Record.Data_RX_BUF[0] )
	{
		case 'A':Motor_Control();break;
		case 'B':Duoji_Control();break;
		case 'C':Camera();break;
	}
}

/*********************************************END OF FILE**********************/
