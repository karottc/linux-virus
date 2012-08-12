virus:virus.o getRoot.o
	gcc -o virus virus.o getRoot.o
virus.o:virus.c
	gcc -c virus.c
getRoot.o:getRoot.c
	gcc -c getRoot.c

clean:
	rm -rf *.o virus
