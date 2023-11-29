#include<stdio.h>

int main() {
	
	int num1, num2;
	float result;
	char ch;
	
	printf("Enter an operator you would like to perform (+, -, *, /, %): ");
	scanf("%c", &ch);
	
	printf("Enter your first number: ");
	scanf("%d", &num1);
	
	printf("Enter your second number: ");
	scanf("%d", &num2);
	
	switch(ch){
		case '+':
            result=num1+num2;
            break;
             
        case '-':
            result=num1-num2;
            break;
         
        case '*':
            result=num1*num2;
            break;
             
        case '/':
            result=(float)num1/(float)num2;
            break;
             
        case '%':
            result=num1%num2;
            break;
        default:
            printf("Invalid operation.\n");
        }

    printf("Result: %d %c %d=%f", num1 ,ch, num2,result);
	return 0;	
}
