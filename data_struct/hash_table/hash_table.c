

/* ���ĳ�����壨�������ڰ༶���е��������һ����ϣ����ʹ��ƽ�����ҵĳ��Ȳ�����2�����Ӧ�Ľ����Ͳ���ĳ���
��ĿҪ��
       ��������Ϊ�����ĺ���ƴ����ʽ���������ϣ������������30����ȡƽ�����ҳ��ȵ�����
    Ϊ2����ϣ�����ó������������죬��α���̽����ɢ�з�������ͻ��
   ��������
       �Զ��塣
*/
#include<stdio.h>
#include<conio.h>
#define HASH_LEN 50                      //��ϣ���ĳ���         
#define M 47                            //�����
#define NAME_NO 30                      //�����ĸ���        
typedef struct      
{
char *py;    //���ֵ�ƴ��
int k;       //ƴ������Ӧ������
}NAME;
NAME NameList[HASH_LEN];    //ȫ�ֱ���NAME             
typedef struct    //��ϣ��
{
	char *py;   //���ֵ�ƴ��
	int k;      //ƴ������Ӧ������
	int si;     //���ҳ���
}HASH;
HASH HashList[HASH_LEN];        //ȫ�ֱ���HASH                     
void InitNameList() //�������ṹ�����飩��ʼ��          
{
	char *f;
	int r,s0,i;
	NameList[0].py="wanghui";
	NameList[1].py="mayuelong";
	NameList[2].py="chenzhicheng";
	NameList[3].py="sunpeng";
	NameList[4].py="zengqinghui";
	NameList[5].py="liqingbo";
	NameList[6].py="liujunpeng";
	NameList[7].py="jiangquanlei";
	NameList[8].py="xingzhengchuan";
	NameList[9].py="luzhaoqian";
	NameList[10].py="gaowenhu";
	NameList[11].py="zhuhaoyin";
	NameList[12].py="chenlili";
	NameList[13].py="wuyunyun";
	NameList[14].py="huangjuanxia";
	NameList[15].py="wangyan";
	NameList[16].py="zhoutao";
	NameList[17].py="jiangzhenyu";
	NameList[18].py="liuxiaolong";
	NameList[19].py="wangziming";
	NameList[20].py="fengjunbo";
	NameList[21].py="lilei";
	NameList[22].py="wangjia";
	NameList[23].py="zhangjianguo";
	NameList[24].py="zhuqingqing";
	NameList[25].py="huangmin";
	NameList[26].py="haoyuhan";
	NameList[27].py="zhoutao";
	NameList[28].py="zhujiang";
	NameList[29].py="lixiaojun";
	for (i=0;i<NAME_NO;i++)
	{
		s0=0;
		f=NameList[i].py;
		for (r=0;*(f+r)!='\0';r++)
			/* ���������ַ����ĸ����ַ�����Ӧ��ASCII����ӣ����õ�������Ϊ��ϣ���Ĺؼ���*/
			s0=*(f+r)+s0;
		NameList[i].k=s0;
	} 
}
void CreateHashList() //������ϣ��   
{
	int i;
	for (i=0; i<HASH_LEN;i++)
	{
		HashList[i].py="";
		HashList[i].k=0;
		HashList[i].si=0;
	}
	for (i=0;i<HASH_LEN;i++)
	{
		int sum=0;
		int adr=(NameList[i].k)%M;  //��ϣ����
		int d=adr;
		if(HashList[adr].si==0)     //�������ͻ
		{
		HashList[adr].k=NameList[i].k;
		HashList[adr].py=NameList[i].py;
		HashList[adr].si=1;
		}
		else   //��ͻ  
		{
			do
			{
				d=(d+NameList[i].k%10+1)%M;   //α���̽����ɢ�з�������ͻ    
				sum=sum+1;   //���Ҵ�����1    
			}while(HashList[d].k!=0);
		
			HashList[d].k=NameList[i].k;
			HashList[d].py=NameList[i].py;
			HashList[d].si=sum+1;
		}
	}
}
void FindList() //����    
{  
	char name[20]={0};
	int s0=0,r,sum=1,adr,d;
	printf("\n������������ƴ��:");     
	scanf("%s",name);
	for (r=0;r<20;r++)   //���������ƴ������Ӧ������(�ؼ���)
		s0+=name[r];
	adr=s0%M;   //ʹ�ù�ϣ����
	d=adr;
	if(HashList[adr].k==s0)          //��3����������ж�
		printf("\n����:%s   �ؼ���:%d   ���ҳ���Ϊ: 1",HashList[d].py,s0);
	else if (HashList[adr].k==0)
		printf("�޴˼�¼!");
	else
	{
		int g=0;
		do
		{
			d=(d+s0%10+1)%M;       //α���̽����ɢ�з�������ͻ                     
			sum=sum+1;
			if (HashList[d].k==0)
			{
			printf("�޴˼�¼! ");  
			g=1;     
			}
			if (HashList[d].k==s0)
			{    
				printf("\n����:%s   �ؼ���:%d   ���ҳ���Ϊ:%d",HashList[d].py,s0,sum);
				g=1;  
			}
		}while(g==0);   
	}
}
void Display() // ��ʾ��ϣ��       
{
	int i;
	float average=0;
    printf("\n\n��ַ\t�ؼ���\t\t��������\tH(key)\t ����\n"); //��ʾ�ĸ�ʽ
	for(i=0; i<50; i++)
	{
	   printf("%d ",i);
	   printf("\t%d ",HashList[i].k);
	   printf("\t\t%d ",HashList[i].si);
	   printf("\t\t%d ",HashList[i].k%M);
	   printf("\t %s ",HashList[i].py);
	   printf("\n");
	}
	for(i=0;i<HASH_LEN;i++)
		average+=HashList[i].si;
	average/=NAME_NO;
	printf("\n\nƽ�����ҳ��ȣ�ASL(%d)=%f \n\n",NAME_NO,average);
}
void main()
{
	char ch1;
	printf("\n                           ��ϣ���Ľ����Ͳ���\n");
	printf("               *-------------------------------------------*\n");
	printf("               |              D. ��ʾ��ϣ��                |\n");
	printf("               |              F. ����                      |\n");
	printf("               |              Q. �˳�                      |\n");
	printf("               *-------------------------------------------*\n");
	InitNameList();                                
	CreateHashList ();                        
	while(1)
	{
		printf("\n       Option-:");
		fflush(stdin);
		ch1=getchar();
		if(ch1=='D'||ch1=='d')  
			Display();   
		else if(ch1=='F'||ch1=='f')
			FindList();
		else if (ch1=='Q'||ch1=='q')
			return;
		else
		{
		printf("\n��������ȷ��ѡ��!");
		}
	}
}