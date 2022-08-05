CXX=		g++
#CXXFLAGS=	-g -Wall -std=c++98
#CXXFLAGS=	-g -Wall
CXXFLAGS=	-g -std=c++11# "g" option is for "default debug information" for gcc compiler
SHELL=		bash
PROGRAMS=	pa2

all:	pa2

pa2:	main.cpp pa2.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

# t32T:	trace32Transformer.cpp
# 	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY:		clean

clean:
	rm -f $(PROGRAMS)
