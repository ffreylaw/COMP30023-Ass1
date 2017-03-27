/* Read a number and determine if it is prime.
*/
#include <stdio.h>

#include "prime.h"


/* Determine whether argument is prime. */
int
isprime(int n) {
	int divisor;
	if (n<2) {
		return 0;
	}
	for (divisor=2; divisor*divisor<=n; divisor++) {
	       if (n%divisor==0) {
		       /* factor found, so can't be prime */
		       return 0;
		}
	}
	/* no factors, so must be prime */
	return 1;
}

int
nextprime(int n) {
	n = n+1;
	while (!isprime(n)) {
		n = n+1;
	}
	return n;
}

/* =====================================================================
   Program written by Alistair Moffat, as an example for the book
   "Programming, Problem Solving, and Abstraction with C", Pearson
   SprintPrint, Sydney, Australia, 2003.

   See http://www.cs.mu.oz.au/~alistair/ppsaa/ for further information.

   This version prepared January 29, 2003.
   ================================================================== */
