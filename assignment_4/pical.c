#include <stdio.h>
#include <time.h>
#include <omp.h>

#define NUMSTEPS 1000000

int pi()
{
    double x, sum = 0.0, pi, step;
    step = 1.0 / (double)NUMSTEPS;

    for (int i = 0; i < NUMSTEPS; ++i)
    {
        x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }
    pi = step * sum;
    return 0;
}

int pi_openmp()
{
    int i;
    double x, sum = 0.0, pi, step;
    step = 1.0 / (double)NUMSTEPS;
    
    printf("This line is out of pragma omp. In parallel?:\t%d\n", omp_in_parallel());
    omp_set_dynamic(1);
    omp_set_nested(1);
    #pragma omp parallel private(i, x)
    {
        int tid = omp_get_thread_num();
        if (tid == 0){
            printf("Number of procs:\t%d\n", omp_get_num_procs());
            printf("Number of threads using:\t%d\n", omp_get_num_threads());
            printf("Max number of threads available:\t%d\n", omp_get_thread_limit());
            printf("This line is in pragma omp. In parallel? :\t%d\n", omp_in_parallel());
            printf("Dynamic enabled? : \t%d\n", omp_get_dynamic());
            printf("Nested? :\t%d\n", omp_get_nested());
        }

        #pragma omp for reduction(+:sum) schedule(static)
        for (i = 0; i < NUMSTEPS; ++i)
        {
            x = (i + 0.5) * step;
            sum += 4.0 / (1.0 + x * x);
        }
    }
    pi = step * sum;
    return 0;
}

int main()
{
    clock_t begin, end;

    begin = clock();
    pi();
    end = clock();
    printf("Normal pical time taken: \t%f\n", (double)(end - begin) / CLOCKS_PER_SEC);

    begin = clock();
    pi_openmp();
    end = clock();
    printf("Omp pical time taken: \t%f\n", (double)(end - begin) / CLOCKS_PER_SEC);
    return 0;
}