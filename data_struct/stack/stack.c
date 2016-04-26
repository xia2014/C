/*implement a stack in c*/  
#define STACK_INIT_SIZE 0  
#define STACK_INCREMENT 1

#include <stdio.h> 
#include <stdlib.h>
#include <assert.h>
  
typedef int ElemType;  
typedef int Status;  
  
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

int main(){  
    int i;  
    SqStack *s;  
    s = InitStack(s);  
    for(i = 0 ; i < 20 ; i++){  
        int elem = i;//rand()%100;  
    Push(s,elem);  
    }  
    print(s);  
    FreeStack(s);  
    return 0;
}  
