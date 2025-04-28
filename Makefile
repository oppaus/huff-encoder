CC := clang

all:
	${CC} -o compress main.c

debug:
	${CC} -g -o compress main.c
