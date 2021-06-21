all: main.c
	gcc -Ofast main.c -o main.out
debug: main.c
	gcc -Ofast main.c -o main.out
example: all mandelbrot.bf
	./main.out mandelbrot.bf