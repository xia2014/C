/******************** (C) 1209 Lab **************************
 * �ļ���  : RightUltraWave.c
 * ����    ��ǰ�ó��������ģ��FRightUltraWave_Conf��������
             ��ʼ������ģ�飬RightUltraWave_Meas��������
			 ������࣬������õ�����ͨ������1��ӡ����         
 * ʵ��ƽ̨��Mini STM32������  STM32F103RBT6
 * Ӳ�����ӣ�------------------
 *          | PA11  - TRIG    |
 *          | PA0 - ECHO      |
 *           ------------------
 * ��汾  ��ST3.5.0
 *
 * ����    ��XIE 
*********************************************************************************/

#include "RightUltraWave.h"
//#include "usart.h"
#include "timer.h"
#include "bsp_SysTick.h"


#define	TRIG_R_PORT      GPIOA		//TRIG       
#define	ECHO_R_PORT      GPIOA		//ECHO 
#define	TRIG_R_PIN       GPIO_Pin_11   //TRIG       
#define	ECHO_R_PIN       GPIO_Pin_0	//ECHO   

float Right_Distance;      //������ұߵľ���    


/*
 * ��������RightUltraWave_Conf
 * ����  ��������ģ��ĳ�ʼ��
 * ����  ����
 * ���  ����	
 */
void RightUltraWave_Conf(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;	
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	//�ر�jtag
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
    
  GPIO_InitStructure.GPIO_Pin = TRIG_R_PIN;					 //PA9��TRIG
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //��Ϊ�������ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
  GPIO_Init(TRIG_R_PORT, &GPIO_InitStructure);	                 //��ʼ������GPIO 

  GPIO_InitStructure.GPIO_Pin = ECHO_R_PIN;				     //PA10��ECH0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;		 //��Ϊ����
  GPIO_Init(ECHO_R_PORT,&GPIO_InitStructure);						 //��ʼ��GPIOA
	
	 //GPIOA.10	  �ж����Լ��жϳ�ʼ������
 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);

 	 EXTI_InitStructure.EXTI_Line=EXTI_Line0;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
		
			
	  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//��ռ���ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;					//�����ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}


/*void EXTI0_IRQHandler(void)
{
	Delay_ms(1);	//��ʱ1ms	                      //��ʱ10us
     if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
			TIM_SetCounter(TIM4,0);
			TIM_Cmd(TIM4, ENABLE);                                             //����ʱ��
		
			while(GPIO_ReadInputDataBit(ECHO_R_PORT,ECHO_R_PIN));	                 //�ȴ��͵�ƽ

			TIM_Cmd(TIM4, DISABLE);			                                 //��ʱ��2ʧ��
			Right_Distance=TIM_GetCounter(TIM4)*5*34/100.0;									 //�������&&UltrasonicWave_Distance<150	
	EXTI_ClearITPendingBit(EXTI_Line0);  //���EXTI10��·����λ
  
}

}*/

/*
 * ��������RightUltraWave_Meas
 * ����  ����ʼ��࣬����һ��>10us�����壬Ȼ��������صĸߵ�ƽʱ��
 * ����  ����
 * ���  ����	
 */
float RightUltraWave_Meas(void)
{
  GPIO_SetBits(TRIG_R_PORT,TRIG_R_PIN); 		  //��>10US�ĸߵ�ƽ�TTRIG_L_PORT,TRIG_L_PIN��������define����
  Delay_ms(2);	//��ʱ2ms	                     //��ʱ20US
  GPIO_ResetBits(TRIG_R_PORT,TRIG_R_PIN);
	return Right_Distance;
//	printf("Right_dis:%f cm\n",Right_Distance);
  //delay_ms(500);
	
}

/******************* (C) 1209 Lab *****END OF FILE************/
