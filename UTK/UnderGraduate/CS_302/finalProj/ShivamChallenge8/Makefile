
all: test

test: test.o Units.o
	g++ -o test test.o Units.o

test.o: Units.cpp Units.h test.cpp
	g++ -c test.cpp

Units.o: Units.cpp Units.h
	g++ -c Units.cpp

clean:
	rm -f test
