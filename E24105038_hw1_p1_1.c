#include<stdio.h>
#define NUM 20
char stack[NUM];
int top = -1;

void push(char x) {
	stack[++top] = x;		//++top//top +1 before the x is inserted in stack array
}
char pop() {
	if (top == -1){
		return -1;
	}
	else{
		return stack[top--];	//top-- //return the value in stack before top -1
	}
}
int priority(char x) {
	if (x == '('){
		return 0;
	}
	if (x == '+' || x == '-'){
		return 1;
	}
	if (x == '*' || x == '/'){
		return 2;
	}
}
int main(void) {
	char input[NUM];
	char* in;
	char x;
	printf("Insert the expression: ");
	scanf("%s", input);
	in = input;	//point to input array 

	while (*in != '\0')
	{
		if (isalnum(*in)){		//check if the char is alphabet or number
			printf("%c", *in);
		}
		else if (*in == '('){		//if is left parenthesis
			push(*in);
		}
		else if (*in == ')')		//if is right parenthesis
		{
			while ((x = pop()) != '('){
				printf("%c", x);
			}
		}
		else
		{
			while (priority(stack[top]) >= priority(*in)){
				printf("%c", pop());
			}
			push(*in);
			
		}
		in++;		//address +1, because it is array, change to next array index address
	}
	while (top != -1) {		//check whether the stack left something
		printf("%c", pop());
	}
	return 0;
}
