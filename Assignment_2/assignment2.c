#include <mpi.h>
#include <stdio.h>

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

void main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, size, buf = 0;
    double start, end;
    MPI_Status status;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_rank(MPI_COMM_WORLD, &size);

    testBroadcast(rank, buf);

    MPI_Finalize();
}