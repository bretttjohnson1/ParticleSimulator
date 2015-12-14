#include <iostream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Particle.h"
#include "Cluster.h"
#include "window.h"
#include <vector>
#include <math.h>
#include <fstream>
#include "globals.h"
using namespace std;
double xsize,ysize,zsize;
int particles;
int clustervalx=10;
int clustervaly=10;
int clustervalz=10;
bool wrapx=false,wrapy=false,wrapz=false;
int main(int args, char **args1){
  string ifieldx="rand";
  string ifieldy="rand";
  string ifieldz="rand";
  string vfieldx="0";
  string vfieldy="0";
  string vfieldz="0";
  xsize=100;
  ysize=100;
  zsize=100;
  particles=64000;
  ifstream config("config.txt");
  string line;
  if(config.good()){
    if(config.is_open()){
      while(getline(config,line)){
	if(line.substr(0,8).compare("IFIELDX:")==0)
	  ifieldx=line.substr(8,line.length());
	if(line.substr(0,8).compare("IFIELDY:")==0)
	  ifieldy=line.substr(8,line.length());
	if(line.substr(0,8).compare("IFIELDZ:")==0)
	  ifieldz=line.substr(8,line.length());
	if(line.substr(0,8).compare("VFIELDX:")==0)
	  vfieldx=line.substr(8,line.length());
	if(line.substr(0,8).compare("VFIELDY:")==0)
	  vfieldy=line.substr(8,line.length());
	if(line.substr(0,8).compare("VFIELDZ:")==0)
	  vfieldz=line.substr(8,line.length());
	if(line.substr(0,6).compare("XSIZE:")==0)
	  xsize=atoi(line.substr(6,line.length()).data());
	if(line.substr(0,6).compare("YSIZE:")==0)
	  ysize=atoi(line.substr(6,line.length()).data());
	if(line.substr(0,6).compare("ZSIZE:")==0)
	  zsize=atoi(line.substr(6,line.length()).data());
	if(line.substr(0,10).compare("PARTICLES:")==0){
	  particles=atoi(line.substr(10,line.length()).data());
	}
	cout<<line<<"\n";
      }
    }
  }
  else{
    free(config);
    ofstream config("config.txt");
    config<<"#IFIELD is the initial vector field, leave as rand for random values or write functions of x and y";
    config<<"#VFIELD is the working vector field, it is the same as IFIELD, but it increments the x,y,z velocities by its function value every tick. basically acceleration field\n";
    config<<"#SIZE is fairly self explanetory, dimensions of the rectangular prism in x,y,z that houses the points\n";
    config<<"#PARTICLES is the number of particles\n";
    config<<"IFIELDX:rand\n";
    config<<"IFIELDY:rand\n";
    config<<"IFIELDY:rand\n";
    config<<"VFIELDX:0\n";
    config<<"VFIELDY:0\n";
    config<<"VFIELDZ:0\n";
    config<<"XSIZE:100\n";
    config<<"YSIZE:100\n";
    config<<"ZSIZE:100\n";
    config<<"PARTICLES:64000\n";
    config<<"END";
  }
  //  cout<<particles<<" "<<xsize<<"\n";
  clustervalx =(int)(pow(particles,1/3.0)+1);
  clustervaly =(int)(pow(particles,1/3.0)+1);
  clustervalz =(int)(pow(particles,1/3.0)+1);
  cout<<xsize/clustervalx;
  Cluster *clusters;
  clusters = new Cluster[clustervalx*clustervaly*clustervalz];
  srand(time(0));
  for(int a = 0; a<clustervalx;a++){
    for(int b = 0;b<clustervaly;b++){
      for(int c =0;c<clustervalz;c++){
	clusters[a*clustervaly*clustervalz+b*clustervalz+c]= Cluster(a*xsize/clustervalx,b*ysize/clustervaly,c*zsize/clustervalz,a*xsize/clustervalx+xsize/clustervalx,b*ysize/clustervaly+ysize/clustervalz,c*zsize/clustervalz+zsize/clustervalz);
	for(int d = 0;d<1;d++){
	  clusters[a*clustervaly*clustervalz+b*clustervalz+c].
	    particles.push_back(Particle(((double)a)*xsize/((double)clustervalx)+((double)rand())/((double)(RAND_MAX))*xsize/((double)clustervalx)
					 ,((double)b)*ysize/((double)clustervaly)+((double)rand())/((double)RAND_MAX)*ysize/((double)clustervaly)
					 ,((double)c)*zsize/((double)clustervalz)+((double)rand())/((double)RAND_MAX)*zsize/((double)clustervalx)
					 ,(rand()/(double)RAND_MAX-.5)
					 ,(rand()/(double)RAND_MAX-.5)
					 ,(rand()/(double)RAND_MAX-.5)));
	}
      }
    }
  }

  Window w (clusters,vfieldx,vfieldy,vfieldz);
  w.start(args, args1);
}
