/******************** (C) 1209 Lab **************************
 * �ļ���  : ObstacleAvoid.c
 * ����    �����������ϳ���
			          
 * ʵ��ƽ̨��Mini STM32������  STM32F103RBT6 
 * Ӳ�����ӣ�--------------------------------------------------
 *          |     ��               ǰ                ��        |
 *          | PA8  - TRIG      PC8  - TRIG       PA11  - TRIG  |
 *          | PA1  - ECHO      PC7  - ECHO       PA0 - ECHO    |
 *           --------------------------------------------------
 * ��汾  ��ST3.5.0
 *
 * ����    ��XIE 
*********************************************************************************/

//#include "sys.h"
//#include "usart.h"
#include "timer.h"
#include "bsp_SysTick.h"
#include "motor.h"
#include "duoji.h"
#include "UltrasonicWave.h"
#include "LeftUltraWave.h"
#include "RightUltraWave.h"
#include "ObstacleAvoid.h"

float Forward;          //С����ǰ���ϰ���ľ���
float Left;             //С��������ϰ���ľ���
float Right;            //С�����ұ��ϰ���ľ���

int Df,Dr,Dl;   //���������ֱ�ΪС����ǰ�桢�ұߡ�����ϰ���ľ����ģ������Ϊ0ʱ��ʾ����Զ��Ϊ1ʱ��ʾ�����
int Tm=1;   //С����Ŀ����λ�ã���Ŀ���������Tm=0��������Tm=1��
int Flag;

/*****************************************************************************
                             С�����Ϻ���
 *****************************************************************************/
