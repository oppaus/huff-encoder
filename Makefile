CC := clang

all:
	${CC} main.c

debug:
	${CC} -g main.c
