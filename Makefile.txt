EXEC = tarefa

CC = gcc
CFLAGS = -Wall -Wextra -O2

SRC = main.c funcoes.c
OBJ = $(SRC:.c=.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(EXEC)

clean-obj:
	rm -f *.o

.PHONY: all clean clean-obj