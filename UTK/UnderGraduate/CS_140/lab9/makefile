ALL = enum ss_solver

all: $(ALL)

clean:
	rm -f *.o $(ALL)

.SUFFIXES: .cpp .o

.cpp.o:
	g++ -c $*.cpp

enum.o: 
enum: enum.o
	g++ -o enum enum.o 

ss_solver.o:
ss_solver: ss_solver.o
	g++ -o ss_solver ss_solver.o
