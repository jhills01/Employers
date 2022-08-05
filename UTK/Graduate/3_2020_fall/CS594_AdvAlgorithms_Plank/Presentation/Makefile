CXX=		g++
CXXFLAGS=	-g
SHELL=		bash
PROGRAMS=	fr

all:	fr

fr:	friendlyRobot.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY:		clean

clean:
	rm -f $(PROGRAMS)
