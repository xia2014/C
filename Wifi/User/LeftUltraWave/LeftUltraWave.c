/******************** (C) 1209 Lab **************************
 * �ļ���  : LeftUltraWave.c
 * ����    ��ǰ�ó��������ģ��FLeftUltraWave_Conf��������
             ��ʼ������ģ�飬LeftUltraWave_Meas��������
			 ������࣬������õ�����ͨ������1��ӡ����         
 * ʵ��ƽ̨��Mini STM32������  STM32F103RBT6
 * Ӳ�����ӣ�------------------
 *          | PA8  - TRIG      |
 *          | PA1  - ECHO      |
 *           ------------------
 * ��汾  ��ST3.5.0
 *
 * ����    ��XIE 
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

float Left_Distance;      //�������ߵľ���    


/*
 * ��������LeftUltraWave_Conf
 * ����  ��������ģ��ĳ�ʼ��
 * ����  ����
 * ���  ����	
 */
void LeftUltraWave_Conf(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;	
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	//�ر�jtag
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
    
  GPIO_InitStructure.GPIO_Pin = TRIG_L_PIN;					 //PA2��TRIG
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //��Ϊ�������ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
  GPIO_Init(TRIG_L_PORT, &GPIO_InitStructure);	                 //��ʼ������GPIO 

  GPIO_InitStructure.GPIO_Pin = ECHO_L_PIN;				     //PA1��ECH0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;		 //��Ϊ����
  GPIO_Init(ECHO_L_PORT,&GPIO_InitStructure);						 //��ʼ��GPIOA
	
	 //GPIOA.1	  �ж����Լ��жϳ�ʼ������
 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1);

 	 EXTI_InitStructure.EXTI_Line=EXTI_Line1;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
		
			
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//��ռ���ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;					//�����ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}


void EXTI1_IRQHandler(void)
{
	//delay_us(10);		                      //��ʱ10us
  Delay_ms(1);
	if(EXTI_GetITStatus(EXTI_Line1) != RESET)
	{
			TIM_SetCounter(TIM3,0);
			TIM_Cmd(TIM3, ENABLE);                                             //����ʱ��
		
			while(GPIO_ReadInputDataBit(ECHO_L_PORT,ECHO_L_PIN));	                 //�ȴ��͵�ƽ

			TIM_Cmd(TIM3, DISABLE);			                                 //��ʱ��2ʧ��
			Left_Distance=TIM_GetCounter(TIM3)*5*34/100.0;									 //�������&&UltrasonicWave_Distance<150
/*		
	if(Left_Distance>0)
	{
		printf("Left_dis:%f cm\n",Left_Distance);
		delay_ms(500);
	}
*/	
	
	
	EXTI_ClearITPendingBit(EXTI_Line1);  //���EXTI1��·����λ
  
}

}

/*
 * ��������FrontUltraWave_Meas
 * ����  ����ʼ��࣬����һ��>10us�����壬Ȼ��������صĸߵ�ƽʱ��
 * ����  ����
 * ���  ����	
 */
float LeftUltraWave_Meas(void)
{
  GPIO_SetBits(TRIG_L_PORT,TRIG_L_PIN); 		  //��>10US�ĸߵ�ƽ�TTRIG_L_PORT,TRIG_L_PIN��������define����
  Delay_ms(2);//��ʱ2ms		                      //��ʱ20US
  GPIO_ResetBits(TRIG_L_PORT,TRIG_L_PIN);
	return Left_Distance;
	//printf("Left_dis:%f cm\n",Left_Distance);
	//delay_ms(500);
	
}

/******************* (C) 1209 Lab *****END OF FILE************/
