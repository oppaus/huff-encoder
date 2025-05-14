CC := clang
CFLAGS :=-c
GTEST_ROOT := /opt/homebrew/Cellar/googletest/1.17.0
GTEST_INC := -I${GTEST_ROOT}/include
GTEST_LIB := -L${GTEST_ROOT}/lib -lgtest

.PHONY: all clean test debug jzip junzip

all: jzip junzip

jzip: main.o tree.o
	${CC} $^ -o $@

junzip: junzip.o tree.o
	${CC} $^

%.o: %.c
	${CC} ${CFLAGS} $< 

debug: CFLAGS +=-g
debug: junzip

test: test_tree
test_tree: test_tree.cpp all
	${CC}++ -std=c++20 ${GTEST_INC} ${GTEST_LIB} tree.o $< -o $@ && ./$@

clean:
	rm -f *.o a.out test_tree jzip junzip
