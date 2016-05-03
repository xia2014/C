#ifndef __MOTOR_H
#define	__MOTOR_H

#include "stm32f10x.h"

void Motor_Mode_Config(u16 CCR1_Val,u16 CCR3_Val);
void Motor_GPIO_Config(void);
/*void Motor_Init(void);
void Motor_GoStraight(void);
void Motor_Back(void);
void Motor_Stop(void);*/

#define Motor_Init() Motor_GPIO_Config()
#define Motor_GoStraight() Motor_Mode_Config(500,48)
#define Motor_Back() Motor_Mode_Config(48,800)
#define Motor_Stop() Motor_Mode_Config(48,48)

#endif /* __MOTOR_H */
