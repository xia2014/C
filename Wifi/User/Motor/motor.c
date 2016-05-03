/**
  ******************************************************************************
  * @file    motor.c
  * @author  Chunling Xie
  * @version V1.0
  * @date    2015-02-04
  * @brief   tim3�����·pwm bsp
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������	
  * ����ԭ��ʹ�ö�ʱ��TIM3�����·PWM���Ƶ�����ٶ���ת��CCR1_Val>CCR3_Valʱ�����ת��CCR1_Val<CCR3_Valʱ�����ת��
              CCR1_Val��CCR3_Val�Ĳ�ֵԽ����ת��Խ�졣��main�����е���Motor_PWM_Init()����ʵ�ֶԵ���Ŀ��ơ�
  * ��������:  ���ģ��				STM32
			   P12������		    ���
			     5V				��Ƭ����5V���
			     P0				  PA6(TIM3_CH1)
			     P1				  PB0(TIM3_CH3)
			     7.2V			   ���ӵ��
			     GND			���أ�Ҫ�뵥Ƭ���ĵ�������һ��
  *
  ***************************************************************************
  */
  
#include "motor.h" 

 /**
  * @brief  ����TIM3�������PWMʱ�õ���I/O
  * @param  ��
  * @retval ��
  */
  
  /*GPIO��ʼ��*/
/*static */void Motor_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

	/* ����TIM3CLK Ϊ 72MHZ */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

  /* GPIOA and GPIOB clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE); 

  /*GPIOA Configuration: TIM3 channel 1 and 2 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // �����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /*GPIOB Configuration: TIM3 channel 3 and 4 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
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
 
 /*ʱ����ʼ��*/
/*static */void Motor_Mode_Config(u16 CCR1_Val,u16 CCR3_Val)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM�źŵ�ƽ����ֵ */
 	//u16 CCR1_Val = 500;     /*CCR1_Val>CCR3_Valʱ�����ת��CCR1_Val<CCR3_Valʱ�����ת*/   
 	//u16 CCR3_Val = 48;

/* ----------------------------------------------------------------------- 
    TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR+1)* 100% = 50%
    TIM3 Channel3 duty cycle = (TIM3_CCR3/ TIM3_ARR+1)* 100% = 4.8%
  ----------------------------------------------------------------------- */

  /* Time base configuration */		 
  TIM_TimeBaseStructure.TIM_Period = 999;       //����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����
  TIM_TimeBaseStructure.TIM_Prescaler = 0;	    //����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ72MHz
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//����ʱ�ӷ�Ƶϵ��������Ƶ(�����ò���)
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //����ΪPWMģʽ1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;	   //��������ֵ�������������������ֵʱ����ƽ��������
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);	 //ʹ��ͨ��1
  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

  
  /* PWM1 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	//����ͨ��3�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM
  TIM_OC3Init(TIM3, &TIM_OCInitStructure);	 //ʹ��ͨ��3
  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);


  /* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);                   //ʹ�ܶ�ʱ��3	
}

/**
  * @brief  TIM3 ���PWM�źų�ʼ����ֻҪ�����������
  *         TIM3���ĸ�ͨ���ͻ���PWM�ź����
  * @param  ��
  * @retval ��
  */
/*void Motor_Init(void)
{
	Motor_GPIO_Config();
	//Motor_Mode_Config();	
}*/


/**
  * @brief  CCR1_Val=500, CCR3_Val=48��CCR1_Val>CCR3_Valʱ�����ת�� С����ǰֱ��
            CCR1_Val=48, CCR3_Val=500��CCR1_Val<CCR3_Valʱ�����ת��С������
  * @param  ��
  * @retval ��
  */
/*void Motor_GoStraight(void)
{
	Motor_Mode_Config(500,48);  //С��ֱ��
}


void Motor_Back(void)
{
	Motor_Mode_Config(48,800);  //С������
}

void Motor_Stop(void)
{
	Motor_Mode_Config(48,48);  //С��ֹͣ
}*/
/*********************************************END OF FILE**********************/
