CXX=		g++
#CXXFLAGS=	-g -std=c++98
CXXFLAGS=	-g -Wall -std=c++98
SHELL=		bash
PROGRAMS=	floyd NegativeGraphDiv1

all:	floyd NegativeGraphDiv1

floyd:	floyd.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

NegativeGraphDiv1:	floyd.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY:		clean

clean:
	rm -f $(PROGRAMS)
