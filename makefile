OBJS=main.o reader.o print.o memory.o selftest.o error.o eval.o symboltable.o environment.o builtinFunctions.o debugtracer.o cpEval.o

CC=clang

CFLAGS=-g -Wall -Wextra -std=c99

js: $(OBJS)
	$(CC) -o jscheme $(OBJS)

clean:
	rm $(OBJS)
