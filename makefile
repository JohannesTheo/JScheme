OBJS=main.o

CC=clang

CFLAGS=-g -Wall -Wextra

scheme: $(OBJS)
	$(CC) -o scheme $(OBJS)

clean:
	rm $(OBJS)
