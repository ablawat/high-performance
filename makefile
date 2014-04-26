CC=icc

all:
	make --no-print-directory image-1
	make --no-pront-directory image-2
	make --no-print-directory image-3


image-3: main-3.o calculation-3.o
	$(CC) -O2 -fopenmp -mmic -o image-3 main-3.o calculation-3.o -lrt

main-3.o: main.c
	$(CC) -O2 -fopenmp -mmic -c -o main-3.o main.c

calculation-3.o: calculation-2.c
	$(CC) -O2 -fopenmp -mmic -c -o calculation-3.o calculation-2.c


image-2: main-2.o calculation-2.o
	$(CC) -O2 -fopenmp -o image-2 main-2.o calculation-2.o -lrt

main-2.o: main.c
	$(CC) -O2 -fopenmp -c -o main-2.o main.c

calculation-2.o: calculation-2.c
	$(CC) -O2 -fopenmp -c -o calculation-2.o calculation-2.c


image-1: main-1.o calculation-1.o
	$(CC) -O2 -o image-1 main-1.o calculation-1.o -lrt

main-1.o: main.c
	$(CC) -O2 -c -o main-1.o main.c

calculation-1.o: calculation-1.c
	$(CC) -O2 -c -o calculation-1.o calculation-1.c

clean:
	find . -type f -name "*.o" | xargs rm -f
	find . -type f -perm /111  | xargs rm -f
