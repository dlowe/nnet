NAME := nnet
CODE := $(NAME).c
OBJ  := $(CODE:.c=.o)

CC     := gcc
CFLAGS := -ggdb -std=c89 -Wall -Werror -pedantic-errors -O3

.PHONY: all
all: test

TNAME   := $(NAME)-test
TCODE   := $(TNAME).c
HACKOBJ := no-main-$(OBJ)
$(TNAME): $(TCODE) $(OBJ)
	ld -r -unexported_symbol _main $(OBJ) -o $(HACKOBJ)
	$(CC) $(CFLAGS) -o $@ $(HACKOBJ) $(TCODE) -lcheck

CODE_SIZE := $(shell cat $(CODE) | wc -c)
RULE_SIZE := $(shell cat $(CODE) | perl -pe 's/[;{}]\s//g' | perl -pe 's/\s//g' | wc -c)

.PHONY: static-test
static-test: $(TNAME)
	@./$(TNAME)
	@echo "code size $(CODE_SIZE) / 4096"
	@test $(CODE_SIZE) -le 4096
	@echo "rule size $(RULE_SIZE) / 2048"
	@test $(RULE_SIZE) -le 2048
	@echo "testing 'build' script"
	@rm -rf prog.c prog
	@cp $(CODE) prog.c
	@./build
	@test -e prog
	@rm -rf prog.c prog
	@echo "testing README.markdown"
	@Markdown.pl README.markdown >/dev/null

.PHONY: test
test: $(NAME)
	@echo "testing training from scratch"
	./$(NAME) -3 ./training/ioccc-1/ ./training/ioccc-0/ < /dev/null > test.brain0 2>test.out
	@test `cat test.out | wc -l` -eq 3
	@test `grep '^[0-2]: [0-9.]\+$$' test.out | wc -l` -eq 3
	@echo "testing incremental training"
	./$(NAME) -3 ./training/ioccc-1/ ./training/ioccc-0/ < test.brain0 > test.brain 2>test.out
	@test `cat test.out | wc -l` -eq 3
	@test `grep '^[0-2]: [0-9.]\+$$' test.out | wc -l` -eq 3
	@echo "testing classification"
	./$(NAME) $(CODE) < test.brain 2>test.out
	@test `cat test.out | wc -l` -eq 1
	@grep -q '^nnet.c 0\.[0-9]\+$$' test.out
	@rm -f test.brain test.brain0 test.out

.PHONY: test_png
test_png: $(NAME)
	./$(NAME) -18000 ./training/png-1/ ./training/png-0/ < /dev/null > png.brain
	./$(NAME) `find $(HOME) -name "*.png" -maxdepth 5` < png.brain
	./$(NAME) `find $(HOME) -name "*.gif" -maxdepth 5` < png.brain

.PHONY: test_ioccc
test_ioccc: $(NAME)
	./$(NAME) -1000 ./training/ioccc-1/ ./training/ioccc-0/ < /dev/null > ioccc.brain
	./$(NAME) $(CODE) < ioccc.brain

.PHONY: test_english
test_english: $(NAME)
	./$(NAME) -2000 ./training/english-1/ ./training/english-0/ < /dev/null > english.brain
	./$(NAME) ./test/english-1/* ./test/english-0/* < english.brain

.PHONY: test_xor
test_xor:
	./$(NAME) -8000 ./training/xor-1/ ./training/xor-0/ < /dev/null > xor.brain
	./$(NAME) ./training/xor-*/* < xor.brain

$(NAME): static-test $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) -lm

.PHONY: clean
clean:
	rm -rf $(NAME) $(OBJ)
	rm -rf prog.c prog
	rm -rf $(TNAME) $(HACKOBJ)
	rm -rf png.brain ioccc.brain english.brain xor.brain
