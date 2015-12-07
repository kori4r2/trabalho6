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
	diff case1.out test1.out
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./program < case2.in > test2.out
	diff case2.out test2.out
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./program < case3.in > test3.out
	diff case3.out test3.out
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./program < case4.in > test4.out
	diff case4.out test4.out
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./program < case5.in > test5.out
	diff case5.out test5.out
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./program < case6.in > test6.out
	diff case6.out test6.out
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./program < case7.in > test7.out
	diff case7.out test7.out
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./program < case8.in > test8.out
	diff case8.out test8.out
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./program < case9.in > test9.out
	diff case9.out test9.out
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./program < case10.in > test10.out
	diff case10.out test10.out
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./program < case11.in > test11.out
	diff case11.out test11.out
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./program < case12.in > test12.out
	diff case12.out test12.out
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./program < case13.in > test13.out
	diff case13.out test13.out
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./program < case14.in > test14.out
	diff case14.out test14.out
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./program < case15.in > test15.out
	diff case15.out test15.out
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./program < case16.in > test16.out
	diff case16.out test16.out
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./program < case17.in > test17.out
	diff case17.out test17.out
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./program < case18.in > test18.out
	diff case18.out test18.out
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./program < case19.in > test19.out
	diff case19.out test19.out
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./program < case20.in > test20.out
	diff case20.out test20.out

