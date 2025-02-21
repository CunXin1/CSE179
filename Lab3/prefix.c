#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define N 40960000
#define NUM_THREADS 4

int main() {
    omp_set_num_threads(NUM_THREADS);

    // Allocate memory for input and prefix sum arrays
    int *X      = (int *)malloc(N * sizeof(int));
    int *prefix = (int *)malloc(N * sizeof(int));

    int T[NUM_THREADS]; // Store last element of each thread's block

    // Timing variables
    double t1, t2, t3, t4;
    double start, end;

    // Initialize input array with random values
    srand((unsigned int)time(NULL));
    for(int i = 0; i < N; i++){
        X[i] = rand() % 100;
    }

    // Step 1: Compute local prefix sum for each thread
    start = omp_get_wtime();
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int chunk_size = N / NUM_THREADS;
        int start_idx  = tid * chunk_size;
        int end_idx;
        if (tid == NUM_THREADS - 1) {
            end_idx = N;
        } else {
            end_idx = start_idx + chunk_size;
        }

        prefix[start_idx] = X[start_idx];
        for(int i = start_idx + 1; i < end_idx; i++) {
            prefix[i] = prefix[i - 1] + X[i];
        }
    }
    end = omp_get_wtime();
    t1 = end - start;

    // Step 2: Collect the last element of each thread's block
    start = omp_get_wtime();
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int chunk_size = N / NUM_THREADS;
        int end_idx;
        if (tid == NUM_THREADS - 1) {
            end_idx = N;
        } else {
            end_idx = (tid + 1) * chunk_size;
        }
        T[tid] = prefix[end_idx - 1];
    }
    end = omp_get_wtime();
    t2 = end - start;

    // Step 3: Compute offsets using prefix sum on T
    start = omp_get_wtime();
    for(int i = 1; i < NUM_THREADS; i++){
        T[i] += T[i - 1];
    }
    end = omp_get_wtime();
    t3 = end - start;

    // Step 4: Adjust each thread's block with its offset
    start = omp_get_wtime();
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        if(tid > 0) {
            int chunk_size = N / NUM_THREADS;
            int start_idx  = tid * chunk_size;
            int end_idx;
            if (tid == NUM_THREADS - 1) {
                end_idx = N;
            } else {
                end_idx = start_idx + chunk_size;
            }

            int offset = T[tid - 1];
            for(int i = start_idx; i < end_idx; i++){
                prefix[i] += offset;
            }
        }
    }
    end = omp_get_wtime();
    t4 = end - start;

    // Print execution time for each step
    printf("Number of threads: %d\n", omp_get_max_threads());
    printf("Total sum = %d\n", prefix[N - 1]);
    printf("Step 1 time: %lf sec\n", t1);
    printf("Step 2 time: %lf sec\n", t2);
    printf("Step 3 time: %lf sec\n", t3);
    printf("Step 4 time: %lf sec\n", t4);
    printf("Total execution time: %lf sec\n", t1 + t2 + t3 + t4);

    free(X);
    free(prefix);
    return 0;
}
