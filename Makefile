all: test
	
test: test.o des.o
	gcc test.o des.o -g -o test
run: test
	./test
	
test.o: test.c
	gcc -c test.c -o test.o -g -Wall
	
des.o: des.c
	gcc -c des.c -o des.o -g -Wall -Wno-unused-value
	
clean:
	rm -f *.o
	