/* A linear congruent pseudorandomized numbers (PRN) generetor follows
 * the following formula:
 * prn_(i+1) = (RAND_MULTIPLIER * prn_i + b) % maxPrn
 * where 'b' will be defined as 0 (zero) and maxPrn is the greatest
 * value of a unsigned long int (64 bits) number.
 */

#include <limits.h>
#include "define.h"

#define RAND_MULTIPLIER 1ul //78125ul //16807ul
#define MAX_PRN         ULINT_MAX

unsigned long int
getNextRandomInt ()
{
  /* PRN equals to pseudorandomized number. */
  static unsigned long int prn = SEED;
  prn = (RAND_MULTIPLIER * prn) % MAX_PRN;
  return prn;
}

double
getNextRandom0toX (double x)
{
  return x*getNextRandomInt()/MAX_PRN;
}
