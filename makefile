CC=gcc

image: main.o calculation.o
	$(CC) -o image main.o calculation.o -lrt

main.o: main.c
	$(CC) -O2 -Wall -c -o main.o main.c

calculation.o: calculation.c
	$(CC) -O2 -Wall -c -o calculation.o calculation.c

clean:
	find . -type f -name "*.o" | xargs rm -f
	find . -type f -perm /111  | xargs rm -f
