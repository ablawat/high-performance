CC=gcc

image: main.o
	$(CC) -o image main.o

main.o: main.c
	$(CC) -c -o main.o main.c

clean:
	find . -type f -name "*.o" | xargs rm -f
	find . -type f -perm /111  | xargs rm -f
