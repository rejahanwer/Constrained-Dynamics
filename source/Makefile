# String Simulation Makefile 
# Rejah Anuvar, USC

LIBRARIES = -framework OpenGL -framework GLUT -lgsl -lgslcblas -lm

COMPILER = g++
COMPILERFLAGS = -O2

all: string

string: string.o showString.o input.o physics.o ppm.o pic.o
	$(COMPILER) $(COMPILERFLAGS) -o $@ $^ $(LIBRARIES)

string.o: string.cpp *.h
	$(COMPILER) -c $(COMPILERFLAGS) string.cpp
input.o: input.cpp *.h
	$(COMPILER) -c $(COMPILERFLAGS) input.cpp
showString.o: showString.cpp *.h
	$(COMPILER) -c $(COMPILERFLAGS) showString.cpp
physics.o: physics.cpp *.h
	$(COMPILER) -c $(COMPILERFLAGS) physics.cpp

clean:
	-rm -rf core *.o *~ "#"*"#" test


