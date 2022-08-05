all: test

test: test.o PerlinNoise.o Map.o
	g++ -o test test.o PerlinNoise.o Map.o
test.o: Map.cpp Map.h
	g++ -c -std=c++11 test.cpp
Map.o: Map.cpp Map.h PerlinNoise.cpp PerlinNoise.h
	g++ -c -std=c++11 Map.cpp
PerlinNoise.o: PerlinNoise.cpp PerlinNoise.h
	g++ -c -std=c++11 PerlinNoise.cpp

clean:
	rm -f test
