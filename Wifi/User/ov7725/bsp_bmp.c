#include "bsp_bmp.h"
//#include "ff.h"
#include "bsp_ili9341_lcd.h"

#define RGB24TORGB16(R,G,B) ((unsigned short int)((((R)>>3)<<11) | (((G)>>2)<<5)	| ((B)>>3)))

/* 如果不需要打印bmp相关的提示信息,将printf注释掉即可
 * 如要用printf()，需将串口驱动文件包含进来
 */
#define BMP_DEBUG_PRINTF(FORMAT,...)  //printf(FORMAT,##__VA_ARGS__)

/**********************************************************
 * 函数名：Screen_shot
 * 描述  ：截取LCD指定位置  指定宽高的像素 保存为24位真彩色bmp格式图片
 * 输入  : 	x								---水平位置 
 *					y								---竖直位置  
 *					Width						---水平宽度   
 *					Height					---竖直高度  	
 *					filename				---文件名
 * 输出  ：	0 		---成功
 *  				-1 		---失败
 *	    		8			---文件已存在
 * 举例  ：Screen_shot(0, 0, 320, 240, "/myScreen");-----全屏截图
 * 注意  ：x范围[0,319]  y范围[0,239]  Width范围[0,320-x]  Height范围[0,240-y]
 *					如果文件已存在,将直接返回	
 **************************************************************/ 
/*
 * bmp文件头有54个字节，其中前14个字节是文件头信息，后40个字节是位图信息头信息
 * bmp文件头之后就是具体的像素信息
 * 0x42 0x4d :bm
 * 54        :实际位图数据的偏移字节数
 * 40        :位图信息头结构体的长度
 * 1         :平面数为1
 * 24        :24bit真彩色
 */
int Screen_shot(unsigned short int x,\
                unsigned short int y,\
                unsigned short int Width,\
                unsigned short int Height)
{
	int i,j;
	unsigned int read_data;
	/* 下面是将指定窗口的数据读出来写到文件里面去 */
	for(i=0; i<Height; i++)					
	{
		for(j=0; j<Width; j++)  
		{
			read_data = LCD_GetPoint(y+j, x+i);				
		}
	}/* 截屏完毕 */
	return 0;
}

/* -------------------------------------end of file--------------------------------- */





