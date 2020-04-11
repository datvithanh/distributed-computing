gcc-9 -fopenmp $1
./a.out $2
wc -l temp.txt