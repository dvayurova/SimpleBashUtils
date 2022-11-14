CC = gcc
CFLAGS = -Wall -Werror -Wextra
all : s21_grep
clean : 
	rm -rf *.o s21_grep
rebuild : clean all

s21_grep.o :
	$(CC) $(CFLAGS) -c s21_grep.c
s21_grep : s21_grep.o
	$(CC) -o s21_grep s21_grep.o
