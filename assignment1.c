#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 2000010
#define mb 1024*1024

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);

  int rank, size;
  double *a, *b;
  double start, end;
  MPI_Status status;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_rank(MPI_COMM_WORLD, &size);

  a = (double *) malloc(N * sizeof(double));
  b = (double *) malloc(N * sizeof(double));

  for(int i = 1; i < 1999999; i*=2) {
    if (i > 1000000)
      i = 1000000;
    int total = i * sizeof(double);
    *(a+0) = (double)i;

    start = MPI_Wtime();
    
    if(rank == 0)
      MPI_Send(a, i, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
    else 
      MPI_Recv(b, i, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);

    MPI_Barrier(MPI_COMM_WORLD);

    end = MPI_Wtime();

    if (rank == 0) 
      printf("%d doubles\tsize %dB\ttime %.10fs\trate %.2f Mb/sec\n", i, total, end-start, (1.0*total/mb)/(end-start));
  }

  MPI_Finalize();
}