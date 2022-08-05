CXX=		g++
#CXXFLAGS=	-g -Wall -std=c++98
#CXXFLAGS=	-g -Wall
CXXFLAGS=	-g -std=c++11# "g" option is for "default debug information" for gcc compiler
SHELL=		bash
PROGRAMS=	pa1 t32T

all:	pa1 t32T

pa1:	pa1.cpp main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

t32T:	trace32Transformer.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY:		clean

clean:
	rm -f $(PROGRAMS)
