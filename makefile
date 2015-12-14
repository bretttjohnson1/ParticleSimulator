all:run

run:window.o test.o Particle.o Cluster.o
	g++ window.o test.o Particle.o  Cluster.o -O2 -g -std=c++0x -lpthread -lmuparser -lGL -lglut -lGLU -o run
test.o: test.cpp window.h globals.h
	g++ -c test.cpp 
window.o: window.cpp window.h
	g++ -c window.cpp -std=c++0x -O2 -g -lpthread -lmuparser -lGL -lglut -lGLU
Particle.o: Particle.cpp Particle.h
	g++ -c Particle.cpp
Cluster.o: Cluster.cpp	Particle.h
	g++ -c Cluster.cpp
clean:
	rm *o run
