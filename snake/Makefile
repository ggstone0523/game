all: build test clean

build:
	sudo apt-get install libncurses-dev
	gcc snake.c -lncurses

test:
	./a.out

clean:
	rm -rf ./a.out
