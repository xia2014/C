/******************** (C) 1209 Lab **************************
 * �ļ���  : UltrasonicWave.c
 * ����    ��ǰ�ó��������ģ��FFrontUltraWave_Conf��������
             ��ʼ������ģ�飬FrontUltraWave_Meas��������
			 ������࣬������õ�����ͨ������1��ӡ����         
 * ʵ��ƽ̨��Mini STM32������  STM32F103RBT6
 * Ӳ�����ӣ�------------------
 *          | PC8  - TRIG      |
 *          | PC7  - ECHO      |
 *           ------------------
 * ��汾  ��ST3.5.0
 *
 * ����    ��Lee 
*********************************************************************************/

#include "UltrasonicWave.h"
//#include "usart.h"
#include "timer.h"
#include "bsp_SysTick.h"


#define	TRIG_F_PORT      GPIOC		//TRIG       
#define	ECHO_F_PORT      GPIOC		//ECHO 
#define	TRIG_F_PIN       GPIO_Pin_8   //TRIG       
#define	ECHO_F_PIN       GPIO_Pin_7	//ECHO   

float Front_Distance;      //�����ǰ��ľ���    


/*
 * ��������FrontUltraWave_Conf
 * ����  ��������ģ��ĳ�ʼ��
 * ����  ����
 * ���  ����	
 */
void FrontUltraWave_Conf(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;	
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	//�ر�jtag
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = TRIG_F_PIN;					 //PC8��TRIG
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //��Ϊ�������ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
  GPIO_Init(TRIG_F_PORT, &GPIO_InitStructure);	                 //��ʼ������GPIO 

  GPIO_InitStructure.GPIO_Pin = ECHO_F_PIN;				     //PC7��ECH0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;		 //��Ϊ����
  GPIO_Init(ECHO_F_PORT,&GPIO_InitStructure);						 //��ʼ��GPIOA
	
	 //GPIOC.7	  �ж����Լ��жϳ�ʼ������
 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource7);

 	 EXTI_InitStructure.EXTI_Line=EXTI_Line7;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
		
			
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//��ռ���ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;					//�����ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}


void EXTI9_5_IRQHandler(void)
{
	Delay_ms(1);	//��ʱ1ms	                      //��ʱ10us
     if(EXTI_GetITStatus(EXTI_Line7) != RESET)
	{
			TIM_SetCounter(TIM2,0);
			TIM_Cmd(TIM2, ENABLE);                                             //����ʱ��
		
			while(GPIO_ReadInputDataBit(ECHO_F_PORT,ECHO_F_PIN));	                 //�ȴ��͵�ƽ

			TIM_Cmd(TIM2, DISABLE);			                                 //��ʱ��2ʧ��
			Front_Distance=TIM_GetCounter(TIM2)*5*34/100.0;									 //�������&&UltrasonicWave_Distance<150
/*		
	if(Front_Distance>0)
	{
		printf("Front_Dis:%f cm\n",Front_Distance);
		delay_ms(500);
	}
*/		
	
	
	EXTI_ClearITPendingBit(EXTI_Line7);  //���EXTI7��·����λ
  
}

}

/*
 * ��������FrontUltraWave_Meas
 * ����  ����ʼ��࣬����һ��>10us�����壬Ȼ��������صĸߵ�ƽʱ��
 * ����  ����
 * ���  ����	
 */
float FrontUltraWave_Meas(void)
{
  GPIO_SetBits(TRIG_F_PORT,TRIG_F_PIN); 		  //��>10US�ĸߵ�ƽ�TRIG_PORT,TRIG_PIN��������define����
  Delay_ms(2);//��ʱ2ms		                      //��ʱ20US
  GPIO_ResetBits(TRIG_F_PORT,TRIG_F_PIN);
	return Front_Distance;
}

/******************* (C) 1209 Lab *****END OF FILE************/
