#include <stdio.h>
#include <stdlib.h>
#include "omp.h"

// matrix-vector multiplication
int* matrix_vector_multi_static(int n, int A[10][10], int X[10]) {
    int* result = (int*) malloc(n * sizeof(int));
    double start = omp_get_wtime();
    
    #pragma omp parallel for schedule(static) num_threads(4)
    for (int i = 0; i < n; i++) {
        result[i] = 0;
        for (int j = 0; j < n; j++) {
            result[i] += A[i][j] * X[j];
        }
    }
    
    double end = omp_get_wtime();
    printf("Static scheduling time: %f ms\n", (end - start) * 1000);
    return result;
}

int* matrix_vector_multi_dynamic(int n, int A[10][10], int X[10]) {
    int* result = (int*) malloc(n * sizeof(int));
    double start = omp_get_wtime();
    
    #pragma omp parallel for schedule(dynamic) num_threads(4)
    for (int i = 0; i < n; i++) {
        result[i] = 0;
        for (int j = 0; j < n; j++) {
            result[i] += A[i][j] * X[j];
        }
    }
    
    double end = omp_get_wtime();
    printf("Dynamic scheduling time: %f ms\n", (end - start) * 1000);
    return result;
}

int* matrix_vector_multi_guided(int n, int A[10][10], int X[10]) {
    int* result = (int*) malloc(n * sizeof(int));
    double start = omp_get_wtime();
    
    #pragma omp parallel for schedule(guided) num_threads(4)
    for (int i = 0; i < n; i++) {
        result[i] = 0;
        for (int j = 0; j < n; j++) {
            result[i] += A[i][j] * X[j];
        }
    }
    
    double end = omp_get_wtime();
    printf("Guided scheduling time: %f ms\n", (end - start) * 1000);
    return result;
}

int main() {
    // set number of threads to 4
    omp_set_num_threads(4);
    printf("Number of threads: %d\n", omp_get_max_threads());

    int n = 10;
    int A[10][10] = {
        {1,  2,  3,  4,  5,  6,  7,  8,  9,  10},
        {11, 12, 13, 14, 15, 16, 17, 18, 19, 20},
        {21, 22, 23, 24, 25, 26, 27, 28, 29, 30},
        {31, 32, 33, 34, 35, 36, 37, 38, 39, 40},
        {41, 42, 43, 44, 45, 46, 47, 48, 49, 50},
        {51, 52, 53, 54, 55, 56, 57, 58, 59, 60},
        {61, 62, 63, 64, 65, 66, 67, 68, 69, 70},
        {71, 72, 73, 74, 75, 76, 77, 78, 79, 80},
        {81, 82, 83, 84, 85, 86, 87, 88, 89, 90},
        {91, 92, 93, 94, 95, 96, 97, 98, 99, 100}
    };
    int X[10] = {5, 10, 15, 20, 25, 30, 35, 40, 45, 50};

    int* B1 = matrix_vector_multi_static(n, A, X);
    int* B2 = matrix_vector_multi_dynamic(n, A, X);
    int* B3 = matrix_vector_multi_guided(n, A, X);

    // print result
    printf("Result: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", B1[i]);
    }
    printf("\n");
    
    return 0;
}
