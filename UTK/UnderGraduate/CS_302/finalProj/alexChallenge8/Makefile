CXX=	  g++
CXXFLAGS= -g -Wall -std=gnu++11
LDFLAGS=
SHELL=	  bash
PROGRAMS= solution
SOURCES= solution.cpp map.cpp Unit.cpp
OBJECTS= $(SOURCES:.cpp=.o)

all:	 $(PROGRAMS)

%.o: 	 %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $^

solution: $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJECTS)

clean:
	rm -f $(PROGRAMS) $(OBJECTS)


