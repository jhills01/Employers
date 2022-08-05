CXX=		g++
#CXXFLAGS=	-g -std=c++98
CXXFLAGS=	-g -Wall -std=c++98
SHELL=		bash
PROGRAMS=	jgraph

all:	jgraph

jgraph:	jgraph.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY:		clean

clean:
	rm -f $(PROGRAMS)