void ObstacleAvoid(void)
{
	FrontUltraWave_Conf();               //�Գ�����ģ���ʼ��
  LeftUltraWave_Conf();
	RightUltraWave_Conf();
	
	//uart_init(9600);         //���ڳ�ʼ��
	Duoji_Init();
  Motor_Init();
  
	Flag=0;
	if (Flag==0)
		Motor_GoStraight();
	else//�����ϰ���ǰ�����Ҷ����ϰ���ͣ�£�����
	{
		Motor_Stop();//���ͣ
		Delay_ms(1000);
		Motor_Back();
	}
  //delay_ms(10);	
	
	Forward=FrontUltraWave_Meas();   //����ǰ��������ຯ��
  //printf("Front_Dis:%f cm\n",Forward);
	if((Forward >= 0) && (Forward <= 100))
	  Df=1;//Near
	else
	{
	  Df=0;//Far
		//Duoji_Zero();
		//delay_ms(1);
	}
	//printf("\n  Df=%d \n",Df);
	
	if(Df==0)//Զ
		{
	    Duoji_Zero();//�������
			Delay_ms(50);
			//delay_ms(10);
			//Motor_GoStraight();
    }
		
   else//��
	 {
		 //Motor_Stop();
		 Left=LeftUltraWave_Meas();   //������������ຯ��
     //printf("Left_Dis:%f cm\n",Left);
     if((Left >= 0) && (Left <= 50))
	     Dl=1;//Near
	   else

		 
	     Dl=0;//Far 
	   //printf("\n  Dl=%d \n",Dl);
		 if(Dl==0)//������Զ
		 {
			 Duoji_Left();
			 Delay_ms(10);
			 //Motor_GoStraight();
		 }
		 else//�����߽�
		 {
			 Right=RightUltraWave_Meas();   //�����ҳ�������ຯ��
			 //printf("Right_Dis:%f cm\n",Right);
       if((Right >= 0) && (Right <= 50))
	       Dr=1;//Near
	     else
	       Dr=0;//Far
	     //printf("\n  Dr=%d \n",Dr);
		   if(Dr==0)//����ұ�Զ
		   {
			   Duoji_Right();
			   Delay_ms(10);
				 //Motor_GoStraight();
		   }
			 else//����ұ�Ҳ��
				 Flag=1;
				 //Motor_Stop();
		 }
	 
	 }

    //Motor_GoStraight();
    //delay_ms(10);	 
	
	
	//Left=LeftUltraWave_Meas();
	//Right=RightUltraWave_Meas();
	
/*	
	
	//printf("Left_Dis:%f cm\n",Left);
	//printf("Right_Dis:%f cm\n",Right);
	
	//ģ��������������С��0.5mʱ�����ǽ����룬�������Df,Dr,Dl��Ϊ0������0.5mʱΪԶ���룬�����Ϊ1
  //while(0 <= Forward <= 50)
	
  if((Left > 0) && (Left <= 50))
	  Dl=0;         //Near
	//while(Left >0.5 && Left <= 4)
	else
	  Dl=1;         //Far
	//printf("\nDl=%d \n",Dl);
  if((Right > 0) && (Right <= 50))
	  Dr=0;          //Near
	//while(Right >0.5 && Right <= 4)
	else
	  Dr=1;          //Far
	//printf("\nDr=%d \n",Dr);

  if(Df==1)		//ǰ���ϰ�����С����Զ
	{
	//Duoji_Zero();
	//if (Df == 1)
	//Duoji_Zero();
 	 //delay_ms(100);
   //Motor_GoStraight();       //С��ֱ��
		//printf("\nС��ǰ��û���ϰ���\n");
   //delay_ms(1000);
	//Duoji_Zero();
	}
	//	else           //ǰ���ϰ������С���Ͻ�ʱ
  if(Df==0)
	{  
		//printf("\nС��ǰ�����ϰ���\n");
		//Motor_GoStraight();
		//Motor_Stop();
		if(Dl==1 && Dr==1)   //�����������ϰ������Զ
		{
			if(Tm==0) 				//�Ҵ�ʱĿ�����С����� 
			{
				//Motor_GoStraight();
				//delay_ms(10);
				Duoji_Left();    //��С����ת
			  delay_ms(10);
				//Duoji_Zero(); 
			}
			else
			{
				//Motor_GoStraight();
				//delay_ms(10);
				//Duoji_Zero();
				//delay_ms(1000);
				Duoji_Right();	 //������ת	
			  delay_ms(5000);
				//printf("\nС����ת\n");
			  //Duoji_Zero();
				//delay_ms(5000);
			}
		}
		if (Dl==0 && Dr==1)    //������ϰ���ұ���
		{
			//Motor_GoStraight();
			//delay_ms(10);
			Duoji_Right();            //��ת 
			delay_ms(5000);
			//printf("\n������ϰ���\n");
		}
		if (Dl==1 && Dr==0)    //�ұ����ϰ������� 
		{
			//Motor_GoStraight();
			//delay_ms(10);
			Duoji_Left();             //��ת 
			delay_ms(5000);
			//printf("\n�ұ����ϰ���\n");
			//Duoji_Zero();
			//delay_ms(10);
		}
		if (Dl==0 && Dr==0)        //���Ҷ����ϰ�����ڴ�ʱǰ��Ҳ���ϰ���߽������� 
		{
			//Motor_Back();
			//printf("Right_dis:%f cm\n",Right);
			//Duoji_Zero();
			//delay_ms(100);
			//Motor_Stop();    //С��ֹͣ
			//Duoji_Zero();
      //delay_ms(1000); 
      //Motor_GoStraight();			
      //printf("����\n");
		  //Motor_Back();	   // С������
			delay_ms(100);
      Duoji_Zero();
			delay_ms(100);
      //printf("\nС������ \n");			

			Motor_GoStraight();			
			delay_ms(100);
		
      //Motor_Back();	   // С������
			//delay_ms(100);			
		}
		//delay_ms(1000);
		//Duoji_Zero();

		else
		{

			Motor_Back();	   // С������
			delay_ms(100);
      Duoji_Zero();
			delay_ms(100);	
		//printf("Right_dis:%f cm\n",Right);	
			Motor_GoStraight();			
			delay_ms(100);
		}
	
	}
	


	if((Forward >= 0.0) && (Forward <= 50.0))
	//if(Forward >= 0 && Forward <= 50)
		//Motor_Stop();
	{
		Motor_GoStraight();
		delay_ms(400);
	}
	
  else
	{
		Motor_Stop();
		delay_ms(400);
	}
*/
/*		
	{
		if(Left > 50 && Right > 50)
			Duoji_Right();
	}
*/	
}

















