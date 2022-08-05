CXX=	  g++
CXXFLAGS= -g -Wall -std=gnu++11
LDFLAGS= -lSDL2
SHELL=	  bash
PROGRAMS= main
SOURCES= main.cpp Units/Units.cpp caseyChallenge8/Map.cpp caseyChallenge8/PerlinNoise.cpp Tom/SDL_cbc/cbc_library.cpp
OBJECTS= $(SOURCES:.cpp=.o)

all:	 $(PROGRAMS)

%.o: 	 %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $^

main: $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJECTS)

clean:
	rm -f $(PROGRAMS) $(OBJECTS)


