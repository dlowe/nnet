all: nnet

CC     := gcc
CFLAGS := -ggdb -std=c89 -Wall -Werror -pedantic-errors

nnet: nnet.o
	$(CC) -o $@ $<

.PHONY: clean
clean:
	rm -rf nnet nnet.o
