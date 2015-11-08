CFLAGS = -g -Wall

all: build

build:
	gcc -o program *.c -I./ $(CFLAGS)

clean:
	rm -f *.o
	rm -f program
	clear

run: build
	./program

.zip:
	zip trabalho6.zip *.c *.h Makefile

debug: build
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./program < case1.in > test1.out
	#diff case1.out test.out
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./program < case2.in > test2.out
	#diff case2.out test.out
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./program < case3.in > test3.out
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./program < case4.in > test4.out

