main:main.o game.o
	clang main.o game.o -o main
main.o:main.c
	clang -c -Wall main.c -o main.o
game.o:game.c
	clang -c -Wall game.c -o game.o

.PHONY:clean
clean:
	rm *.o
