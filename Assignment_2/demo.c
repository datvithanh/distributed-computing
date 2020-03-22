#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define N 2000010
#define mb 1024 * 1024
#define MAX_LOOP 100

void printData(int data[])
{
    for (int i = 0; i < sizeof(data); i++)
    {
        printf(" %d", data[i]);
    }
    printf("\n");
}

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
    srand(time(0));
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

void testAllGather(int rank, int size)
{
    srand(time(0));
    int *globalData = NULL, *localData = NULL, temp;
    globalData = malloc(size * sizeof(int));
    localData = malloc(size * sizeof(int));
    for (int i = 0; i < size; i++)
    {
        globalData[i] = 0;
        localData[i] = 0;
    }
    if (rank == 0)
    {
        for (int i = 0; i < size; i++)
        {
            globalData[i] = rand() % 20;
        }
    }

    printf("Before globalData: P %d:", rank);
    printData(globalData);
    printf("Before localData: P %d:", rank);
    printData(localData);

    MPI_Allgather(globalData, size, MPI_INT, localData, size, MPI_INT, MPI_COMM_WORLD);

    printf("After globalData: P %d:", rank);
    printData(globalData);
    printf("After localData: P %d:", rank);
    printData(localData);

    free(globalData);
    free(localData);
}

void testAllToAll(int rank, int size)
{
    srand(time(0));
    int *globalData = NULL, *localData = NULL, temp;
    globalData = malloc(size * sizeof(int));
    localData = malloc(size * sizeof(int));
    for (int i = 0; i < size; i++)
    {
        globalData[i] = 0;
        localData[i] = 0;
    }
    if (rank == 0)
    {
        for (int i = 0; i < size; i++)
        {
            globalData[i] = rand() % 20;
        }
    }

    printf("Before globalData: P %d:", rank);
    printData(globalData);
    printf("Before localData: P %d:", rank);
    printData(localData);

    MPI_Alltoall(globalData, size, MPI_INT, localData, size, MPI_INT, MPI_COMM_WORLD);

    printf("After globalData: P %d:", rank);
    printData(globalData);
    printf("After localData: P %d:", rank);
    printData(localData);

    free(globalData);
    free(localData);
}

void testReduce(int rank, int size)
{
    srand(time(0));
    int *globalData = NULL, *localData = NULL, temp, localSum = 0, globalSum = 0;
    globalData = malloc(size * sizeof(int));
    localData = malloc(size * sizeof(int));
    for (int i = 0; i < size; i++)
    {
        globalData[i] = 0;
        localData[i] = 0;
    }
    for (int i = 0; i < size; i++)
    {
        localData[i] = rand() % 20;
        localSum += localData[i];
    }

    printf("Before globalSum: P %d: %d\n", rank, globalSum);
    printf("Before localSum: P %d: %d\n", rank, localSum);

    MPI_Reduce(&localSum, &globalSum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    printf("After globalSum: P %d: %d\n", rank, globalSum);
    printf("After localSum: P %d: %d\n", rank, localSum);
    free(globalData);
    free(localData);
}

void testAllReduce(int rank, int size)
{
    srand(time(0));
    int *globalData = NULL, *localData = NULL, temp, localSum = 0, globalSum = 0;
    globalData = malloc(size * sizeof(int));
    localData = malloc(size * sizeof(int));
    for (int i = 0; i < size; i++)
    {
        globalData[i] = 0;
        localData[i] = 0;
    }
    for (int i = 0; i < size; i++)
    {
        localData[i] = rand() % 20;
        localSum += localData[i];
    }

    printf("Before globalSum: P %d: %d\n", rank, globalSum);
    printf("Before localSum: P %d: %d\n", rank, localSum);

    MPI_Reduce(&localSum, &globalSum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    printf("After globalSum: P %d: %d\n", rank, globalSum);
    printf("After localSum: P %d: %d\n", rank, localSum);
    free(globalData);
    free(localData);
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
    // testScatterGather(rank, size);
    testAllGather(rank, size);
    // testAllToAll(rank, size);
    // testReduce(rank, size);
    // testAllReduce(rank,size);

    MPI_Finalize();
}