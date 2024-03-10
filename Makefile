all: csnake

csnake: src/main.c
	gcc -o csnake -lncurses src/main.c
