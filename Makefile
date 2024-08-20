all: main

main: main.c
	gcc -o main main.c -lraylib -O2
