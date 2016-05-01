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
volatile uint8_t infrared_scan_flag;
volatile uint8_t stop_flag;
/*
DMA使用
USART2，空闲线接收DMA
USART2，发送DMA
*/
void Scan_Command(void);

int main(void)
{
	SysTick_Init();
	WiFi_Config();
	CameraInit();
	Moving_Init();
	GPIO_INFRARED_Config();
	ESP8266_Connect_Server();
	infrared_scan_flag = 0;
	stop_flag = 0;
	while(1)
	{
		Delay_ms ( 500 );
		Scan_Command();
		if( infrared_scan_flag == 1 && stop_flag != 1 )
			Infrared_Scan();
	}
}

void Scan_Command(void)
{
	switch( strEsp8266_Fram_Record.Data_RX_BUF[0] )
	{
		case 'A':Motor_Control();break;
		case 'B':Duoji_Control();break;
		case 'C':Camera();break;
		case 'D':Infrared();break;
	}
}

/*********************************************END OF FILE**********************/
