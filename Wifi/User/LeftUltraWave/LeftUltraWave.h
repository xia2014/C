#ifndef __LeftUltraWave_H
#define	__LeftUltraWave_H

void LeftUltraWave_Conf(void);               //对超声波模块初始化
float LeftUltraWave_Meas(void);                //开始测距，发送一个>10us的脉冲，然后测量返回的高电平时间

#endif /* __LeftUltraWave_H */

