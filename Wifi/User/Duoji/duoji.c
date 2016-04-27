/**
  ******************************************************************************
  * @file    duoji.c
  * @author  Chunling Xie
  * @version V1.0
  * @date    2015-02-07
  * @brief   tim3输出一路pwm 
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 iSO STM32 开发板 
  * 工作原理:使用STM32的定时器TIM3输出一路PWM，根据PWM的不同占空比来控制舵机的转向。
			 具体实现：配置CCR2_Val即可得到不同占空比的PWM。在main函数中调用Duoji_Init()
			 即可实现对舵机的控制。CCR2_Val = 0.5476*angle+75。
			   CCR2_Val				角度（“+”为车前进方向的右边）				占空比
			     100				  +45°										  10%
				 91					  +30°										  9.1%
				 83					  +15°										  8.3%
				 75					   0°										  7.5%
				 67					  -15°										  6.7%
				 59					  -30°										  5.9%
				 50					  -45°										  5.0%
  * 引脚连接:舵机：红-------->5V电源
				   黑-------->GND
				   白-------->PA7(信号输入)
  *
  ******************************************************************************
  */
  
#include "duoji.h" 

 /**
  * @brief  配置TIM3复用输出PWM时用到的I/O
  * @param  无
  * @retval 无
  */
/*static */void Duoji_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

	/* 设置TIM3CLK 为 72MHZ */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

  /* GPIOA and GPIOB clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 

  /*GPIOA Configuration: TIM3 channel 1 and 2 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

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
/*static */void Duoji_Mode_Config(int angle)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM信号电平跳变值 */
  float CCR2_Val = 0.5476*angle+75;/*u16 angle需在主函数中定义,由所需要转的角度转换为CR2_Val来控制占空比*/ 	
	CCR2_Val=(int) CCR2_Val;	
 	

/* ----------------------------------------------------------------------- 
   
   TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR+1)* 100% = 37.5%
   
  ----------------------------------------------------------------------- */

  /* Time base configuration */		 
  TIM_TimeBaseStructure.TIM_Period = 999;       //当定时器从0计数到999，即为1000次，为一个定时周期
  TIM_TimeBaseStructure.TIM_Prescaler =1439;	    //设置预分频：预分频，为50Hz
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//设置时钟分频系数：不分频(这里用不到)
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //配置为PWM模式1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;	   //设置跳变值，当计数器计数到这个值时，电平发生跳变
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //当定时器计数值小于CCR2_Val时为高电平
  TIM_OC2Init(TIM3, &TIM_OCInitStructure);	 //使能通道2
  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

  /* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);                   //使能定时器3	
}

/**
  * @brief  TIM3 输出PWM信号初始化，只要调用这个函数
  *         TIM3的四个通道就会有PWM信号输出
  * @param  无
  * @retval 无
  */
/*void Duoji_Init(void)
{
	Duoji_GPIO_Config();
	//Duoji_Mode_Config();	
}*/


/*****************************************************************************
  *                             控制舵机转向的函数 
  *         
  * 
  *
  */
/*void Duoji_Right(void)
{
	Duoji_Mode_Config(-30);   //舵机左转30°,小车右转
}

void Duoji_Left(void)
{
	Duoji_Mode_Config(30);   //舵机右转30°，小车左转
}

void Duoji_Zero(void)
{
	Duoji_Mode_Config(0);   //舵机归中
}*/
/*********************************************END OF FILE**********************/
