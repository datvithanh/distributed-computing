mpicc $1
mpirun --oversubscribe -x LD_PRELOAD=libmpi.so -np $2 a.out 