#include <stdio.h>
#define getLength(a) sizeof(a)/sizeof(a[0])
int AdjustArray(int s[], int l, int r) //返回调整后基准数的位置  
{  
    int i = l, j = r;  
    int x = s[l]; //s[l]即s[i]就是第一个坑  
    while (i < j)  
    {  
        // 从右向左找小于x的数来填s[i]  
        while(i < j && s[j] >= x)   
            j--;    
        if(i < j)   
        {  
            s[i] = s[j]; //将s[j]填到s[i]中，s[j]就形成了一个新的坑  
            i++;  
        }  
  
        // 从左向右找大于或等于x的数来填s[j]  
        while(i < j && s[i] < x)  
            i++;    
        if(i < j)   
        {  
            s[j] = s[i]; //将s[i]填到s[j]中，s[i]就形成了一个新的坑  
            j--;  
        }  
    }  
    //退出时，i等于j。将x填到这个坑中。  
    s[i] = x;  
  
    return i;  
}
void quick_sort1(int s[], int l, int r)  
{  
    if (l < r)  
    {  
        int i = AdjustArray(s, l, r);//先成挖坑填数法调整s[]  
        quick_sort1(s, l, i - 1); // 递归调用   
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
*函数名称：ShellInsert 
*参数说明：pDataArray 无序数组； 
*          d          增量大小 
*          iDataNum为无序数据个数 
*说明：    希尔按增量d的插入排序 
*********************************************************/  
void ShellInsert(int* pDataArray, int d, int iDataNum)  
{  
	int i;
    for (i = d; i < iDataNum; i += 1)    //从第2个数据开始插入  
    {  
        int j = i - d;  
        int temp = pDataArray[i];    //记录要插入的数据  
        while (j >= 0 && pDataArray[j] > temp)    //从后向前，找到比其小的数的位置  
        {  
            pDataArray[j+d] = pDataArray[j];    //向后挪动  
            j -= d;  
        }  
  
        if (j != i - d)    //存在比其小的数  
            pDataArray[j+d] = temp;  
    }  
}  
  
/******************************************************** 
*函数名称：ShellSort 
*参数说明：pDataArray 无序数组； 
*          iDataNum为无序数据个数 
*说明：    希尔排序 
*********************************************************/  
void ShellSort(int* pDataArray, int iDataNum)  
{  
    int d = iDataNum / 2;    //初始增量设为数组长度的一半  
    while(d >= 1)  
    {  
        ShellInsert(pDataArray, d, iDataNum);  
        d = d / 2;    //每次增量变为上次的二分之一  
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