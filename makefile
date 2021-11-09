all: run test

run: main.c fun.c
	gcc  -o run main.c fun.c
test:test.c fun.c
	gcc -o test test.c  fun.c
clean:
	rm run
	rm test