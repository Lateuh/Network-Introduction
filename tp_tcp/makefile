CC=gcc
CFLAGS=-Wall
LDFLAGS=
EXEC=serveur client
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)

all: $(EXEC)

client: client.o
	@$(CC) -o $@ $^ $(LDFLAGS)

serveur: serveur.o
	@$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf $(EXEC)