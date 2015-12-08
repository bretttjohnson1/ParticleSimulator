all:run

run:window.o test.o Particle.o Cluster.o
	g++ window.o test.o Particle.o Cluster.o -lmuparser -lGL -lglut -lGLU -o run
test.o: test.cpp
	g++ -c test.cpp
window.o: window.cpp window.h
	g++ -c window.cpp -lmuparser -lGL -lglut -lGLU
Particle.o: Particle.cpp Particle.h
	g++ -c Particle.cpp
Cluster.o: Cluster.cpp	Particle.h
	g++ -c Cluster.cpp
clean:
	rm *o run
