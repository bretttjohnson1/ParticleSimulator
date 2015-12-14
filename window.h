#ifndef WINDOW_H
#define WINDOW_H
#include <stdio.h>
#include <stdlib.h>
#include "Cluster.h"
#include <vector>
#include "Particle.h"
#include "globals.h"
#include <string>
//#define CLUSTERVAL 40

using namespace std;
class Window
{
 private:
  void init(int width, int height);
 public:
  void start(int argc, char **argv);
  void update(vector<Particle> &p);
  Window(Cluster *clu, string gravx, string gravy, string gravz);

};
#endif
