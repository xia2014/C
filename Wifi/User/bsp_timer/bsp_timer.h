#ifndef __BSP_TIMER_H
#define __BSP_TIMER_H
#include "stm32f10x.h"
#include "bsp_gpio.h"
#include "wifi_config.h"

#define Motor_Mode_Config(var1,var2) 	TIM3->CCR1 = var1;\
										TIM3->CCR3 = var2
#define Duoji_Mode_Config(var) TIM4->CCR1 = 0.5476*var+75

#define Motor_Move(var) Motor_Mode_Config(var,48)
#define Motor_Back(var) Motor_Mode_Config(48,var)
#define Motor_Stop() Motor_Mode_Config(48,48)


#define Duoji_TurnLeft() Duoji_Mode_Config(45)
#define Duoji_TurnRight() Duoji_Mode_Config(-45)
#define Duoji_Zero() Duoji_Mode_Config(0)

#define DELAY_TIME 100
#define DELAY_TIME2 500
#define START_SPEED 250
#define RUN_SPEED 220
#define ANGLE 60

#define READ_INFO(INFO)   	do{\
	                                  INFO=0;\
	                                  INFO = (GPIOC->IDR) & 0x000f;\
                                    }while(0)

void Moving_Init(void);
void Motor_Control(void);
void Duoji_Control(void);
void Infrared_Scan(void);
void Infrared(void);
void MoveOrBack( u8 Move_Back );
void Change_Direction( u8 Left_Right );
void Infrared_Scan2(void);
#endif
