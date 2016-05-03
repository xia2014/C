#ifndef __RightUltraWave_H
#define	__RightUltraWave_H

void RightUltraWave_Conf(void);               //对超声波模块初始化
float RightUltraWave_Meas(void);                //开始测距，发送一个>10us的脉冲，然后测量返回的高电平时间

#endif /* __RightUltraWave_H */

