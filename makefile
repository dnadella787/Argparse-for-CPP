CC=g++
CFLAGS=-std=c++1z

main: main.o arg.o parser.o 
	$(CC) $(CFLAGS) -o argparse main.o arg.o parser.o

main.o: main.cpp arg.h parser.h 
	$(CC) $(CFLAGS) -c main.cpp

arg.o: arg.h 
	$(CC) $(CFLAGS) -c -o arg.o arg.cpp 

parser.o: parser.h arg.h
	$(CC) $(CFLAGS) -c -o parser.o parser.cpp 

clean: 
	rm arg.o parser.o main.o argparse 

