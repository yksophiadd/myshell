all: myshell

myshell: main.c
	gcc main.c -o myshell -Wall

run: myshell
	./myshell

debug: main.c
	gcc main.c -g -o myshell -Wall
	gdb ./myshell

.PHONY: all run debug
