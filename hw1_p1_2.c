#include<stdio.h>
#include<string.h> 
#include<stdlib.h>
#define NUM 30

char stack[NUM], str[NUM];
int top = -1;

void push(char x) {
	stack[++top] = x;		//++top//top +1 before the x is inserted in stack array
}

char pop() {
		return stack[top--];	//top-- //return the value in stack before top -1
}

int checkOperator(char x){
    switch(x){
        case '+':
        case '-':
        case '/':
        case '*':
            return 1;
    }
    return 0;
}

int main(void) {
    
    int receiveStringLength, count1, count2 = 0;
    char stack2[NUM];
    char a;//use to receive the non-important char(using for the judgement)

  printf("Enter the postfix expression\n");
  scanf("%s", str);     //char array

  receiveStringLength = strlen(str);        //receive string length

  for (count1 = 0; count1 < NUM; count1++){     //initialize stack and stack2 array to null
    stack[count1] = '\0';
    stack2[count1] = '\0';
  }
    
  printf("Prefix expression is:\t");

  for (count1 = receiveStringLength - 1; count1 >= 0; count1--)    //because the index is the actual string length subtracted by one 
  {
    if (checkOperator(str[count1]) == 1)  //check if it is a operator
    {
      push(str[count1]); //push into the stack
    } else  //if not an operator
    {
      stack2[count2++] = str[count1];  //push into stack2
      while ((top != -1) && (stack[top] == '#')) 
      {
        a = pop();  //receive '#'
        stack2[count2++] = pop();     //receive the actual character that is needed
      }
      push('#');    //push '#' into stack (using for judgement)
    }
  }
  stack2[count2] = '\0';         //add one null element at the top of the stack2

  count1 = 0;
  count2 = strlen(stack2) - 1;
  char tempstack[NUM];  //using for the final output

  while (stack2[count1] != '\0') {
    tempstack[count2--] = stack2[count1++];
  }
for(count1=0;count1<strlen(stack2);count1++){  //tempstack[0] does not have any char, so the strlen doesnt need to -1
    if(tempstack[count1] != '\0'){       //if not null then print out
  printf("%c", tempstack[count1]);      //output
    }
}
	return 0;
}
