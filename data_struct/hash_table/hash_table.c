

/* 针对某个集体（比如所在班级）中的人名设计一个哈希表，使的平均查找的长度不超过2完成相应的建表和查表的程序
题目要求：
       假设人名为姓名的汉语拼音形式，待填入哈希表的人名工有30个，取平均查找长度的上限
    为2。哈希函数用除留余数法构造，用伪随机探测再散列法处理冲突。
   测试数据
       自定义。
*/
#include<stdio.h>
#include<conio.h>
#define HASH_LEN 50                      //哈希表的长度         
#define M 47                            //随机数
#define NAME_NO 30                      //人名的个数        
typedef struct      
{
char *py;    //名字的拼音
int k;       //拼音所对应的整数
}NAME;
NAME NameList[HASH_LEN];    //全局变量NAME             
typedef struct    //哈希表
{
	char *py;   //名字的拼音
	int k;      //拼音所对应的整数
	int si;     //查找长度
}HASH;
HASH HashList[HASH_LEN];        //全局变量HASH                     
void InitNameList() //姓名（结构体数组）初始化          
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
			/* 方法：将字符串的各个字符所对应的ASCII码相加，所得的整数做为哈希表的关键字*/
			s0=*(f+r)+s0;
		NameList[i].k=s0;
	} 
}
void CreateHashList() //建立哈希表   
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
		int adr=(NameList[i].k)%M;  //哈希函数
		int d=adr;
		if(HashList[adr].si==0)     //如果不冲突
		{
		HashList[adr].k=NameList[i].k;
		HashList[adr].py=NameList[i].py;
		HashList[adr].si=1;
		}
		else   //冲突  
		{
			do
			{
				d=(d+NameList[i].k%10+1)%M;   //伪随机探测再散列法处理冲突    
				sum=sum+1;   //查找次数加1    
			}while(HashList[d].k!=0);
		
			HashList[d].k=NameList[i].k;
			HashList[d].py=NameList[i].py;
			HashList[d].si=sum+1;
		}
	}
}
void FindList() //查找    
{  
	char name[20]={0};
	int s0=0,r,sum=1,adr,d;
	printf("\n请输入姓名的拼音:");     
	scanf("%s",name);
	for (r=0;r<20;r++)   //求出姓名的拼音所对应的整数(关键字)
		s0+=name[r];
	adr=s0%M;   //使用哈希函数
	d=adr;
	if(HashList[adr].k==s0)          //分3种情况进行判断
		printf("\n姓名:%s   关键字:%d   查找长度为: 1",HashList[d].py,s0);
	else if (HashList[adr].k==0)
		printf("无此记录!");
	else
	{
		int g=0;
		do
		{
			d=(d+s0%10+1)%M;       //伪随机探测再散列法处理冲突                     
			sum=sum+1;
			if (HashList[d].k==0)
			{
			printf("无此记录! ");  
			g=1;     
			}
			if (HashList[d].k==s0)
			{    
				printf("\n姓名:%s   关键字:%d   查找长度为:%d",HashList[d].py,s0,sum);
				g=1;  
			}
		}while(g==0);   
	}
}
void Display() // 显示哈希表       
{
	int i;
	float average=0;
    printf("\n\n地址\t关键字\t\t搜索长度\tH(key)\t 姓名\n"); //显示的格式
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
	printf("\n\n平均查找长度：ASL(%d)=%f \n\n",NAME_NO,average);
}
void main()
{
	char ch1;
	printf("\n                           哈希表的建立和查找\n");
	printf("               *-------------------------------------------*\n");
	printf("               |              D. 显示哈希表                |\n");
	printf("               |              F. 查找                      |\n");
	printf("               |              Q. 退出                      |\n");
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
		printf("\n请输入正确的选择!");
		}
	}
}