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
DMAʹ��
USART2�������߽���DMA
USART2������DMA
*/
void Scan_Command(void);

int main(void)
{
	/***********��ʼ������***********/
	//��ʼ���δ�ʱ�ӣ�����Ӳ����ʱ
	SysTick_Init();
	//��ʼ��WiFiģ��
	WiFi_Init();
	//��ʼ������ͷģ��
	CameraInit();
	//��ʼ����������ģ��
	Moving_Init();
	//��ʼ���������ģ��
	Infrared_Init();
	
	//������ҳ������
	ESP8266_Connect_Server();
	infrared_scan_flag = 1;
	//stop_flag = 0;
	while(1)
	{
		//�ʵ�����ʱ��ȷ����MCU��ȷ���յ�����
		Delay_ms(50);
		//Ϊ��֤ͼ���ʵʱ�ԣ���whileѭ����һֱ��������ͷ����
		//Camera();
		//ɨ��strEsp8266_Fram_Record.Data_RX_BUF�����ݣ���������������Ӧ�Ĵ���
		Scan_Command();
		//��������ϱ�־��һ����ִ�к����Զ�����
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
