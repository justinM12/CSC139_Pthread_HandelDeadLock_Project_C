
all.o: main.o buffer.o
	gcc main.o buffer.o -lpthread -lrt

buffer.o: buffer.c
	gcc -c buffer.c
main.o: main.c
	gcc -c main.c
