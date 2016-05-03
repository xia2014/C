#ifndef __DUOJI_H
#define	__DUOJI_H

#include "stm32f10x.h"
void Duoji_GPIO_Config(void);
void Duoji_Mode_Config(int angle);
/*void Duoji_Init(void);
void Duoji_Right(void);
void Duoji_Left(void);
void Duoji_Zero(void);*/

#define Duoji_Init() Duoji_GPIO_Config()
#define Duoji_Zero() Duoji_Mode_Config(0)
#define Duoji_Right() Duoji_Mode_Config(-30)
#define Duoji_Left() Duoji_Mode_Config(30)
#endif /* __DUOJI_H */
