/******************** (C) 1209 Lab **************************
 * 文件名  : LeftUltraWave.c
 * 描述    ：前置超声波测距模块LeftUltraWave_Conf（）函数
             初始化超声模块，LeftUltraWave_Meas（）函数
			 启动测距，并将测得的数据通过串口1打印出来         
 * 实验平台：Mini STM32开发板  STM32F103RBT6
 * 硬件连接：------------------
 *          | PA8  - TRIG      |
 *          | PA1  - ECHO      |
 *           ------------------
 * 库版本  ：ST3.5.0
 *
 * 作者    ：XIE 
*********************************************************************************/

#include "LeftUltraWave.h"
//#include "usart.h"
#include "timer.h"
//#include "delay.h"
#include "bsp_SysTick.h"

#define	TRIG_L_PORT      GPIOA		//TRIG       
#define	ECHO_L_PORT      GPIOA		//ECHO 
#define	TRIG_L_PIN       GPIO_Pin_8   //TRIG       
#define	ECHO_L_PIN       GPIO_Pin_1	//ECHO   

float Left_Distance;      //计算出左边的距离    


/*
 * 函数名：LeftUltraWave_Conf
 * 描述  ：超声波模块的初始化
 * 输入  ：无
 * 输出  ：无	
 */
void LeftUltraWave_Conf(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;	
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	//关闭jtag
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
    
  GPIO_InitStructure.GPIO_Pin = TRIG_L_PIN;					 //PA2接TRIG
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //设为推挽输出模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
  GPIO_Init(TRIG_L_PORT, &GPIO_InitStructure);	                 //初始化外设GPIO 

  GPIO_InitStructure.GPIO_Pin = ECHO_L_PIN;				     //PA1接ECH0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;		 //设为输入
  GPIO_Init(ECHO_L_PORT,&GPIO_InitStructure);						 //初始化GPIOA
	
	 //GPIOA.1	  中断线以及中断初始化配置
 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1);

 	 EXTI_InitStructure.EXTI_Line=EXTI_Line1;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
		
			
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			//使能按键所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;					//子优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}


void EXTI1_IRQHandler(void)
{
	//delay_us(10);		                      //延时10us
  Delay_ms(1);
	if(EXTI_GetITStatus(EXTI_Line1) != RESET)
	{
			TIM_SetCounter(TIM3,0);
			TIM_Cmd(TIM3, ENABLE);                                             //开启时钟
		
			while(GPIO_ReadInputDataBit(ECHO_L_PORT,ECHO_L_PIN));	                 //等待低电平

			TIM_Cmd(TIM3, DISABLE);			                                 //定时器2失能
			Left_Distance=TIM_GetCounter(TIM3)*5*34/100.0;									 //计算距离&&UltrasonicWave_Distance<150
/*		
	if(Left_Distance>0)
	{
		printf("Left_dis:%f cm\n",Left_Distance);
		delay_ms(500);
	}
*/	
	
	
	EXTI_ClearITPendingBit(EXTI_Line1);  //清除EXTI1线路挂起位
  
}

}

/*
 * 函数名：FrontUltraWave_Meas
 * 描述  ：开始测距，发送一个>10us的脉冲，然后测量返回的高电平时间
 * 输入  ：无
 * 输出  ：无	
 */
float LeftUltraWave_Meas(void)
{
  GPIO_SetBits(TRIG_L_PORT,TRIG_L_PIN); 		  //送>10US的高电平TRIG_L_PORT,TRIG_L_PIN这两个在define中有
  Delay_ms(2);//延时2ms		                      //延时20US
  GPIO_ResetBits(TRIG_L_PORT,TRIG_L_PIN);
	return Left_Distance;
	//printf("Left_dis:%f cm\n",Left_Distance);
	//delay_ms(500);
	
}

/******************* (C) 1209 Lab *****END OF FILE************/
