/*#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE 5
typedef struct
{
    int r[MAXSIZE+1];//r[0]用作哨兵或临时变量
    int length;
}SqList;

void swap(SqList *L,int i,int j)
{
    int temp = L->r[i];
    L->r[i] = L->r[j];
    L->r[j] = temp;
}

void HeapAdjust(SqList *L,int s,int m)
{
    int temp,j;
    temp = L->r[s];
    for(j=2*s;j<=m;j*=2)
    {
        if( j<m && L->r[j]<L->r[j+1] )
        {
            ++j;
        }
        if( temp >= L->r[j] )
            break;
        L->r[s] = L->r[j];
        s = j;
    }
    L->r[s] = temp;
}

void HeapSort(SqList *L)
{
    int i;
    for( i = L->length/2; i > 0; i-- )
        HeapAdjust(L,i,L->length);

    for( i = L->length; i > 1; i-- )
    {
        swap(L,1,i);
        HeapAdjust(L,1,i-1);
    }
}

int main()
{
    SqList* L;
    int i;
    L = (SqList*)malloc(sizeof(SqList));
    L->r[0] = 5;
    L->r[1] = 2;
    L->r[2] = 3;
    L->r[3] = 1;
    L->r[4] = 4;
    L->r[5] = 0;
    L->length = 5;
    for( i = 0; i < MAXSIZE+1; i++)
        printf("%d ",L->r[i]);
    HeapSort(L);
    printf("\n\n");
    for( i = 0; i < MAXSIZE+1; i++)
        printf("%d ",L->r[i]);
    return 0;
}
*/
#include <stdio.h>
int main()
{
    return 0;
}
