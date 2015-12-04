#include <iostream>
#include "Particle.h"
using namespace std;


 Particle::Particle(double x, double y, double z, double vx, double vy, double vz){
    this->x=x;
    this->y=y;
    this->z=z;
    this->vx=vx;
    this->vy=vy;
    this->vz=vz;

  }
void Particle::updatelocation(){
    x+=vx;
    y+=vy;
    z+=vz;
  }
void Particle::setvelocity(double vx,double vy,double vz){
    this->vx=vx;
    this->vy=vy;
    this->vz=vz;
  }
void Particle::addforce(double fx, double fy, double fz){
    vx+=fx;
    vy+=fy;
    vz+=fz;
    
  }
  
