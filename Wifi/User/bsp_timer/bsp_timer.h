#ifndef __BSP_TIMER_H
#define __BSP_TIMER_H
#include "stm32f10x.h"
#include "bsp_gpio.h"
#include "wifi_config.h"

#define Motor_GoStraight() Motor_Mode_Config(200,48)
#define Motor_Fallback() Motor_Mode_Config(48,200)
#define Motor_Stop() Motor_Mode_Config(48,48)

#define Duoji_TurnLeft() Duoji_Mode_Config(45)
#define Duoji_TurnRight() Duoji_Mode_Config(-45)
#define Duoji_Zero() Duoji_Mode_Config(0)

#define DELAY_TIME 500
#define SPEED 200
#define ANGLE 60

void Motor_Mode_Config(u16 CCR1_Val,u16 CCR3_Val);
void Duoji_Mode_Config(int angle);
void Moving_Init(void);
void Motor_Control(void);
void Duoji_Control(void);
void Infrared_Scan(void);
void Infrared(void);
#endif
