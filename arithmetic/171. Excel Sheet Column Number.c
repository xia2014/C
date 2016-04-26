/*
Related to question Excel Sheet Column Title
Given a column title as appear in an Excel sheet, return its corresponding column number.

For example:
    A -> 1
    B -> 2
    C -> 3
    ...
    Z -> 26
    AA -> 27
    AB -> 28 
Credits:
Special thanks to @ts for adding this problem and creating all test cases.

Subscribe to see which companies asked this question
*/

#include<stdio.h>
#include<stdlib.h>
char arr[] = "ABC";
int titleToNumber(char* s);
int main()
{
	int num;
	num = titleToNumber(arr);
	printf("%d",num);
	return 0;
}

int titleToNumber(char* s) {
    int number = 0;
    int i;
    for (i = 0; s[i] != 0; i++) {
        number = number*26 + s[i] - 'A' + 1;
    }
/*
AAB，即117，根据图中的26进制，可推得十进制数为731 
731 = 7 + 1 x 26 + 1 x 26 x 26 
	= 7 + ( 1 + 1 x 26 ) x 26
	= 7 + ( 1 + ( 1 + 0 x 26 ) x 26 ) x26
	number = 0
		   = (0 x 26) + 1 = 1
		   = (1 x 26) + 1 = 27
		   = (27 x 26) + 7 = 731
*/
    return number;
}
