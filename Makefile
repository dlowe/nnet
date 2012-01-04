NAME := nnet
CODE := $(NAME).c
OBJ  := $(CODE:.c=.o)

CC     := gcc
CFLAGS := -ggdb -std=c89 -Wall -Werror -pedantic-errors -O3

.PHONY: all
all: $(NAME)

TNAME   := $(NAME)-test
TCODE   := $(TNAME).c
HACKOBJ := no-main-$(OBJ)
$(TNAME): $(TCODE) $(OBJ)
	ld -r -unexported_symbol _main $(OBJ) -o $(HACKOBJ)
	$(CC) $(CFLAGS) -o $@ $(HACKOBJ) $(TCODE) -lcheck

CODE_SIZE := $(shell cat $(CODE) | wc -c)
RULE_SIZE := $(shell cat $(CODE) | perl -pe 's/[;{}]\s//g' | perl -pe 's/\s//g' | wc -c)

.PHONY: test
test: $(TNAME)
	@./$(TNAME)
	@echo "code size $(CODE_SIZE) / 4096"
	#@test $(CODE_SIZE) -le 4096
	@echo "rule size $(RULE_SIZE) / 2048"
	#@test $(RULE_SIZE) -le 2048
	@echo "testing 'build' script"
	@rm -rf prog.c prog
	@cp $(CODE) prog.c
	@./build
	@test -e prog
	@rm -rf prog.c prog
	@echo "testing README.markdown"
	@Markdown.pl README.markdown >/dev/null

$(NAME): test $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

.PHONY: clean
clean:
	rm -rf $(NAME) $(OBJ)
	rm -rf prog.c prog
	rm -rf $(TNAME) $(HACKOBJ)
