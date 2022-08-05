CXX=		g++
CXXFLAGS=	-g -Wall -std=gnu++11
SHELL=		bash
PROGRAMS=	dijkstras generate_map

all:		dijkstras generate_map

generate_map:	generate_map.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

dijkstras:	dijkstras.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY:		test test-output test-memory clean

clean:
	rm -f $(PROGRAMS)

test:		test-output test-memory

test-output:	dijkstras
	@echo Testing output 0...
	@diff --suppress-common-lines -y <(./dijkstras < data/input0.txt) data/output0.txt
	@echo Testing output 1...
	@diff --suppress-common-lines -y <(./dijkstras < data/input1.txt) data/output1.txt
	@echo Testing output 2...
	@diff --suppress-common-lines -y <(./dijkstras < data/input2.txt) data/output2.txt
	@echo Testing output 3...
	@diff --suppress-common-lines -y <(./dijkstras < data/input3.txt | head -n 5) <(head -n 5 data/output3.txt)
	@diff --suppress-common-lines -y <(./dijkstras < data/input3.txt | tail -n 5) <(tail -n 5 data/output3.txt)

test-memory:	dijkstras
	@echo Testing memory...
	@[ `valgrind --leak-check=full ./dijkstras < data/input1.txt 2>&1 | grep ERROR | awk '{print $$4}'` = 0 ]
