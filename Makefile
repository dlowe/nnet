NAME := nnet
CODE := $(NAME).c
OBJ  := $(CODE:.c=.o)

CC     := gcc
CFLAGS := -ggdb -std=c89 -Wall -Werror -pedantic-errors

.PHONY: all
all: $(NAME)

CODE_SIZE := $(shell cat $(CODE) | wc -c)
RULE_SIZE := $(shell cat $(CODE) | perl -pe 's/[;{}]\s//g' | perl -pe 's/\s//g' | wc -c)

.PHONY: test
test:
	@test $(CODE_SIZE) -le 4096
	@test $(RULE_SIZE) -le 2048
	@test `cat nnet.c | perl -pe 's/[;{}]\s//g' | perl -pe 's/\s//g' | wc -c` -le 2048

$(NAME): test $(OBJ)
	$(CC) -o $@ $(OBJ)

.PHONY: clean
clean:
	rm -rf $(NAME) $(OBJ)
