#include "MathIsFun.h"

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
	if (n == 1)
			return x % d;
	else if ( n % 2 == 0)
	{
		int pow = funPow(x, n/2, d);
		return (pow * pow) % d;
	}
	else
	{
		int pow = funPow(x, (n-1)/2, d);
		return (pow * pow * x) % d;
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
	bool isPrime = true;
	for (int i = 2 ; i <= funSqrt(x) ; i++)
	{
		if (x % i == 0)
		{
			isPrime = false;
		}
	}
	return isPrime;
}

bool funPalindromeCheck(int x) {
	if (x < 0)
		return false;

	else if (x == 0)
		return true;

	int new = 0;
	while (x > 0)
	{
		new *= 10;
		new += x % 10;
		x /= 10;
	}

	return x == new;
}
