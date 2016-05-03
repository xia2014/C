#ifndef __FrontUltraWave_H
#define	__FrontUltraWave_H

void FrontUltraWave_Conf(void);               //对超声波模块初始化
//void FrontUltraWave_Meas(void);                //开始测距，发送一个>10us的脉冲，然后测量返回的高电平时间
float FrontUltraWave_Meas(void); 
#endif /* __UltrasonicWave_H */

