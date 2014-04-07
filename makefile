CC=gcc

image: main.o
	$(CC) -o image main.o -lrt

main.o: main.c
	$(CC) -O2 -c -o main.o main.c

clean:
	find . -type f -name "*.o" | xargs rm -f
	find . -type f -perm /111  | xargs rm -f
