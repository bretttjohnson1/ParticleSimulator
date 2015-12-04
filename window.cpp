#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include "window.h"
#include "Cluster.h"
#include "Particle.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>
#include <GL/glut.h>
#include <vector>
#define ESCAPE 27
#define KEY_W 119
#define KEY_A 97
#define KEY_S 115
#define KEY_D 100
using namespace std;

int window;
float offx=1.5f;
float offy=0.0f;
float offz=-6.0f;
bool keys[256];
float phi=0;
float thet=0;
//vector<Particle> display;
Cluster clusters[CLUSTERVAL][CLUSTERVAL][CLUSTERVAL];
Window::Window(Cluster clu[CLUSTERVAL][CLUSTERVAL][CLUSTERVAL]){
  for(int a = 0; a<CLUSTERVAL;a++)
    for(int b = 0;b<CLUSTERVAL;b++)
      for(int c =0;c<CLUSTERVAL;c++)
	clusters[a][b][c]=clu[a][b][c];
}
void Window::init(int width, int height){
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0);
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, (GLfloat)width/(GLfloat)height,0.1f,100.0f);
  glMatrixMode(GL_MODELVIEW);
}

void phys(){
    for(int a = 0;a<CLUSTERVAL;a++){
      for(int b = 0;b<CLUSTERVAL;b++){
        for(int c = 0;c<CLUSTERVAL;c++){
	  for(unsigned long d = 0;d<clusters[a][b][c].particles.size();d++){
	    if(clusters[a][b][c].particles.at(d).x<0
	       || clusters[a][b][c].particles.at(d).x>=10*CLUSTERVAL)
	      clusters[a][b][c].particles.at(d).vx*=-1;
	    if(clusters[a][b][c].particles.at(d).y<0
	       || clusters[a][b][c].particles.at(d).y>=10*CLUSTERVAL)
	      clusters[a][b][c].particles.at(d).vy*=-1;
	    if(clusters[a][b][c].particles.at(d).z<0
	       || clusters[a][b][c].particles.at(d).z>=10*CLUSTERVAL)
	      clusters[a][b][c].particles.at(d).vz*=-1;
	    clusters[a][b][c].particles.at(d).updatelocation();
          }
        }
      }
    }
  
}


void moveandrotate(){
  if(keys[32]){
    offy-=.05;
  }
  if(keys[(int)'c'])
    offy+=.05;
  if(keys[(int)'w']){
    offz+=.05;
    //keys[(int)'w']=false;
  }
  if(keys[(int)'s']){
    offz-=.05;
    //keys[(int)'s']=false;
  }
  if(keys[(int)'d']){
    offx-=.05;
    //keys[(int)'d']=false;
  }
  if(keys[(int)'a']){
    offx+=.05;
    //keys[(int)'a']=false;
  }
  if(keys[(int)'j'])
    thet+=1;
  if(keys[(int)'l'])
    thet+=-1;
  if(keys[(int)'i'])
    phi+=1;
  if(keys[(int)'k'])
    phi-=1;
}

void draw(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  moveandrotate();
  phys();
  glTranslatef(offx,offy,offz);
  glScalef(.25/CLUSTERVAL,.25/CLUSTERVAL,.25/CLUSTERVAL);
  glRotatef(thet,0,1,0);
  // glRotatef(thet,cos(thet)*sin(phi),sin(thet)*sin(phi),cos(phi));
  for(int a = 0; a<CLUSTERVAL;a++)
    for(int b = 0;b<CLUSTERVAL;b++)
      for(int c =0;c<CLUSTERVAL;c++)
	for(unsigned long d = 0;d<clusters[a][b][c].particles.size();d++){
	  float col = abs(pow(clusters[a][b][c].particles.at(d).vx,2)+pow(clusters[a][b][c].particles.at(d).vy,2)+pow(clusters[a][b][c].particles.at(d).vz,2));
	  glColor3f((col)*2,0,2*(.5-col));
	  glBegin(GL_POINTS);
	  glVertex3f(((float)(clusters[a][b][c].particles.at(d).x-50)),
		     ((float)(clusters[a][b][c].particles.at(d).y-50)),
		     ((float)(clusters[a][b][c].particles.at(d).z-50)));
	  glEnd();
  }
   /*glBegin(GL_QUADS);
  glVertex3f(-1,1,0);
  glVertex3f(-1,-1,0);
  glVertex3f(1,-1,0);
  glVertex3f(1,1,0);
  glEnd();*/
  glutSwapBuffers();
}
void resize(int width,int height){
  if(height==0)
    height=1;
  glViewport(0,0,width,height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, (GLfloat)width/(GLfloat)height,0.1f,100.0f);
  glMatrixMode(GL_MODELVIEW);
  
}
void keyPressed(unsigned char key, int x, int y){
  keys[key]=true;
  usleep(100);
  if(key == ESCAPE){
    glutDestroyWindow(window);
    exit(0);
  }

}
void keyUp(unsigned char key, int x, int y){
  keys[key]=false;
}
void Window::start(int argc, char **argv){
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
  glutInitWindowSize(640,480);
  window = glutCreateWindow("penor XDDDD");
  glutDisplayFunc(&draw);
  glutFullScreen();
  glutIdleFunc(&draw);
  glutReshapeFunc(&resize);
  glutKeyboardFunc(&keyPressed);
  glutKeyboardUpFunc(&keyUp);
  init(640,480);
  glutMainLoop();
}
void Window::update(vector<Particle> &p){

}
