#include <stdio.h>
#include <time.h>

#define NUMSTEPS 10000

int pi_openmp()
{
    int i;
    double x, sum = 0.0, pi, step;
    step = 1.0 / (double)NUMSTEPS;

    #pragma omp parallel private(i, x)
    {
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

int main()
{
    clock_t begin, end;

    begin = clock();
    pi_openmp();
    end = clock();
    printf("Normal pical time taken: \t%f\n", (double)(end - begin) / CLOCKS_PER_SEC);

    begin = clock();
    pi_openmp();
    end = clock();
    printf("Omp pical time taken: \t%f\n", (double)(end - begin) / CLOCKS_PER_SEC);
    return 0;
}