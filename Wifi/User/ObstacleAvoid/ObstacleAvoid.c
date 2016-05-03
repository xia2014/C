/******************** (C) 1209 Lab **************************
 * 文件名  : ObstacleAvoid.c
 * 描述    ：超声波避障程序
			          
 * 实验平台：Mini STM32开发板  STM32F103RBT6 
 * 硬件连接：--------------------------------------------------
 *          |     左               前                右        |
 *          | PA8  - TRIG      PC8  - TRIG       PA11  - TRIG  |
 *          | PA1  - ECHO      PC7  - ECHO       PA0 - ECHO    |
 *           --------------------------------------------------
 * 库版本  ：ST3.5.0
 *
 * 作者    ：XIE 
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

float Forward;          //小车与前面障碍物的距离
float Left;             //小车与左边障碍物的距离
float Right;            //小车与右边障碍物的距离

int Df,Dr,Dl;   //这三个量分别为小车与前面、右边、左边障碍物的距离的模糊量，为0时表示距离远，为1时表示距离近
int Tm=1;   //小车与目标点的位置，若目标点在左则Tm=0，在右则Tm=1。
int Flag;

/*****************************************************************************
                             小车避障函数
 *****************************************************************************/
void ObstacleAvoid(void)
{
	FrontUltraWave_Conf();               //对超声波模块初始化
  LeftUltraWave_Conf();
	RightUltraWave_Conf();
	
	//uart_init(9600);         //串口初始化
	Duoji_Init();
  Motor_Init();
  
	Flag=0;
	if (Flag==0)
		Motor_GoStraight();
	else//遇到障碍，前、左、右都有障碍，停下，后退
	{
		Motor_Stop();//电机停
		Delay_ms(1000);
		Motor_Back();
	}
  //delay_ms(10);	
	
	Forward=FrontUltraWave_Meas();   //调用前超声波测距函数
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
	
	if(Df==0)//远
		{
	    Duoji_Zero();//舵机摆正
			Delay_ms(50);
			//delay_ms(10);
			//Motor_GoStraight();
    }
		
   else//近
	 {
		 //Motor_Stop();
		 Left=LeftUltraWave_Meas();   //调用左超声波测距函数
     //printf("Left_Dis:%f cm\n",Left);
     if((Left >= 0) && (Left <= 50))
	     Dl=1;//Near
	   else

		 
	     Dl=0;//Far 
	   //printf("\n  Dl=%d \n",Dl);
		 if(Dl==0)//如果左边远
		 {
			 Duoji_Left();
			 Delay_ms(10);
			 //Motor_GoStraight();
		 }
		 else//如果左边近
		 {
			 Right=RightUltraWave_Meas();   //调用右超声波测距函数
			 //printf("Right_Dis:%f cm\n",Right);
       if((Right >= 0) && (Right <= 50))
	       Dr=1;//Near
	     else
	       Dr=0;//Far
	     //printf("\n  Dr=%d \n",Dr);
		   if(Dr==0)//如果右边远
		   {
			   Duoji_Right();
			   Delay_ms(10);
				 //Motor_GoStraight();
		   }
			 else//如果右边也近
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
	
	//模糊化处理，当距离小于0.5m时看作是近距离，令变量（Df,Dr,Dl）为0，大于0.5m时为远距离，令变量为1
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

  if(Df==1)		//前面障碍物离小车较远
	{
	//Duoji_Zero();
	//if (Df == 1)
	//Duoji_Zero();
 	 //delay_ms(100);
   //Motor_GoStraight();       //小车直走
		//printf("\n小车前面没有障碍物\n");
   //delay_ms(1000);
	//Duoji_Zero();
	}
	//	else           //前面障碍物距离小车较近时
  if(Df==0)
	{  
		//printf("\n小车前面有障碍物\n");
		//Motor_GoStraight();
		//Motor_Stop();
		if(Dl==1 && Dr==1)   //若左右两边障碍物均较远
		{
			if(Tm==0) 				//且此时目标点在小车左边 
			{
				//Motor_GoStraight();
				//delay_ms(10);
				Duoji_Left();    //则小车左转
			  delay_ms(10);
				//Duoji_Zero(); 
			}
			else
			{
				//Motor_GoStraight();
				//delay_ms(10);
				//Duoji_Zero();
				//delay_ms(1000);
				Duoji_Right();	 //否则右转	
			  delay_ms(5000);
				//printf("\n小车右转\n");
			  //Duoji_Zero();
				//delay_ms(5000);
			}
		}
		if (Dl==0 && Dr==1)    //左边有障碍物，右边无
		{
			//Motor_GoStraight();
			//delay_ms(10);
			Duoji_Right();            //右转 
			delay_ms(5000);
			//printf("\n左边有障碍物\n");
		}
		if (Dl==1 && Dr==0)    //右边有障碍物，左边无 
		{
			//Motor_GoStraight();
			//delay_ms(10);
			Duoji_Left();             //左转 
			delay_ms(5000);
			//printf("\n右边有障碍物\n");
			//Duoji_Zero();
			//delay_ms(10);
		}
		if (Dl==0 && Dr==0)        //左右都有障碍物，由于此时前面也有障碍物，走进了死区 
		{
			//Motor_Back();
			//printf("Right_dis:%f cm\n",Right);
			//Duoji_Zero();
			//delay_ms(100);
			//Motor_Stop();    //小车停止
			//Duoji_Zero();
      //delay_ms(1000); 
      //Motor_GoStraight();			
      //printf("死区\n");
		  //Motor_Back();	   // 小车后退
			delay_ms(100);
      Duoji_Zero();
			delay_ms(100);
      //printf("\n小车后退 \n");			

			Motor_GoStraight();			
			delay_ms(100);
		
      //Motor_Back();	   // 小车后退
			//delay_ms(100);			
		}
		//delay_ms(1000);
		//Duoji_Zero();

		else
		{

			Motor_Back();	   // 小车后退
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

















