#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int matmul_omp(int m, int n, int p, int *A, int *B, int *res)
{
    int i, j, k;

    #pragma omp parallel shared(A, B, res) private(i, j, k)
    {
        #pragma omp for schedule(static)
        for (i = 0; i < m; ++i)
            for (j = 0; j < p; ++j)
                for (k = 0; k < n; ++k)
                    *(res + i*n + j) = *(res + i*n + j) + *(A + i*p + k) * *(B + k*n + j);
    }

    return 0;
}

int matmul(int m, int n, int p, int *A, int *B, int *res)
{
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < p; ++j)
            for (int k = 0; k < n; ++k)
                *(res + i*n + j) = *(res + i*n + j) + *(A + i*p + k) * *(B + k*n + j);
    return 0;
}

int main()
{
    int m = 100, n = 200, p = 300;
    int i, j;
    int *A, *B, *res;
    clock_t begin, end;

    A = (int *)malloc(m * n * sizeof(int));
    B = (int *)malloc(n * p * sizeof(int));
    res = (int *)malloc(m * p * sizeof(int));

    for (i = 0; i < m; ++i)
        for (j = 0; j < n; ++j)
            *(A + i * p + j) = i*n + j;

    for (i = 0; i < n; ++i)
        for (j = 0; j < p; ++j)
            *(B + i * n + j) = i*p + j;
    
    for (i = 0; i < m; ++i)
        for (j = 0; j < p; ++j)
            *(res + i * n + j) = 0;

    begin = clock();
    matmul(m, n, p, A, B, res);
    end = clock();
    printf("Normal matmul time taken: \t%f\n", (double)(end - begin) / CLOCKS_PER_SEC);

    begin = clock();
    matmul_omp(m, n, p, A, B, res);
    end = clock();
    printf("Omp matmul time taken: \t%f\n", (double)(end - begin) / CLOCKS_PER_SEC);

}