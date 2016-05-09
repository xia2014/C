#include "bsp_timer.h"
#include "bsp_SysTick.h"
#include <string.h>
extern u8 volatile infrared_scan_flag;
extern u8 volatile stop_flag;
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数  ，240000
static void Timer3_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	TIM_TimeBaseStructure.TIM_Period = arr-1; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =(psc-1); //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	//TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	//设置TIM3_CH2的跳变值
	TIM_OCInitStructure.TIM_Pulse = 48;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM3, ENABLE);
	
	TIM_ITConfig( TIM3,TIM_IT_Update | TIM_IT_Trigger, ENABLE );
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器		 
}

static void Timer4_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	TIM_TimeBaseStructure.TIM_Period = arr-1; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =(psc-1); //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	//float CCR1_Val = 0.5476*angle+75;
	//TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	//设置TIM3_CH2的跳变值
	TIM_OCInitStructure.TIM_Pulse = 75;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM4, ENABLE);
	
	TIM_ITConfig( TIM4,TIM_IT_Update | TIM_IT_Trigger, ENABLE );
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;  //从优先级3级
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

void Moving_Init(void)
{
	Timer3_Init(1000,10);
	Timer4_Init(1000,1440);
	GPIO_DUOJI_Config();
	GPIO_MOTOR_Config();
	Duoji_Zero();
}

void Motor_Control(void)
{
	if( strEsp8266_Fram_Record.Data_RX_BUF[0] == 'A' )
	{
		switch( strEsp8266_Fram_Record.Data_RX_BUF[1] )
		{
			case '1':Motor_Move(RUN_SPEED);stop_flag = 0;break;
			case '2':Motor_Back(RUN_SPEED);break;
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
	//若检测到前方无障碍物，则舵机摆正，小车直行
//	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1) == 1 )   //F为1时前面无障碍物
//	{	   
//		MoveOrBack(0);	
//	}
//	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8) == 0 )   //B为0时向后
//	{
//		MoveOrBack(1);
//	}
//	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11) == 0 )   //L为0时左转
//	{
//		Change_Direction(0);
//	}
//	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12) == 0 )   //R为0时右转
//	{
//		Change_Direction(1);
//	}
	//A4->STOP
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1) == 1 )   //F为1时前面无障碍物
	{
		MoveOrBack(0);	
	}
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8) == 0 )   //B为0时向后
	{
		MoveOrBack(1);
	}
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11) == 0 )   //L为0时左转
	{
		if( GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4) == 0 )
		{	
			Duoji_Zero();
			Motor_Stop();
		}
		else{
			Change_Direction(0);
		}
	}
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12) == 0 )   //R为0时右转
	{
		if( GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4) == 0 )
		{	
			Duoji_Zero();
			Motor_Stop();
		}
		else{
			Change_Direction(1);
		}
	}
}

void MoveOrBack( u8 Move_Back )
{
	//Duoji_Zero();
	//Delay_ms(DELAY_TIME);
	if( Move_Back == 0 )
	{
		Duoji_Zero();
		Delay_ms( DELAY_TIME );
		Motor_Move(RUN_SPEED);
		Delay_ms( DELAY_TIME );
	}else{
		Motor_Back(RUN_SPEED);
		Delay_ms( DELAY_TIME );
	}
}

void Change_Direction( u8 Left_Right )
{
	//Motor_Back( START_SPEED );//急刹车
	//Delay_ms( DELAY_TIME );
	Motor_Stop();//停下来
	Duoji_Zero();//舵机摆正
	//Motor_Back( START_SPEED );//高速启动
	Delay_ms( DELAY_TIME );
	Motor_Back( RUN_SPEED );//正常速度后退
	Delay_ms( DELAY_TIME );
	Motor_Stop();//停下来
	if( Left_Right == 0 )
		Duoji_Mode_Config(ANGLE);//重新左转
	else
		Duoji_Mode_Config(-ANGLE);//重新右转
	Delay_ms( DELAY_TIME );
	Motor_Move(START_SPEED);//高速启动
	Delay_ms( DELAY_TIME );
	Motor_Move(RUN_SPEED);//正常速度前进
	Delay_ms( DELAY_TIME2 );
}

void Infrared(void)
{
	if( strEsp8266_Fram_Record.Data_RX_BUF[0] == 'D' )
	{
		switch( strEsp8266_Fram_Record.Data_RX_BUF[1] )
		{
			case '1':infrared_scan_flag = 1;break;
			case '2':infrared_scan_flag = 0;Motor_Stop();Duoji_Zero();break;
		}
	}
}

void Infrared_Scan2(void)
{
	u8 state,state_bit[4];
	READ_INFO(state);
	state_bit[0] = (state&0x08)>>3;
	state_bit[1] = (state&0x04)>>2;
	state_bit[2] = (state&0x02)>>1;
	state_bit[3] = (state&0x01)>>0;
	if( state_bit[0] == 1 )
	{
		//move
	}else{
		if( state_bit[1] == 0 )
		{
			//stop
		}else{
			if( (state_bit[0]==0 && state_bit[1]==1 && state_bit[2]==0 && state_bit[3]==1) ||
				(state_bit[0]==0 && state_bit[1]==1 && state_bit[2]==1 && state_bit[3]==1)	)
			{
				//turn_right
			}else if( state_bit[0]==0 && state_bit[1]==1 && state_bit[2]==1 && state_bit[3]==0 )
			{
				//turn_left
			}else{
				//back
			}
		}
	}
}

/***************************END OF FILE****************************/
