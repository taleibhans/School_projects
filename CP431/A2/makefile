build: pseudorandom.c
	mpicc merge.c -lm -o merge.o

run: build
	mpirun -np 6 ./merge.o 100000
