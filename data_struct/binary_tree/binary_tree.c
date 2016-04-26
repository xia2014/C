#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define STACK_INIT_SIZE 0  
#define STACK_INCREMENT 1
typedef int ElemType;
typedef char TElemType;
typedef ElemType *Triplet;
typedef int Status;
#define OK 1
#define ERROR 0
#define OVERFLOW 0

typedef struct SqStack
{  
    ElemType *base;  
    ElemType *top;  
    int stackSize;  
}SqStack;

//traverse the stack from base to top  
void print(SqStack *s)
{
    assert(s);
    ElemType *elem = s->base;
    if(s->base == s->top)
	{
    	printf("This is an empty stack!\n");  
    }
	printf("Elements are ");
    for(; elem != s->top; elem++)
	{
    	printf("%d ",*elem);  
    }
    printf("\n");
      
}  
  
//init statck   
SqStack* InitStack(SqStack *s){  
    s = (SqStack*)malloc(sizeof(SqStack));  
    if(!s)  
    	exit(0);  
    s->base = (ElemType*)malloc(STACK_INIT_SIZE*sizeof(ElemType));  
    s->top = s->base;  
    s->stackSize = STACK_INIT_SIZE;  
    printf("Init stack successfully!\n");  
    return s;
}   

//free stack  
Status FreeStack(SqStack *s){  
    if(!s)
	{  
    	printf("Stack is null no need to free!\n");  
    	return 0;  
    }else{  
    	free(s->base);  
    	free(s);  
    	return 1;  
    }  
}

//push element into stack  
Status Push(SqStack *s, ElemType e)
{
    assert(s);
    //stack is full, need more space
    if(s->top - s->base >= s->stackSize)
	{
    	s->base = (ElemType*)realloc(s->base, (s->stackSize + STACK_INCREMENT)*sizeof(ElemType));  
        if(!s->base)
		{
        	printf("realloc failed!\n");
        	exit(1);
    	}
    	s->top = s->base + s->stackSize;
    	s->stackSize += STACK_INCREMENT;
    }
    *(s->top) = e;
    ++(s->top);
    return 1;
}  
  
//pop out elem from stack
Status Pop(SqStack *s, ElemType *e){  
    assert(s);  
    if(s->base == s->top)
	{
	    printf("It's a empty stack\n");  
	    return 0;  
    }
    e = s->top;  
    --(s->top);  
    return 1;  
}

typedef struct BiTNode  
{  
    TElemType data;  
    struct tree * lchild;  
    struct tree * rchild;  
    //unsigned int isOut;   //专为后序遍历设置的，0为不需要被输出，1为需要被输出  
}BiTNode,*BiTree;

/*Status CreateBiTree(Tree T)
{
	ElemType ch;
	printf("Please input your data:");
	scanf("%d",&ch);
	printf("\n");
	if(ch == '#')
	{
		T = NULL;
	}else
	{
		if((T = (Tree)malloc(sizeof(TreeNode)))==NULL)
		{
			printf("Malloc error!\n");
			return ERROR;
		}
		T->data = ch;
		CreateBiTree(T->lchild);
		//CreateBiTree(T->rchild);
	}
	return OK;
}*/
void CreateBiTree(BiTree *T)
{
	TElemType ch,temp;
	scanf("%c",&ch);
	fflush(stdin);
	//temp = getchar();
	if(ch == '#')
		*T = NULL;
	else{
		*T = (BiTree)malloc(sizeof(BiTNode));
		if(!*T)
		{
			printf("Malloc error!\n");
			return;
			//exit(OVERFLOW);
		}
		(*T)->data = ch;
		printf("Please input lchild:");
		CreateBiTree(&(*T)->lchild);
		printf("Please input rchild:");
		CreateBiTree(&(*T)->rchild);
	}
}

void PreOrderTraverse(BiTree T)
{
	if(T==NULL)
	{
		return;
	}
	printf("%c",T->data);
	PreOrderTraverse(T->lchild);
	PreOrderTraverse(T->rchild);
}

int main()
{
	BiTree *T;
	printf("Inputing # means no leafs\n");
	printf("Input the first node of the tree:");
	CreateBiTree(T);
	PreOrderTraverse(*T);
	printf("end\n");
	return 0;
} 
