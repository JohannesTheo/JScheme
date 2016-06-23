OBJS=main.o reader.o print.o memory.o selftest.o eval.o error.o

CC=clang

CFLAGS=-g -Wall -Wextra

js: $(OBJS)
	$(CC) -o jscheme $(OBJS)

clean:
	rm $(OBJS)
