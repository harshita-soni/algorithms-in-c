/*
problem3.c

Driver function for Problem 3.

Skeleton written by Grady Fitzpatrick for COMP20007 Assignment 1 2021
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <limits.h>

/* Constants */
#define OLDCHIP 0
#define NEWCHIP 1
#define MAXNUMERATOR 100
#define MAXDENOMINATOR 100
#define ASSIGNMENT 1
#define DIVISION 5
#define MODULO 5
#define IFBRANCH 1
#define WHILECHECK 1

/* Used to store all the statistics for a single chip. */
struct statistics;

/* Used to store all the statistics for both chips for each problem. */
struct chipStatistics;

struct statistics {
  int operations;
  int instances;
  int minOperations;
  double avgOperations;
  int maxOperations;
};

struct chipStatistics {
  struct statistics oldChipEuclid;
  struct statistics newChipEuclid;
  struct statistics oldChipSieve;
  struct statistics newChipSieve;
};

/* Set all statistics to 0s */
void initialiseStatistics(struct statistics *stats);

/* Collects the minimum, average and maximum operations from running all
combinations of numerators from 1 to the given maxNumerator and 1 to the given
maxDenominator. */
void collectStatistics(struct chipStatistics *chipStats, int maxNumerator,
  int maxDenominator);

/* Divides the number of operations by the number of instances. */
void calculateAverage(struct statistics *stats);

/* Prints out the minimum, average and maximum operations from given
statistics. */
void printStatistics(struct statistics *stats);

/* Updates statistics for every iteration of either of the two algorithms. */
void updateStats(struct statistics *s, int ops);

/* Calculates the number of operations required for Euclid's algorithm given the
numerator and denominator when running on the given chip type (one of OLDCHIP
and NEWCHIP) by moving through the steps of the algorithm and counting each
pseudocode operation. */
void euclid(int numerator, int denominator, int chip, struct statistics *s);

/* Calculates the number of operations required for the sieve of Eratosthenes
given the numerator and denominator when running on the given chip type (one of
OLDCHIP and NEWCHIP) by moving through the steps of the algorithm and counting
each pseudocode operation. */
void eratosthenes(int numerator, int denominator, int chip,
  struct statistics *s);

int main(int argc, char **argv){
  struct chipStatistics summaryStatistics;

  collectStatistics(&summaryStatistics, MAXNUMERATOR, MAXDENOMINATOR);

  printf("Old chip (Euclid):\n");
  printStatistics(&(summaryStatistics.oldChipEuclid));
  printf("\n");
  printf("New chip (Euclid)\n");
  printStatistics(&(summaryStatistics.newChipEuclid));
  printf("\n");
  printf("Old chip (Sieve)\n");
  printStatistics(&(summaryStatistics.oldChipSieve));
  printf("\n");
  printf("New chip (Sieve)\n");
  printStatistics(&(summaryStatistics.newChipSieve));
  printf("\n");

  return 0;
}

void collectStatistics(struct chipStatistics *chipStats, int maxNumerator,
  int maxDenominator){
  int numerator, denominator;
  /* Initialise all statistics */
  initialiseStatistics(&(chipStats->oldChipEuclid));
  initialiseStatistics(&(chipStats->newChipEuclid));
  initialiseStatistics(&(chipStats->oldChipSieve));
  initialiseStatistics(&(chipStats->newChipSieve));

  for(numerator = 1; numerator <= maxNumerator; numerator++){
    for(denominator = 1; denominator <= maxDenominator; denominator++){
      /* Run algorithms for all combinations of numerator and denominator. */
      euclid(numerator, denominator, OLDCHIP,
        &(chipStats->oldChipEuclid));
      euclid(numerator, denominator, NEWCHIP,
        &(chipStats->newChipEuclid));
      eratosthenes(numerator, denominator, OLDCHIP,
        &(chipStats->oldChipSieve));
      eratosthenes(numerator, denominator, NEWCHIP,
        &(chipStats->newChipSieve));
    }
  }
  calculateAverage(&(chipStats->oldChipEuclid));
  calculateAverage(&(chipStats->newChipEuclid));
  calculateAverage(&(chipStats->oldChipSieve));
  calculateAverage(&(chipStats->newChipSieve));
}

