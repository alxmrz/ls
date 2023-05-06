SOURCES = main.c \
src/arg_parser.c \
src/dir_reader.c

run:
	./ls -la
all: build
build:
	gcc -g -I./inc -L./src $(SOURCES) -o ls
clean:
	rm ./ls