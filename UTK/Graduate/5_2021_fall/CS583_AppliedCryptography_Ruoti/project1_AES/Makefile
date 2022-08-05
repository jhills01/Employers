#VERY IMPORTANT!! Lines must be indented with TAB instead of 4 spaces. Otherwise, you'll get "missing separator" error. 
# 		  IF YOU OPENED THIS FILE IN VISUAL STUDIO 2019, DON'T!!! I have TAB set to 4 spaces and don't want to change it. 
# 		  EDIT MAKEFILES IN VIM!!!!

CXX=        g++
CXXFLAGS=   -g
SHELL=      bash
PROGRAMS=   main

all:    main 

main:  main.cpp 1.cpp 2.cpp 3.cpp 4.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(PROGRAMS)