void calculateAverage(struct statistics *stats){
  stats->avgOperations = (double) stats->operations / stats->instances;
}

void initialiseStatistics(struct statistics *stats){
  stats->operations = 0;
  stats->instances = 0;
  stats->minOperations = INT_MAX;
  stats->avgOperations = 0;
  stats->maxOperations = 0;
}

void euclid(int numerator, int denominator, int chip, struct statistics *s){
  /* IMPLEMENT THIS */
  int ops = 0; 

  int a = denominator, b = numerator, t;
  ops += 2*ASSIGNMENT;
  
  while (b) {
    ops += WHILECHECK;
    t = b;
    b = a % b;
    a = t;
    ops = ops + 3*ASSIGNMENT + 1*MODULO;
  }
  ops += WHILECHECK; // for the failed condition check in the while loop

  // printf("%d / %d", numerator / a, denominator / a);
  updateStats(s, ops);
}

void eratosthenes(int numerator, int denominator, int chip,
  struct statistics *s){
  int i, j, ops = 0;
  int num = numerator, den = denominator;
  ops += 2*ASSIGNMENT;

  int numCandidates;
  if (num < den) {
    numCandidates = num;
  } else {
    numCandidates = den;
  }
  ops += ASSIGNMENT;    // assumption: finding minimum has 0 operational cost

  int primes[numCandidates+1];
  ops += ASSIGNMENT;    // assigning pointer to the first element of the array

  for (i = 0; i < (numCandidates + 1); i++) {
    primes[i] = 1;
    // assumption: filling the array with 1s has 0 operational cost
  }

  i = 1;
  ops += ASSIGNMENT;

  while (i < numCandidates) {
    ops += WHILECHECK;

    i += 1;
    ops += ASSIGNMENT;

    ops += IFBRANCH;
    if (primes[i]) {
      j = i + i;
      if (chip == OLDCHIP)
        ops += ASSIGNMENT; 
      else 
        ops += 1;     //operational cost for new chip for the entire section  

      while (j <= numCandidates) {
        if (chip == OLDCHIP)
          ops += WHILECHECK;

        if ((j / i) > 1 && (j % i) == 0) {
          primes[j] = 0;
        }
        j += i;

        if (chip == OLDCHIP) {
          ops += IFBRANCH;               
          // for each evaluation of "if ((j / i) > 1 && (j % i) == 0)"

          ops = ops + DIVISION + MODULO; // for "(j / i) > 1 && (j % i) == 0"
          ops += ASSIGNMENT;             // for "primes[j] = 0;"
          ops += ASSIGNMENT;             // for "j += i;"
        } 
        
      }
      if (chip == OLDCHIP)
        ops += WHILECHECK; // failed while while (j <= numCandidates)

      while ((num % i) == 0 && (den % i) == 0) {
        ops = ops + WHILECHECK + 2*MODULO;

        num = num / i;
        den = den / i;
        ops += 2*DIVISION + 2*ASSIGNMENT;
      }
      ops = ops + WHILECHECK + 2*MODULO; 
      //failed while ((num % i) == 0 && (den % i) == 0)
    }
  }
  ops += WHILECHECK;   //failed while (i < numCandidates)
  updateStats(s, ops);
}

void printStatistics(struct statistics *stats){
  printf("Minimum operations: %d\n", stats->minOperations);
  printf("Average operations: %f\n", stats->avgOperations);
  printf("Maximum operations: %d\n", stats->maxOperations);
}

void updateStats(struct statistics *s, int ops) {
  s->instances += 1;
  s->operations += ops;
  if (ops < s->minOperations) s->minOperations = ops;
  if (ops > s->maxOperations) s->maxOperations = ops;
}