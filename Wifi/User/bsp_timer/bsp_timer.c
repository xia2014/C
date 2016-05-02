#include "bsp_timer.h"
#include "bsp_SysTick.h"
#include <string.h>
extern u8 volatile infrared_scan_flag;
extern u8 volatile stop_flag;
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��  ��240000
static void Timer3_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	TIM_TimeBaseStructure.TIM_Period = arr-1; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =(psc-1); //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	TIM_ITConfig( TIM3,TIM_IT_Update | TIM_IT_Trigger, ENABLE );
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���		 
}

static void Timer4_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	TIM_TimeBaseStructure.TIM_Period = arr-1; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =(psc-1); //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	TIM_ITConfig( TIM4,TIM_IT_Update | TIM_IT_Trigger, ENABLE );
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���		 
}


/* PWM�źŵ�ƽ����ֵ */
//u16 CCR1_Val = 500;     /*CCR1_Val>CCR3_Valʱ�����ת��CCR1_Val<CCR3_Valʱ�����ת*/   
//u16 CCR3_Val = 48;

/* ----------------------------------------------------------------------- 
TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR+1)* 100% = 50%
TIM3 Channel3 duty cycle = (TIM3_CCR3/ TIM3_ARR+1)* 100% = 4.8%
----------------------------------------------------------------------- */

void Motor_Mode_Config(u16 CCR1_Val,u16 CCR3_Val)
{
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	//����TIM3_CH1������ֵ
	TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	//����TIM3ͨ��1
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	//����TIM3ͨ��3
	//����TIM3_CH3������ֵ
	TIM_OCInitStructure.TIM_Pulse = CCR3_Val;
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_Cmd(TIM3, ENABLE);
}

void Duoji_Mode_Config(int angle)
{
	float CCR1_Val = 0.5476*angle+75;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	//����TIM3_CH2������ֵ
	TIM_OCInitStructure.TIM_Pulse = (int)CCR1_Val;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_Cmd(TIM4, ENABLE);
}

void Moving_Init(void)
{
	Timer3_Init(1000,10);
	Timer4_Init(1000,1440);
	GPIO_DUOJI_Config();
	GPIO_MOTOR_Config();
}

void Motor_Control(void)
{
	if( strEsp8266_Fram_Record.Data_RX_BUF[0] == 'A' )
	{
		switch( strEsp8266_Fram_Record.Data_RX_BUF[1] )
		{
			case '1':Motor_GoStraight();stop_flag = 0;break;
			case '2':Motor_Fallback();break;
			case '3':Motor_Stop();stop_flag = 1;break;
		}
	}
	memset(strEsp8266_Fram_Record.Data_RX_BUF,0,3);
}
void Duoji_Control(void)
{
	if( strEsp8266_Fram_Record.Data_RX_BUF[0] == 'B' )
	{
		switch( strEsp8266_Fram_Record.Data_RX_BUF[1] )
		{
			case '1':Duoji_TurnLeft();break;
			case '2':Duoji_TurnRight();break;
			case '3':Duoji_Zero();break;
		}
	}
	memset(strEsp8266_Fram_Record.Data_RX_BUF,0,3);
}

void Infrared_Scan(void)
{
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1) == 1 )   //FΪ1ʱǰ�����ϰ���
	{	   
		Duoji_Zero();
		Delay_ms( 1000 );
		Motor_GoStraight();
	//	printf("ֱ��\r\n");
	}
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8) == 0 )   //BΪ0ʱ���
	{
		Motor_Stop();
		Delay_ms( 1000 );
		Motor_Fallback();
	//	printf("����\r\n");
	}
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11) == 0 )   //LΪ0ʱ��ת
	{
		Duoji_TurnLeft();
		Delay_ms( 1000 );
	//	printf("��תȻ��ֱ��\r\n");
    Motor_GoStraight();
	}
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12) == 0 )   //RΪ0ʱ��ת
	{	   
		Duoji_TurnRight();
		Motor_Stop();
		Delay_ms( 1000 );
	//	printf("��תȻ��ֱ��\r\n");
		Motor_GoStraight();
	}
}

void Infrared(void)
{
	if( strEsp8266_Fram_Record.Data_RX_BUF[0] == 'D' )
	{
		switch( strEsp8266_Fram_Record.Data_RX_BUF[1] )
		{
			case '1':infrared_scan_flag = 1;break;
			case '2':infrared_scan_flag = 0;break;
		}
	}
}

/***************************END OF FILE****************************/
