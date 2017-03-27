/* Read a number and determine if it is prime.
*/
#include <stdio.h>

#include "prime.h"

int
main(int argc, char **argv) {
	int n;
	printf("Enter a number n: ");
	scanf("%d", &n);
	if (isprime(n)) {
		printf("%d is a prime number\n", n);
	} else {
		printf("%d is not a prime number\n", n);
	}
	printf("The next prime is : %d\n", nextprime(n));
	return 0;
}

