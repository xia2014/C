/**
  ******************************************************************************
  * @file    motor.c
  * @author  Chunling Xie
  * @version V1.0
  * @date    2015-02-04
  * @brief   tim3输出两路pwm bsp
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 iSO STM32 开发板	
  * 工作原理：使用定时器TIM3输出两路PWM控制电机的速度与转向。CCR1_Val>CCR3_Val时电机正转，CCR1_Val<CCR3_Val时电机反转；
              CCR1_Val与CCR3_Val的差值越大电机转得越快。在main函数中调用Motor_PWM_Init()即可实现对电机的控制。
  * 引脚连接:  电机模块				STM32
			   P12接线柱		    电机
			     5V				单片机的5V输出
			     P0				  PA6(TIM3_CH1)
			     P1				  PB0(TIM3_CH3)
			     7.2V			   连接电池
			     GND			共地，要与单片机的地连接在一起
  *
  ***************************************************************************
  */
  
#include "motor.h" 

 /**
  * @brief  配置TIM3复用输出PWM时用到的I/O
  * @param  无
  * @retval 无
  */
  
  /*GPIO初始化*/
/*static */void Motor_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

	/* 设置TIM3CLK 为 72MHZ */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

  /* GPIOA and GPIOB clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE); 

  /*GPIOA Configuration: TIM3 channel 1 and 2 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /*GPIOB Configuration: TIM3 channel 3 and 4 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}


/**
  * @brief  配置TIM3输出的PWM信号的模式，如周期、极性、占空比
  * @param  无
  * @retval 无
  */
/*
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIMx_ARR --> TIMxCNT 重新计数
 *                    TIMx_CCR(电平发生变化)
 * 信号周期=(TIMx_ARR +1 ) * 时钟周期
 * 占空比=TIMx_CCR/(TIMx_ARR +1)
 */
 
 /*时基初始化*/
/*static */void Motor_Mode_Config(u16 CCR1_Val,u16 CCR3_Val)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM信号电平跳变值 */
 	//u16 CCR1_Val = 500;     /*CCR1_Val>CCR3_Val时电机正转，CCR1_Val<CCR3_Val时电机反转*/   
 	//u16 CCR3_Val = 48;

/* ----------------------------------------------------------------------- 
    TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR+1)* 100% = 50%
    TIM3 Channel3 duty cycle = (TIM3_CCR3/ TIM3_ARR+1)* 100% = 4.8%
  ----------------------------------------------------------------------- */

  /* Time base configuration */		 
  TIM_TimeBaseStructure.TIM_Period = 999;       //当定时器从0计数到999，即为1000次，为一个定时周期
  TIM_TimeBaseStructure.TIM_Prescaler = 0;	    //设置预分频：不预分频，即为72MHz
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//设置时钟分频系数：不分频(这里用不到)
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //配置为PWM模式1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;	   //设置跳变值，当计数器计数到这个值时，电平发生跳变
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //当定时器计数值小于CCR1_Val时为高电平
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);	 //使能通道1
  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

  
  /* PWM1 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	//设置通道3的电平跳变值，输出另外一个占空比的PWM
  TIM_OC3Init(TIM3, &TIM_OCInitStructure);	 //使能通道3
  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);


  /* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);                   //使能定时器3	
}

/**
  * @brief  TIM3 输出PWM信号初始化，只要调用这个函数
  *         TIM3的四个通道就会有PWM信号输出
  * @param  无
  * @retval 无
  */
/*void Motor_Init(void)
{
	Motor_GPIO_Config();
	//Motor_Mode_Config();	
}*/


/**
  * @brief  CCR1_Val=500, CCR3_Val=48，CCR1_Val>CCR3_Val时电机正转， 小车向前直走
            CCR1_Val=48, CCR3_Val=500，CCR1_Val<CCR3_Val时电机反转，小车后退
  * @param  无
  * @retval 无
  */
/*void Motor_GoStraight(void)
{
	Motor_Mode_Config(500,48);  //小车直走
}


void Motor_Back(void)
{
	Motor_Mode_Config(48,800);  //小车后退
}

void Motor_Stop(void)
{
	Motor_Mode_Config(48,48);  //小车停止
}*/
/*********************************************END OF FILE**********************/
