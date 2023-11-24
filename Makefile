FLAGS = -Wall -Werror -Wextra
CC = gcc
CLANG_FILE_PATH = ../materials/linters/.clang-format

all: build_debug

build: clang stack.o polish.o io_module.o
	$(CC) $(FLAGS) polish.o stack.o io_module.o graph.c -o graph
	mv graph ../build/

build_debug: clang polish.o stack.o io_module.o 
	$(CC) -DDEBUG $(FLAGS) polish.o stack.o io_module.o graph.c -o graph
	mv graph ../build/

polish.o: stack.c polish.c
	$(CC) -c $(FLAGS) stack.c polish.c 

stack.o: stack.c
	$(CC) -c $(FLAGS) stack.c

io_module.o: io_module.c
	$(CC) -c $(FLAGS) io_module.c

clang:
	clang-format -style=file:$(CLANG_FILE_PATH) -i *.c
	clang-format -style=file:$(CLANG_FILE_PATH) -i *.h

rebuild: clean build

clean:
	rm -rf *.o