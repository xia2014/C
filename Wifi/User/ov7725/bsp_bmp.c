#include "bsp_bmp.h"
//#include "ff.h"
#include "bsp_ili9341_lcd.h"

#define RGB24TORGB16(R,G,B) ((unsigned short int)((((R)>>3)<<11) | (((G)>>2)<<5)	| ((B)>>3)))

/* �������Ҫ��ӡbmp��ص���ʾ��Ϣ,��printfע�͵�����
 * ��Ҫ��printf()���轫���������ļ���������
 */
#define BMP_DEBUG_PRINTF(FORMAT,...)  //printf(FORMAT,##__VA_ARGS__)

/**********************************************************
 * ��������Screen_shot
 * ����  ����ȡLCDָ��λ��  ָ����ߵ����� ����Ϊ24λ���ɫbmp��ʽͼƬ
 * ����  : 	x								---ˮƽλ�� 
 *					y								---��ֱλ��  
 *					Width						---ˮƽ���   
 *					Height					---��ֱ�߶�  	
 *					filename				---�ļ���
 * ���  ��	0 		---�ɹ�
 *  				-1 		---ʧ��
 *	    		8			---�ļ��Ѵ���
 * ����  ��Screen_shot(0, 0, 320, 240, "/myScreen");-----ȫ����ͼ
 * ע��  ��x��Χ[0,319]  y��Χ[0,239]  Width��Χ[0,320-x]  Height��Χ[0,240-y]
 *					����ļ��Ѵ���,��ֱ�ӷ���	
 **************************************************************/ 
/*
 * bmp�ļ�ͷ��54���ֽڣ�����ǰ14���ֽ����ļ�ͷ��Ϣ����40���ֽ���λͼ��Ϣͷ��Ϣ
 * bmp�ļ�ͷ֮����Ǿ����������Ϣ
 * 0x42 0x4d :bm
 * 54        :ʵ��λͼ���ݵ�ƫ���ֽ���
 * 40        :λͼ��Ϣͷ�ṹ��ĳ���
 * 1         :ƽ����Ϊ1
 * 24        :24bit���ɫ
 */
int Screen_shot(unsigned short int x,\
                unsigned short int y,\
                unsigned short int Width,\
                unsigned short int Height)
{
	int i,j;
	unsigned int read_data;
	/* �����ǽ�ָ�����ڵ����ݶ�����д���ļ�����ȥ */
	for(i=0; i<Height; i++)					
	{
		for(j=0; j<Width; j++)  
		{
			read_data = LCD_GetPoint(y+j, x+i);				
		}
	}/* ������� */
	return 0;
}

/* -------------------------------------end of file--------------------------------- */





