/*
**  PROGRAM: A simple serial producer/consumer program
**
**  One function generates (i.e. produces) an array of random values.
**  A second functions consumes that array and sums it.
**
**  HISTORY: Written by Tim Mattson, April 2007.
*/
#include "omp.h"
#include <stdlib.h>
#include <stdio.h>

#define N 1000000000

/* Some random number constants from numerical recipies */
#define SEED 2531
#define RAND_MULT 1366
#define RAND_ADD 150889
#define RAND_MOD 714025
int randy = SEED;

/* function to fill an array with random numbers */
void fill_rand(int length, double *a)
{
  int i;
  for (i = 0; i < length; i++)
  {
    randy = (RAND_MULT * randy + RAND_ADD) % RAND_MOD;
    *(a + i) = ((double)randy) / ((double)RAND_MOD);
  }
}

/* function to sum the elements of an array */
double Sum_array(int length, double *a)
{
  int i;
  double sum = 0.0;


#pragma omp parallel for reduction(+ : sum)

  for (i = 0; i < length; i++)
  {
    sum += *(a + i);
  }

  return sum;
}

double Sum_array1(int length, double *a)
{
  double sum = 0.0;
  int i;

#pragma omp parallel
  {
    double local_sum = 0.0;
#pragma omp for
    for (i = 0; i < length; i++)
    {
      local_sum += a[i];
    }
#pragma omp atomic
    sum += local_sum;
  }
  return sum;
}

int main()
{
  double *A, sum, runtime;
  int flag = 0;

  A = (double *)malloc(N * sizeof(double));

  omp_set_num_threads(2);
  printf("Number of threads: %d\n", omp_get_max_threads());


  runtime = omp_get_wtime();

  fill_rand(N, A); // Producer: fill an array of data

  sum = Sum_array(N, A); // Consumer: sum the array

  runtime = omp_get_wtime() - runtime;

  printf("For Sum_array, In %lf seconds, The sum is %lf \n", runtime, sum);

  
  omp_set_num_threads(4);
  printf("Number of threads: %d\n", omp_get_max_threads());


  runtime = omp_get_wtime();

  fill_rand(N, A); // Producer: fill an array of data

  sum = Sum_array(N, A); // Consumer: sum the array

  runtime = omp_get_wtime() - runtime;

  printf("For Sum_array, In %lf seconds, The sum is %lf \n", runtime, sum);

  
  omp_set_num_threads(8);
  printf("Number of threads: %d\n", omp_get_max_threads());


  runtime = omp_get_wtime();

  fill_rand(N, A); // Producer: fill an array of data

  sum = Sum_array(N, A); // Consumer: sum the array

  runtime = omp_get_wtime() - runtime;

  printf("For Sum_array, In %lf seconds, The sum is %lf \n", runtime, sum);

  
  omp_set_num_threads(16);
  printf("Number of threads: %d\n", omp_get_max_threads());


  runtime = omp_get_wtime();

  fill_rand(N, A); // Producer: fill an array of data

  sum = Sum_array(N, A); // Consumer: sum the array

  runtime = omp_get_wtime() - runtime;

  printf("For Sum_array, In %lf seconds, The sum is %lf \n", runtime, sum);

}
