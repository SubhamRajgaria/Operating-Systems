a.out: main.o libass2_15CS10061.a
	c++ main.o -L. -lass2_15CS10061

main.o: main.c myl.h
	c++ -Wall -c main.c

libtestCase1.a: ass2_15CS10061.o
	ar -rcs libtestCase1.a libtestCase1.o

ass2_15CS10061.o: testCase1.cpp myl.h
	g++ -Wall -c testCase1.cpp

clean:
	rm a.out main.o libass2_15CS10061.a ass2_15CS10061.o
