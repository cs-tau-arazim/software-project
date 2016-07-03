#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 30
#define Mac(a,b)	(a == b)? (a* 0.5) : (a * 1.2)
void swap (const char** ptr_s1, const char** ptr_s2)
{
	const char* temp = *ptr_s1;
	*ptr_s1 = *ptr_s2;
	*ptr_s2 = temp;
}

struct t{
	char* name;
	int age;
};

void foo(struct t tmp)
{
	char st[] = "here";
	strcpy(tmp.name,st);
	tmp.age = 0;
}

int x,y=2;
int f1 (int x, int a)
{
	x = x+2;
	a = a+3;
	y = a*x;
	return x;
}

void f2 (int *a, int *b)
{
	*a = 4;
	*b += 2;
	 x = 2;
	*a = *b + *a;
}

void print_tx(int* arr, int length, int (*tx)(int))
{
	int i;
	for(i=0; i<length; ++i)
		printf ("%d ", tx(arr[i]));
}
void print_tx2(int* arr, int length, int (*tx)(int))
{
	int i;
	for(i=0; i<length; ++i)
		printf ("%d ", tx(arr[i]));
}

int s(int x) {return x*x; }
int c(int x) {return x*s(x); }

int func(unsigned int a, unsigned int b)
{
	int result = 0;
	while(b!=0)
	{
		if (b&1)
			result += a;
		b >>=1;
		a <<=1;
	}
	return result;
}
int main(int argc, char* argv[]) {
	putchar(*argv[0]);
	/*
	int arr[] = {0,1,2,3,4,5};
	print_tx(arr,6,s);
	print_tx2(arr,6,c);
	return 0;

	x = 1;
	y = f1(x+y, y);
	f2(&x, &y);
	printf ("x = %d and y = %d\n", x, y);
	return 0;


	int i=0;
	char j[22] = "This is a long string", k[3];
	scanf("%2s",k);
	sprintf(j,k);
	for (;i<21;printf("%c",j[i++]));

	int i = 1, j = 2;
	int k = Mac(i,--j);
	printf("%d,%d,%d\n",i,j,k);
	return 0;
	*/
}
