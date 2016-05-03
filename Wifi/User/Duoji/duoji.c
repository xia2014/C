/**
  ******************************************************************************
  * @file    duoji.c
  * @author  Chunling Xie
  * @version V1.0
  * @date    2015-02-07
  * @brief   tim3���һ·pwm 
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  * ����ԭ��:ʹ��STM32�Ķ�ʱ��TIM3���һ·PWM������PWM�Ĳ�ͬռ�ձ������ƶ����ת��
			 ����ʵ�֣�����CCR2_Val���ɵõ���ͬռ�ձȵ�PWM����main�����е���Duoji_Init()
			 ����ʵ�ֶԶ���Ŀ��ơ�CCR2_Val = 0.5476*angle+75��
			   CCR2_Val				�Ƕȣ���+��Ϊ��ǰ��������ұߣ�				ռ�ձ�
			     100				  +45��										  10%
				 91					  +30��										  9.1%
				 83					  +15��										  8.3%
				 75					   0��										  7.5%
				 67					  -15��										  6.7%
				 59					  -30��										  5.9%
				 50					  -45��										  5.0%
  * ��������:�������-------->5V��Դ
				   ��-------->GND
				   ��-------->PA7(�ź�����)
  *
  ******************************************************************************
  */
  
#include "duoji.h" 

 /**
  * @brief  ����TIM3�������PWMʱ�õ���I/O
  * @param  ��
  * @retval ��
  */
/*static */void Duoji_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

	/* ����TIM3CLK Ϊ 72MHZ */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

  /* GPIOA and GPIOB clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 

  /*GPIOA Configuration: TIM3 channel 1 and 2 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // �����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

}

/**
  * @brief  ����TIM3�����PWM�źŵ�ģʽ�������ڡ����ԡ�ռ�ձ�
  * @param  ��
  * @retval ��
  */
/*
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIMx_ARR --> TIMxCNT ���¼���
 *                    TIMx_CCR(��ƽ�����仯)
 * �ź�����=(TIMx_ARR +1 ) * ʱ������
 * ռ�ձ�=TIMx_CCR/(TIMx_ARR +1)
 */
/*static */void Duoji_Mode_Config(int angle)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM�źŵ�ƽ����ֵ */
  float CCR2_Val = 0.5476*angle+75;/*u16 angle�����������ж���,������Ҫת�ĽǶ�ת��ΪCR2_Val������ռ�ձ�*/ 	
	CCR2_Val=(int) CCR2_Val;	
 	

/* ----------------------------------------------------------------------- 
   
   TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR+1)* 100% = 37.5%
   
  ----------------------------------------------------------------------- */

  /* Time base configuration */		 
  TIM_TimeBaseStructure.TIM_Period = 999;       //����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����
  TIM_TimeBaseStructure.TIM_Prescaler =1439;	    //����Ԥ��Ƶ��Ԥ��Ƶ��Ϊ50Hz
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//����ʱ�ӷ�Ƶϵ��������Ƶ(�����ò���)
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //����ΪPWMģʽ1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;	   //��������ֵ�������������������ֵʱ����ƽ��������
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //����ʱ������ֵС��CCR2_ValʱΪ�ߵ�ƽ
  TIM_OC2Init(TIM3, &TIM_OCInitStructure);	 //ʹ��ͨ��2
  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

  /* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);                   //ʹ�ܶ�ʱ��3	
}

/**
  * @brief  TIM3 ���PWM�źų�ʼ����ֻҪ�����������
  *         TIM3���ĸ�ͨ���ͻ���PWM�ź����
  * @param  ��
  * @retval ��
  */
/*void Duoji_Init(void)
{
	Duoji_GPIO_Config();
	//Duoji_Mode_Config();	
}*/


/*****************************************************************************
  *                             ���ƶ��ת��ĺ��� 
  *         
  * 
  *
  */
/*void Duoji_Right(void)
{
	Duoji_Mode_Config(-30);   //�����ת30��,С����ת
}

void Duoji_Left(void)
{
	Duoji_Mode_Config(30);   //�����ת30�㣬С����ת
}

void Duoji_Zero(void)
{
	Duoji_Mode_Config(0);   //�������
}*/
/*********************************************END OF FILE**********************/
