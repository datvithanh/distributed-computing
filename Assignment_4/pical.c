#include <stdio.h>
#include <time.h>
#include <omp.h>

#define NUMSTEPS 1000000

int pi_openmp()
{
    int i;
    double x, sum = 0.0, pi, step;
    step = 1.0 / (double)NUMSTEPS;

#pragma omp parallel private(i, x)
    {
#pragma omp for reduction(+ \
                          : sum)
        for (i = 0; i < NUMSTEPS; ++i)
        {
            x = (i + 0.5) * step;
            sum += 4.0 / (1.0 + x * x);
        }
#pragma omp master
        {
            printf("Number of processors available: %d\n", omp_get_num_procs());
            printf("Number of threads being used: %d\n", omp_get_num_threads());
            printf("Maximum number of threads available: %d\n", omp_get_max_threads());
            printf("In parallel region: %s\n", omp_in_parallel() ? "true" : "false");
            printf("Dynamic threads are enabled: %s\n", omp_get_dynamic() ? "true" : "false");
            printf("Nested parallelism is supported: %s\n", omp_get_nested() ? "true" : "false");
        }
    }
    pi = step * sum;
    return 0;
}

int main()
{
    clock_t begin, end;
    omp_set_num_threads(2);
    // begin = clock();
    // pi();
    // end = clock();
    // printf("Normal pical time taken: \t%f\n", (double)(end - begin) / CLOCKS_PER_SEC);

    begin = clock();
    pi_openmp();
    end = clock();
    printf("Omp pical time taken: \t%f\n", (double)(end - begin) / CLOCKS_PER_SEC);
    return 0;
}