OBJS=main.o reader.o print.o memory.o selftest.o error.o eval.o

CC=clang

CFLAGS=-g -Wall -Wextra

js: $(OBJS)
	$(CC) -o jscheme $(OBJS)

clean:
	rm $(OBJS)
