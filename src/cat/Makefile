CC = gcc
CFLAGS = -Wall -Werror -Wextra
all : s21_cat
clean : 
	rm -rf *.o s21_cat
rebuild : clean all

s21_cat.o :
	$(CC) $(CFLAGS) -c s21_cat.c
s21_cat : s21_cat.o
	$(CC) -o s21_cat s21_cat.o