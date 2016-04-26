#include <stdio.h>
#define getLength(a) sizeof(a)/sizeof(a[0])
int AdjustArray(int s[], int l, int r) //���ص������׼����λ��  
{  
    int i = l, j = r;  
    int x = s[l]; //s[l]��s[i]���ǵ�һ����  
    while (i < j)  
    {  
        // ����������С��x��������s[i]  
        while(i < j && s[j] >= x)   
            j--;    
        if(i < j)   
        {  
            s[i] = s[j]; //��s[j]�s[i]�У�s[j]���γ���һ���µĿ�  
            i++;  
        }  
  
        // ���������Ҵ��ڻ����x��������s[j]  
        while(i < j && s[i] < x)  
            i++;    
        if(i < j)   
        {  
            s[j] = s[i]; //��s[i]�s[j]�У�s[i]���γ���һ���µĿ�  
            j--;  
        }  
    }  
    //�˳�ʱ��i����j����x�������С�  
    s[i] = x;  
  
    return i;  
}
void quick_sort1(int s[], int l, int r)  
{  
    if (l < r)  
    {  
        int i = AdjustArray(s, l, r);//�ȳ��ڿ�����������s[]  
        quick_sort1(s, l, i - 1); // �ݹ����   
        quick_sort1(s, i + 1, r);  
    }  
}

void InsertSort(int *pDataArray, int iDataNum)
{
	int i,j;
	int temp;
	for(i = 1; i < iDataNum; ++i)
	{
		j = i-1;
		temp = pDataArray[i];
		while(j>=0&&pDataArray[j]>temp)
		{
			pDataArray[j+1] = pDataArray[j];
			j--;
		}
		if(j!=i-1)
			pDataArray[j+1] = temp;
	}
}

/******************************************************** 
*�������ƣ�ShellInsert 
*����˵����pDataArray �������飻 
*          d          ������С 
*          iDataNumΪ�������ݸ��� 
*˵����    ϣ��������d�Ĳ������� 
*********************************************************/  
void ShellInsert(int* pDataArray, int d, int iDataNum)  
{  
	int i;
    for (i = d; i < iDataNum; i += 1)    //�ӵ�2�����ݿ�ʼ����  
    {  
        int j = i - d;  
        int temp = pDataArray[i];    //��¼Ҫ���������  
        while (j >= 0 && pDataArray[j] > temp)    //�Ӻ���ǰ���ҵ�����С������λ��  
        {  
            pDataArray[j+d] = pDataArray[j];    //���Ų��  
            j -= d;  
        }  
  
        if (j != i - d)    //���ڱ���С����  
            pDataArray[j+d] = temp;  
    }  
}  
  
/******************************************************** 
*�������ƣ�ShellSort 
*����˵����pDataArray �������飻 
*          iDataNumΪ�������ݸ��� 
*˵����    ϣ������ 
*********************************************************/  
void ShellSort(int* pDataArray, int iDataNum)  
{  
    int d = iDataNum / 2;    //��ʼ������Ϊ���鳤�ȵ�һ��  
    while(d >= 1)  
    {  
        ShellInsert(pDataArray, d, iDataNum);  
        d = d / 2;    //ÿ��������Ϊ�ϴεĶ���֮һ  
    }  
}  
int main()
{
	int i;
	int arr[10] = {6,2,12,85,1,8,3,7,21,77};
	int length = getLength(arr);
	printf("Before we sort:");
	for(i=0;i<10;i++)
		printf("%d,",arr[i]);
	//ShellSort(arr,10);
	//InsertSort(arr,10);
	quick_sort1(arr,0,length-1);
	printf("\nAfter we sort:");
	for(i=0;i<10;i++)
		printf("%d,",arr[i]);
	return 0;
}