#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 2000010
#define mb 1024 * 1024
#define MAX_LOOP 100

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    double *a, *b;
    double start, end;
    MPI_Status status;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    a = (double *)malloc(N * sizeof(double));
    b = (double *)malloc(N * sizeof(double));

    if (rank == 0)
    {
        printf("Number of doubles| Size (B) |Avg time (s)|Rate (Mb/sec)\n");
        printf("-----------------|----------|------------|-------------\n");
    }

    for (int i = 1; i < 1999999; i *= 2)
    {
        if (i > 1000000)
            i = 1000000;
        int total = i * sizeof(double);
        *a = (double)i;

        start = MPI_Wtime();

        if (rank == 0)
        {
            for (int j = 0; j < MAX_LOOP; j++)
            {
                MPI_Send(a, i, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
            }
        }
        else
        {
            for (int j = 0; j < MAX_LOOP; j++)
            {
                MPI_Recv(b, i, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
            }
        }

        MPI_Barrier(MPI_COMM_WORLD);

        end = MPI_Wtime();

        if (rank == 0)
        {
            double avgTime = (end - start) / MAX_LOOP;
            printf("%17d|%10d|%1.10f|%8.6f\n", i, total, avgTime, 1.0 * total / mb / avgTime);
        }
    }

    MPI_Finalize();
}