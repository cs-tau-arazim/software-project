/*
 * main.c
 *
 *  Created on: Mar 22, 2016
 *      Author: Tom
 */
#include <MathIsFun.c>

int main()
{
	printf("Welcome to Math Is Fun - beta version\n");
	printf("Supported operations are:\n");
	printf("1 - Power calculation\n");
	printf("2 - Prime test\n");
	printf("3 - Palindrome test\n");
	printf("Please enter operation number (1/2/3): ");



	int oper = 0;

	scanf("%d", &oper);

	if (oper == 1)
	{
		printf("Please enter three space separated numbers: ");
		int x;
		int n;
		int d;

		scanf("%d %d %d", &x, &n, &d);

		printf("res = %d" , funPow(x,n,d));
	}
	else if (oper == 2)
	{
		printf("Please enter an integer: ");
		int n;
		scanf("%d", &n);

		printf("res = %s" , funPrimeCheck(n) ? "true" : "false");
	}
	else if (oper == 3)
	{
		printf("Please enter an integer: ");
		int n;
		scanf("%d", &n);

		printf("res = %s" , funPalindromeCheck(n) ? "true" : "false");
	}

}
