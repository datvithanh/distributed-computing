#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define RANGE 20

int rank, size;

void printArray(int *data, int length);
void testBroadcast();
void testScatterGather();
void testAllGather();
void testAllToAll();
void testReduce(int allReduce);
void testScan();
void testReduceScatter();

int main(int argc, char **argv)
{
  MPI_Init(&argc, &argv);

  MPI_Comm comm;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // TODO: tidy up this topology initialization
  int topology = 1;
  int *dim, *period, reorder = 1;

  if (topology == 1)
    printf("Linear topology\n");
  else {
    if (topology == 2){
      if(sqrt(size)*sqrt(size) != size){
        if(rank == 0)
          printf("sorry your size is not suitable for mesh topology (rn only support square number)\n");

        MPI_Finalize();
        exit(0);
      }
      dim = (int *) malloc(2*sizeof(int));
      period = (int *) malloc(2*sizeof(int));

      dim[0] = sqrt(size); dim[1] = sqrt(size);
      period[0] = 1; period[1] = 1;

      printf("Mesh topology\n");
    }
    else{
      if(cbrt(size)*cbrt(size) != size){
        if(rank == 0)
          printf("sorry your size is not suitable for cubed topology (rn only support cubed number)\n");

        MPI_Finalize();
        exit(0);
      }

      dim = (int *) malloc(3*sizeof(int));
      period = (int *) malloc(3*sizeof(int));

      dim[0] = cbrt(size); dim[1] = cbrt(size); dim[2] = cbrt(size); 
      period[0] = 1; period[1] = 1; period[2] = 1;
      
      printf("Cube topology\n");
    }
  }

  // Create new comm topology
  if (topology > 1) 
    MPI_Cart_create(MPI_COMM_WORLD, 2, dim, period, reorder, &comm);

  if (rank == 0)
    printf("Every op = MPI_SUM\n");

  testBroadcast();
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == 0) printf("\n");

  testScatterGather();
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == 0) printf("\n");

  testAllGather();
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == 0) printf("\n");

  testAllToAll();
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == 0) printf("\n");

  //reduce
  testReduce(0);
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == 0) printf("\n");

  //all reduce
  testReduce(1);
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == 0) printf("\n");

  testScan();
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == 0)
    printf("\n");

  testReduceScatter();

  MPI_Finalize();
  return 0;
}

void printArray(int *data, int length)
{
  for (int i = 0; i < length; i++)
    printf(" %d", *(data + i));
  printf("\n");
}

void testBroadcast()
{
  if (rank == 0)
    printf("Test bcast\n");

  int *buf;
  buf = (int *)malloc(sizeof(int));
  *buf = 0;

  if (rank == 0)
    *buf = 777;

  printf("Process %d: Before Bcast, buf is %d\n", rank, *buf);

  MPI_Bcast(buf, 1, MPI_INT, 0, MPI_COMM_WORLD);

  printf("Process %d: After Bcast, buf is %d\n", rank, *buf);
}

void testScatterGather()
{
  if (rank == 0)
    printf("Test Scatter\n");

  srand(time(0));
  int *globalData = NULL, *localData, temp;
  localData = (int *)malloc(sizeof(int));

  if (rank == 0)
  {
    globalData = malloc(size * sizeof(int));
    printf("Root has data:");

    for (int i = 0; i < size; i++)
    {
      temp = rand() % RANGE;
      globalData[i] = temp;
      printf(" %d", temp);
    }
    printf("\n");
  }

  MPI_Scatter(globalData, 1, MPI_INT, localData, 1, MPI_INT, 0, MPI_COMM_WORLD);
  printf("Processor %d has data %d\n", rank, *localData);
  MPI_Barrier(MPI_COMM_WORLD);

  if (rank == 0)
    printf("Test Gather\n");
  *localData *= 2;
  printf("Processor %d doubling the data, now has %d\n", rank, *localData);

  MPI_Gather(localData, 1, MPI_INT, globalData, 1, MPI_INT, 0, MPI_COMM_WORLD);

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

void testAllGather()
{
  if (rank == 0)
    printf("Test Allgather\n");
  int *globalData = NULL;
  globalData = malloc(sizeof(int));

  for (int i = 0; i < size; i++)
    globalData[i] = 0;

  *globalData = rank + 1;

  printf("Before all gather P %d:", rank);
  printArray(globalData, size);

  MPI_Allgather(globalData, 1, MPI_INT, globalData, 1, MPI_INT, MPI_COMM_WORLD);

  printf("After all gather P %d:", rank);
  printArray(globalData, size);

  free(globalData);
}

void testAllToAll()
{
  if (rank == 0)
    printf("Test Alltoall\n");
  int *globalData = NULL;
  globalData = malloc(size * sizeof(int));

  for (int i = 0; i < size; i++)
    globalData[i] = i + rank * size;

  printf("Before all to all P %d:", rank);
  printArray(globalData, size);

  MPI_Alltoall(globalData, 1, MPI_INT, globalData, 1, MPI_INT, MPI_COMM_WORLD);

  printf("After all to all P %d:", rank);
  printArray(globalData, size);

  free(globalData);
}

void testReduce(int allReduce)
{
  if (rank == 0 && allReduce == 1)
    printf("Test all reduce\n");
  if (rank == 0 && allReduce == 0)
    printf("Test reduce\n");

  int *scalar = NULL, *result = NULL;
  scalar = (int *)malloc(sizeof(int));
  result = (int *)malloc(sizeof(int));

  *scalar = rank + 1;

  printf("Data at Process %d: %d\n", rank, *scalar);

  if (allReduce)
    MPI_Allreduce(scalar, result, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  else
    MPI_Reduce(scalar, result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  MPI_Barrier(MPI_COMM_WORLD);
  
  printf("Data at process %d after reduce: %d\n", rank, *result);

  free(scalar);
}

void testScan()
{
  if (rank == 0)
    printf("Test scan\n");

  int *scalar = NULL;
  scalar = (int *)malloc(sizeof(int));

  *scalar = rank + 1;

  printf("Data at Process %d: %d\n", rank, *scalar);

  MPI_Scan(scalar, scalar, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

  MPI_Barrier(MPI_COMM_WORLD);
  printf("Data at process %d after scan: %d\n", rank, *scalar);

  free(scalar);
}

void testReduceScatter()
{
  if (rank == 0)
    printf("Test Reduce_scatter\n");

  MPI_Barrier(MPI_COMM_WORLD);

  int *globalData = NULL, *result = NULL, *rcounts = NULL;
  globalData = malloc(size * sizeof(int));
  rcounts = malloc(size * sizeof(int));
  result = malloc(sizeof(int));

  for (int i = 0; i < size; i++){
    globalData[i] = i + rank * size;
    rcounts[i] = 1;
  }

  printf("Before reduce scatter P %d:", rank);
  printArray(globalData, size);

  MPI_Reduce_scatter(globalData, result, rcounts, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

  MPI_Barrier(MPI_COMM_WORLD);

  printf("After reduce scatter P %d: %d\n", rank, *result);

  free(globalData);
  free(result);
  free(rcounts); 
}