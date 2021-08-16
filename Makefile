all: myshell

myshell: main.c
	gcc main.c -o myshell

run: myshell
	./myshell

debug: main.c
	gcc main.c -g -o myshell
	gdb ./myshell

.PHONY: all run debug
