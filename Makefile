CC = gcc
CFLAGS = -std=c99 
FICHIERS_C = clientAPI.c snakeAPI.c fonction.c main.c 
FICHIERS_O = $(FICHIERS_C: .c=.o)
EXEC = game 

all: $(EXEC)

$(EXEC): $(FICHIERS_O)
	$(CC) -o $@ $^

%.o: %.c 
	$(CC) $(CLFAGS) -c -o $@ $<

clean:
	rm -f $(EXEC) *.o 

.PHONY: all clean 