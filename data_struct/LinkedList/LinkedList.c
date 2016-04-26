#include <stdio.h>
#include <stdlib.h>
typedef int ElementType;
typedef struct ListNode
{
	ElementType value;
	struct ListNode *next;
}*SqList;

SqList Create_LinkedList(void);
void Traverse_LinkedList(SqList head);
void Insert_LinkedList(SqList head);
SqList Search_LinkedList(SqList head,ElementType name);
SqList Delete_LinkedList(SqList head,ElementType name);

int main()
{
	SqList head,L;
	ElementType value;
	//L = (SqList)malloc(sizeof(Link));
	head = Create_LinkedList();
	Traverse_LinkedList( head );
	puts("Please input the value you want to search");
	scanf("%d",&value);
	getchar();
	L = Search_LinkedList( head, value );
	if( L == NULL )
	{
		puts("Cannot find the value.\n");
	}else{
		puts("Found!\n");
		printf("The value is %d",L->next->value);
	}
	return 0;
}

SqList Create_LinkedList(void)
{
	SqList head,p1,p2;
	ElementType i,n;
	puts("\nPlease input the number of linklist:");
    scanf("%d",&n);
    getchar();
    if( (head = (SqList)malloc(sizeof(SqList))) == NULL )
    {
    	printf("error!\n");
    	exit(0);
    }
    head->value = 0;
    head->next = NULL;
    p1 = head;
    for( i = 0; i < n; i++ )
    {
	    if( (p2 = (SqList)malloc(sizeof(SqList))) == NULL )
	    {
	    	printf("error!\n");
	    	exit(0);
	    }
	    p1->next = p2;
	    puts("Please input the value:");
	    scanf("%d",&(p2->value));
	    getchar();
	    p2->next = NULL;
	    p1 = p2;
    }
    return head;
}

void Traverse_LinkedList(SqList head)
{
	SqList p;
	p = head->next;
	while(p!=NULL)
	{
		printf("%d ",p->value);
		p = p->next;
	}
	printf("\n");
}

void Insert_LinkedList(SqList head)
{
	SqList p;
	if( (p = (SqList)malloc(sizeof(SqList))) == NULL )
	{
		printf("error!\n");
		exit(0);
	}
	puts("Please input the value you want to insert:");
	scanf("%d",&(p->value)); 
	getchar();
	p->next = head->next;//新节点p的next指向第一个节点 
	head->next= p;//头节点的next指向新节点p 
}

SqList Search_LinkedList(SqList head,ElementType name)
{
	SqList p;
	p = head;
	while( p->next != NULL )
	{
		if( (p->next)->value == name )
		{
			//printf("%d",(p->next)->value);
			printf("%d",p->value);
			return p;
		}
		p->next = (p->next)->next;
	}
	
	/*p = head->next;
	while( p != NULL )
	{
		if( p->value == name )
		{
			return p;
		}
		p = p->next;
	}*/
	return NULL;
}

SqList Delete_LinkedList(SqList head,ElementType name)
{
	//SqList p;
	//p = head->next;
	//p = Search_LinkedList(head,name);
	
}
