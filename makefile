all:run

run: window.o test.o Particle.o Cluster.o
	g++ window.o test.o Particle.o Cluster.o -O2 -g -std=c++0x -lpthread -lmuparser -lGL -lglut -lGLU -o run
	rm window.o test.o Particle.o Cluster.o
test.o: test.cpp window.hpp globals.h
	g++ -c test.cpp -lmuparser
window.o: window.cpp window.hpp
	g++ -c window.cpp -std=c++0x -O2 -g -lpthread -lmuparser -lGL -lglut -lGLU
Particle.o: Particle.cpp Particle.hpp
	g++ -c Particle.cpp
Cluster.o: Cluster.cpp	Particle.hpp
	g++ -c Cluster.cpp
clean:
	rm *o run
