/*
- 20164803 - Vi Thành Đạt
- 20189669 - Đào Đức Minh
- 20155365 - Nguyễn Duy Đoàn
- 20162145 - Nguyễn Đình Khánh
*/

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  MPI_Init(&argc, &argv);

  int rank, size;
  MPI_Status status;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  printf("%d\n", rank);

  MPI_Finalize();
}