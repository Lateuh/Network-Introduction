CC=gcc
CFLAGS=-Wall
LDFLAGS=
EXEC=recepfich trsf_fich
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)

all: $(EXEC)

trsf_fich: trsf_fich.o
	@$(CC) -o $@ $^ $(LDFLAGS)

recepfich: recep_fich.o
	@$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf $(EXEC)