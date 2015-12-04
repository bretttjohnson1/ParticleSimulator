#ifndef WINDOW_H
#define WINDOW_H
#include <stdio.h>
#include <stdlib.h>
#include "Cluster.h"
#include <vector>
#include "Particle.h"
#define CLUSTERVAL 20

using namespace std;

class Window
{
 private:
  void init(int width, int height);
 public:
  void start(int argc, char **argv);
  void update(vector<Particle> &p);
  Window(Cluster clu[CLUSTERVAL][CLUSTERVAL][CLUSTERVAL]);

};
#endif
