#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 2000010
#define mb 1024 * 1024
#define MAX_LOOP 100

void testBroadcast(int rank, int buf)
{
    printf("[%d]: Before Bcast, buf is %d\n", rank, buf);
    if (rank == 0)
    {
        buf = 777;
    }
    MPI_Bcast(&buf, 1, MPI_INT, 0, MPI_COMM_WORLD);

    printf("[%d]: After Bcast, buf is %d\n", rank, buf);
}

void testScatterGather(int rank, int size)
{
    int *globalData = NULL, localData, temp;
    if (rank == 0)
    {
        globalData = malloc(size * sizeof(int));

        printf("Root has data:");

        for (int i = 0; i < size; i++)
        {
            temp = rand() % 20;
            globalData[i] = temp;
            printf(" %d", temp);
        }
        printf("\n");
    }

    MPI_Scatter(globalData, 1, MPI_INT, &localData, 1, MPI_INT, 0, MPI_COMM_WORLD);
    printf("Processor %d has data %d\n", rank, localData);
    localData *= 2;
    printf("Processor %d doubling the data, now has %d\n", rank, localData);

    MPI_Gather(&localData, 1, MPI_INT, globalData, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("Processor %d has data: ", rank);
        for (int i = 0; i < size; i++)
            printf("%d ", globalData[i]);
        printf("\n");
    }

    if (rank == 0)
        free(globalData);
}

void main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, size, buf = 0;
    double start, end;
    MPI_Status status;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // testBroadcast(rank, buf);
    testScatterGather(rank, size);

    MPI_Finalize();
}