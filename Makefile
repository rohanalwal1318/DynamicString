all: demo test

demo: src/main.c src/dstring.c include/dstring.h
	gcc -g -Iinclude src/main.c src/dstring.c -o demo

test: tests/test_dstring.c src/dstring.c include/dstring.h
	gcc -g -Iinclude tests/test_dstring.c src/dstring.c -o test_dstring