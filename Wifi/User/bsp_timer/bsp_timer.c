#include "bsp_timer.h"

//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
static void Timerx_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =(psc-1); //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	TIM_ITConfig( TIM3,TIM_IT_Update | TIM_IT_Trigger, ENABLE );
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器		 
}

/* PWM信号电平跳变值 */
//u16 CCR1_Val = 500;     /*CCR1_Val>CCR3_Val时电机正转，CCR1_Val<CCR3_Val时电机反转*/   
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
	//设置TIM3_CH1的跳变值
	TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	//配置TIM3通道1
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	//配置TIM3通道3
	//设置TIM3_CH3的跳变值
	TIM_OCInitStructure.TIM_Pulse = CCR3_Val;
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_Cmd(TIM3, ENABLE);
}

void Duoji_Mode_Config(int angle)
{
	float CCR2_Val = 0.5476*angle+75;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	//设置TIM3_CH2的跳变值
	TIM_OCInitStructure.TIM_Pulse = (int)CCR2_Val;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_Cmd(TIM3, ENABLE);
}

void Moving_Init(void)
{
	Timerx_Init(1000,1440);
	GPIO_DUOJI_Config();
	GPIO_MOTOR_Config();
}

void Motor_Control(void)
{
	if( strEsp8266_Fram_Record.Data_RX_BUF[0] == 'A' )
	{
		switch( strEsp8266_Fram_Record.Data_RX_BUF[1] )
		{
			case '1':Motor_GoStrait();break;
			case '2':Motor_Fallback();break;
			case '3':Motor_Stop();break;
		}
	}
	strEsp8266_Fram_Record.Data_RX_BUF[0] = '\0';
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
	strEsp8266_Fram_Record.Data_RX_BUF[0] = '\0';
}

/***************************END OF FILE****************************/
