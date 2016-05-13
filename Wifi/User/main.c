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
#include <string.h>
extern volatile uint8_t Ov7725_vsync ;
volatile uint8_t screen_flag = 0;
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
	/***********初始化部分***********/
	//初始化滴答时钟，用于硬件延时
	SysTick_Init();
	//初始化WiFi模块
	WiFi_Init();
	//初始化摄像头模块
	CameraInit();
	//初始化电机、舵机模块
	Moving_Init();
	//初始化红外避障模块
	Infrared_Init();
	
	//连接网页服务器
	ESP8266_Connect_Server();
	infrared_scan_flag = 1;
	//stop_flag = 0;
	while(1)
	{
		//适当的延时，确保让MCU正确接收到命令
		Delay_ms(50);
		//为保证图像的实时性，在while循环里一直调用摄像头函数
		//Camera();
		//扫描strEsp8266_Fram_Record.Data_RX_BUF的内容，根据命令做出相应的处理
		Scan_Command();
		//若红外避障标志置一，则执行红外自动避障
		if( infrared_scan_flag == 1 )
			Infrared_Scan2();
	}
}

void Scan_Command(void)
{
	switch( strEsp8266_Fram_Record.Data_RX_BUF[0] )
	{
		case 'A':Motor_Control();break;
		case 'B':Duoji_Control();break;
		case 'C':/*screen_flag = 1;*/Camera();break;
		case 'D':Infrared();break;
	}
}

/*********************************************END OF FILE**********************/
