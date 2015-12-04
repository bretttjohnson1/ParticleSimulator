#include <iostream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Particle.h"
#include "Cluster.h"
#include "window.h"
#include <vector>
using namespace std;
int main(int args, char **args1){
  cout<<CLUSTERVAL;
  Cluster clusters[CLUSTERVAL][CLUSTERVAL][CLUSTERVAL];
  srand(time(0));
  for(int a = 0; a<CLUSTERVAL;a++){
    for(int b = 0;b<CLUSTERVAL;b++){
      for(int c =0;c<CLUSTERVAL;c++){
	clusters[a][b][c]= Cluster(a*10,b*10,c*10,a*10+10,b*10+10,c*10+10);
	for(int d = 0;d<10;d++){
	  clusters[a][b][c].particles.push_back(
	    Particle(a*10+rand()%10,b*10+rand()%10,c*10+rand()%10,
		     (rand()/(double)RAND_MAX-.5),rand()/(double)RAND_MAX-.5,rand()/(double)RAND_MAX-.5));
	}
      }
    }
  }
  Window w (clusters);
  w.start(args, args1);
}

