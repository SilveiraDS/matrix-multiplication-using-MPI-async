all:
	mpicc desafio3.c cmp134.c -o main

scorep:
	scorep --nopomp --nocompiler --nocuda --noonline-access --nopdt --nouser  --noopencl mpicc desafio3.c cmp134.c -o main

lab:
	mpicc.mpich2 desafio3.c cmp134.c -o main

labs:
	scorep --nopomp --nocompiler --nocuda --noonline-access --nopdt --nouser  --noopencl mpicc.mpich2 desafio3.c cmp134.c main
	
clean:
	rm *~
