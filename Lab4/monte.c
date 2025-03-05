#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>

#define TOTAL_POINTS 100000000  // Total points
#define PI_TRUE 3.141592653589793

int num_threads; 
long points_per_thread; 
long total_hits = 0;
pthread_mutex_t mutex;

void* monte() {
    long hit = 0;

    // Set random seed
    struct timeval tv;
    gettimeofday(&tv, NULL);
    srand(tv.tv_usec * 114514 + pthread_self());

    for (long i = 0; i < points_per_thread; i++) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        if (x * x + y * y <= 1.0)
            hit++;
    }

    // Update total_hits safely
    pthread_mutex_lock(&mutex);
    total_hits += hit;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main() {
    int thread_counts[] = {2, 4, 8, 16};
    int num_tests = sizeof(thread_counts) / sizeof(thread_counts[0]);

    pthread_mutex_init(&mutex, NULL);

    for (int test = 0; test < num_tests; test++) {
        num_threads = thread_counts[test];
        points_per_thread = TOTAL_POINTS / num_threads;
        pthread_t threads[num_threads];

        struct timeval start, end;
        gettimeofday(&start, NULL);

        total_hits = 0;

        // 创建线程
        for (int i = 0; i < num_threads; i++) {
            if (pthread_create(&threads[i], NULL, monte, NULL) != 0) {
                perror("pthread_create error");
                exit(EXIT_FAILURE);
            }
        }

        // 等待所有线程执行完毕
        for (int i = 0; i < num_threads; i++) {
            pthread_join(threads[i], NULL);
        }

        gettimeofday(&end, NULL);
        double time_used = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;

        double pi_estimate = 4.0 * total_hits / TOTAL_POINTS;
        double error_percentage = fabs((pi_estimate - PI_TRUE) / PI_TRUE) * 100;

        printf("Threads: %d, Estimate: %.6f, Time: %.2f ms, %%Error: %.4f%%\n",
               num_threads, pi_estimate, time_used, error_percentage);
    }

    pthread_mutex_destroy(&mutex); 
    return 0;
}