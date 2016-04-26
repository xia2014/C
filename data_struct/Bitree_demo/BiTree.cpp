#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>


typedef int  ElemType; //��������

//����������ṹ���뵥�������ƣ�����һ���Һ��ӽ��
typedef struct BiTNode
{
	ElemType  data; //������
	struct BiTNode
	*lChild, *rChlid; //����������
}BiTNode, *BiTree;


//���򴴽�������
int CreateBiTree(BiTree *T)
{
	ElemType ch;
	ElemType temp;
	
	//printf("This is a test");
	scanf("%d", &ch);
	temp = getchar();
	if (ch == -1)
		*T = NULL;
	else
	{
		*T = (BiTree)malloc(sizeof(BiTNode));
		if (!(*T))
			exit(-1);
		
		(*T)->data = ch;
		printf("����%d�����ӽڵ㣺", ch);
		CreateBiTree(&(*T)->lChild);
		printf("����%d�����ӽڵ㣺", ch);
		CreateBiTree(&(*T)->rChlid);
	}
	return 1;
}


//�������������
void TraverseBiTree(BiTree T)
{
	if (T == NULL)
		return ;
	printf("%d ", T->data);
	TraverseBiTree(T->lChild);
	TraverseBiTree(T->rChlid);
}


//�������������
void InOrderBiTree(BiTree T)
{
	if (T == NULL)
	return ;
	InOrderBiTree(T->lChild);
	printf("%d ", T->data);
	InOrderBiTree(T->rChlid);
}


//�������������
void PostOrderBiTree(BiTree T)
{
	if (T == NULL)
		return ;
	PostOrderBiTree(T->lChild);
	PostOrderBiTree(T->rChlid);
	printf("%d ", T->data);
}

//�����������
int TreeDeep(BiTree T)
{
	int deep = 0;
	if(T)
	{
		int leftdeep = TreeDeep(T->lChild);
		int rightdeep = TreeDeep(T->rChlid);
		deep = leftdeep>=rightdeep?leftdeep+1:rightdeep+1;
	}
	return deep;
}


//�������Ҷ�ӽ�����
int Leafcount(BiTree T,int &num)
{  
	if(T)
	{
		if(T->lChild ==NULL &&T->rChlid==NULL)
			num++;
		Leafcount(T->lChild,num);
		Leafcount(T->rChlid,num);
	}
	return num;
}
//������
int main(void)
{
	BiTree T;
	BiTree *p = (BiTree*)malloc(sizeof(BiTree));
	int deepth,num=0 ;
	printf("�������һ������ֵ,-1��ʾû��Ҷ���:\n");
	CreateBiTree(&T);
	
	printf("�������������:\n");
	TraverseBiTree(T);
	
	printf("\n");
	printf("�������������:\n");
	InOrderBiTree(T);
	
	printf("\n");
	printf("�������������:\n");
	PostOrderBiTree(T);
	
	printf("\n");
	deepth=TreeDeep(T);
	printf("�������Ϊ:%d",deepth);
	
	printf("\n");
	Leafcount(T,num);
	printf("����Ҷ�ӽ�����Ϊ:%d",num);
	printf("\n");
	return 0;
}