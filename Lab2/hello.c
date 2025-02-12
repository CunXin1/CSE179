#include <stdio.h>
#include "omp.h"

int main() {
    
    //set number of threads to 4
    omp_set_num_threads(4);

    #pragma omp parallel
    {
        printf("\nThread %d\n", omp_get_thread_num());
        printf("Number of threads: %d\n", omp_get_num_threads());
        printf("Hello World\n");
    }
    return 0;
}