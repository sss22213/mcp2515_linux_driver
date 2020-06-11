CC=gcc

all: src/mcp2515.h src/mcp2515.c src/main.c
	${CC} src/mcp2515.h src/mcp2515.c src/main.c -o build/main -Wall -g

clear:
	rm build/main

excute:
	./build/main