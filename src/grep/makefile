all: s21_grep

s21_grep:
	gcc -std=c11 -pedantic -D_GNU_SOURCE -Wall -Werror -Wextra  s21_grep.c s21_grep.h -o s21_grep
clean: 
	rm s21_grep
rebuild:
	make clean
	make all