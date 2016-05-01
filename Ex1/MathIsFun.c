#include "MathIsFun.h"
#include <stdio.h>

/*
 * Calculates the largest integer less or equal than the square root of x.
 * funSqrt(10) = 3
 * funSqrt(16) = 4
 * @param x - An integer for which the function applies
 * @return the value of |_sqrt(x)_|, if x is negative then the function
 * returns -1.
 */
int funSqrt(int x); // ==> Declaration is always in the beginning of the file.

int funPow(int x, int n, int d) {

	if (n == 0)
		return 1;
	while (x < 0)
		x += d;
	if (n == 1)
		return x % d;
	else if ( n % 2 == 0)
	{
<<<<<<< Updated upstream
		int pow = funPow(x, n/2, d) % d;
=======
		int pow = funPow(x, n/2, d);
		printf("x = %d n = %d d = %d pow = %d\n" , x, n/2, d, pow);
		printf("res = %d\n" , (pow * pow)  );
>>>>>>> Stashed changes
		return (pow * pow) % d;
	}
	else
	{
<<<<<<< Updated upstream
		int pow = funPow(x, (n-1)/2, d) % d;
=======
		int pow = funPow(x, (n-1)/2, d);
		printf("x = %d n = %d d = %d pow = %d\n" , x, (n-1)/2, d, pow);
		printf("res = %d\n" , (pow * pow * x) );
>>>>>>> Stashed changes
		return (((pow * pow) % d) * x) % d;
	}
}

int funSqrt(int x) {
	 int big = x;
	 int small = 1;

	 while ((small+1)*(small+1) <= x)
	 {
		 int mid = (big+small)/2;
		 if (mid*mid > x)
		 {
			 big = mid;
		 }
		 else
		 {
			 small = mid;
		 }
	 }
	 return small;
}

bool funPrimeCheck(int x) {
	if (x <= 1)
		return false;
	for (int i = 2 ; i <= funSqrt(x) ; i++)
	{
		if (x % i == 0)
		{
			return false;
		}
	}
	return true;
}

bool funPalindromeCheck(int x) {
	int y = x; 	
	if (y < 0)
		return false;
	
	else if (y == 0)
		return true;

	int new = 0;
	while (y > 0)
	{
		new *= 10;
		new += y % 10;
		y /= 10;
	}
	return x == new;
}
