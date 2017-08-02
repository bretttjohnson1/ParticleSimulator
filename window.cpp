#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include "Cluster.hpp"
#include "Particle.hpp"
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>
#include <GL/glut.h>
#include <cmath>
#include <muParser.h>
#include <muParserBase.h>
#include <string>
#include <sys/time.h>
#include <vector>
#include <thread>
#include "window.hpp"
#include "globals.h"

#define ESCAPE 27
#define KEY_W 119
#define KEY_A 97
#define KEY_S 115
#define KEY_D 100
using namespace std;
using namespace mu;


int window;
float offx=1.5f;
float offy=0.0f;
float offz=-6.0f;
bool keys[256];
float phi=0;
float thet=0;
string gravityx,gravityy,gravityz;
//vector<Particle> display;
Cluster *clusters;
Window::Window(Cluster *clu,string gravx, string gravy, string gravz){
	clusters=clu;
	gravityx=gravx;
	gravityy=gravy;
	gravityz=gravz;
	cout<<xsize;
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

void phys(string gravityx, string gravityy,string gravityz, int quad, Cluster *c){
	Cluster *clusters = c;
	//determines which quadrant to do math on
	int xbound=0,ybound=0,zbound=0;
	switch(quad) {
	case 0:
		xbound=0; ybound=0; zbound=0;
		break;
	case 1:
		xbound=clustervalx/2; ybound=0; zbound=0;
		break;
	case 2:
		xbound=0; ybound=clustervaly/2; zbound=0;
		break;
	case 3:
		xbound=0; ybound=0; zbound=clustervalz/2;
		break;
	case 4:
		xbound=clustervalx/2; ybound=clustervaly/2; zbound=0;
		break;
	case 5:
		xbound=clustervalx/2; ybound=0; zbound=clustervalz/2;
		break;
	case 6:
		xbound=0; ybound=clustervaly/2; zbound=clustervalz/2;
		break;
	case 7:
		xbound=clustervalx/2; ybound=clustervaly/2; zbound=clustervalz/2;
		break;
	}

	//creates 3 equation parsers for the vectorfield an defines references to x,y,z
	Parser parsex;
	Parser parsey;
	Parser parsez;
	parsex.SetExpr(gravityx);
	parsey.SetExpr(gravityy);
	parsez.SetExpr(gravityz);
	double x=0,y=0,z=0;
	parsex.ParserBase::DefineVar("x",&x);
	parsex.ParserBase::DefineVar("y",&y);
	parsex.ParserBase::DefineVar("z",&z);

	parsey.ParserBase::DefineVar("x",&x);
	parsey.ParserBase::DefineVar("y",&y);
	parsey.ParserBase::DefineVar("z",&z);

	parsez.ParserBase::DefineVar("x",&x);
	parsez.ParserBase::DefineVar("y",&y);
	parsez.ParserBase::DefineVar("z",&z);
	//these values offset the rounding error from divide by two of clusterval
	int roundoffx=clustervalx%2,roundoffy=clustervaly%2,roundoffz=clustervalz%2;
	if(xbound==0) roundoffx=0;
	if(ybound==0) roundoffy=0;
	if(zbound==0) roundoffz=0;

	for(int a = xbound; a<clustervalx/2+xbound+roundoffx; a++) {
		for(int b = ybound; b<clustervaly/2+ybound+roundoffy; b++) {
			for(int c = zbound; c<clustervalz/2+zbound+roundoffz; c++) {
				for(unsigned long d = 0; d<clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.size(); d++) {
					//sets x,y,z for use in vector field. offset because 0,0,0 should be in middle
					x=clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).x-xsize/2;
					y=clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).y-ysize/2;
					z=clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).z-zsize/2;

					double forcex=0,forcey=0,forcez=0;
					for(int i=a-1; i<=a+1; i++)
						for(int j=b-1; j<=b+1; j++)
							for(int k=c-1; k<=c+1; k++)
								if(i>=0 && j>=0 && k>=0 && i<clustervalx && j<clustervaly && k<clustervalz)
									for(unsigned long l=0; l<clusters[i*clustervaly*clustervalz+j*clustervalz+k].particles.size(); l++) {
										double disx=clusters[i*clustervaly*clustervalz+j*clustervalz+k].particles.at(l).x
										             -clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).x;
										double disy=clusters[i*clustervaly*clustervalz+j*clustervalz+k].particles.at(l).
										             y-clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).y;
										double disz=clusters[i*clustervaly*clustervalz+j*clustervalz+k].particles.at(l).z
										             -clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).z;
										if(!(disx==0 && disy==0 && disz==0)) {
											double dist = sqrt(disx*disx+disy*disy+disz*disz)/3;
											double realdist = sqrt(disx*disx+disy*disy+disz*disz);
											if(dist!=0) {
												forcex+=(2/(dist*dist+.02)-1/((dist-2)*(dist-2)+1.5))* -1*disx/realdist;
												forcey+=(2/(dist*dist+.02)-1/((dist-2)*(dist-2)+1.5))* -1*disy/realdist;
												forcez+=(2/(dist*dist+.02)-1/((dist-2)*(dist-2)+1.5))* -1*disz/realdist;
											}
										}

									}

					//cout<<forcex;
					clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).vx+=forcex;
					clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).vy+=forcey;
					clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).vz+=forcez;



					//edge checking if wrapxyz=true, particles loop from one side to another
					//if wrapxyz=false particles bounce off the boundaries
					if(!wrapx) {
						if(clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).x<0)
							clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).vx=
							        abs(clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).vx);
						if(clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).x>=xsize)
							clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).vx=
							        -1*abs(clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).vx);
					}else{
						if(clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).x<0)
							clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).x=xsize;
						if(clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).x>xsize)
							clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).x=0;
					}
					if(!wrapy) {
						if(clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).y<0)
							clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).vy=
							        abs(clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).vy);
						if(clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).y>=ysize)
							clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).vy=
							        -1*abs(clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).vy);
					}else{
						if(clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).y<0)
							clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).y=ysize;
						if(clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).y>ysize)
							clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).y=0;
					}
					if(!wrapz) {
						if(clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).z<0)
							clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).vz=
							        abs(clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).vz);
						if(clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).z>=zsize)
							clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).vz=
							        -1*abs(clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).vz);
					}else{
						if(clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).z<0)
							clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).z=zsize;
						if(clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).z>zsize)
							clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).z=0;
					}
					clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).vx+=parsex.Eval()/100;
					clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).vy+=parsey.Eval()/100;
					clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).vz+=parsez.Eval()/100;
				}
			}
		}
	}
	for(int a = xbound; a<clustervalx/2+xbound+roundoffx; a++) {
		for(int b = ybound; b<clustervaly/2+ybound+roundoffy; b++) {
			for(int c = zbound; c<clustervalz/2+zbound+roundoffz; c++) {
				for(unsigned long d = 0; d<clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.size(); d++) {
					clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).updatelocation();
				}
			}
		}
	}


}
void physics(){
	for(int a = 0; a<clustervalx; a++)
		for(int b = 0; b<clustervaly; b++)
			for(int c =0; c<clustervalz; c++)
				for(unsigned long d = 0; d<clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.size(); d++) {
					int clusterx = clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).x/xsize*clustervalx;
					int clustery = clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).y/ysize*clustervaly;
					int clusterz = clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).z/zsize*clustervalz;
					// cout<<clusterz<<"\n";
					if(clusterx<clustervalx && clustery<clustervaly && clusterz<clustervalz && clusterx>=0 && clustery>=0 && clusterz>=0)
						if(clusterx!=a ||clustery!=b || clusterz!=c) {
							/*double x = clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).x;
							   double y = clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).y;
							   double z = clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).z;
							   double vx = clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).vx;
							   double vy = clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).vy;
							   double vz = clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).vz;*/
							clusters[clusterx*clustervaly*clustervalz+clustery*clustervalz+clusterz].particles.push_back(clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles[d]);
							Particle tmp = clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles[d];
							clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles[d]=clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles[clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.size()-1];
							clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles[clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.size()-1]=tmp;
							clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.pop_back();
							d--;
						}
				}



	thread threads[8];
	for(int a = 0; a<8; a++) {
		threads[a]=thread (phys,gravityx,gravityy,gravityz,a,clusters);
	}
	for(int a = 0; a<8; a++) {
		threads[a].join();
	}
}


void moveandrotate(){
	if(keys[32]) {
		offy-=.05+keys[(int)'m'];
	}
	if(keys[(int)'c'])
		offy+=.05+keys[(int)'m'];
	if(keys[(int)'w']) {
		offz+=.05+keys[(int)'m'];
		//keys[(int)'w']=false;
	}
	if(keys[(int)'s']) {
		offz-=.05+keys[(int)'m'];
		//keys[(int)'s']=false;
	}
	if(keys[(int)'d']) {
		offx-=.05+keys[(int)'m'];
		//keys[(int)'d']=false;
	}
	if(keys[(int)'a']) {
		offx+=.05+keys[(int)'m'];
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
	struct timeval begin,end;
	gettimeofday(&begin, NULL);
	physics();
	gettimeofday(&end,NULL);
	// cout<<end.tv_usec-begin.tv_usec<<"\n";
	glTranslatef(offx,offy,offz);
	glScalef(.01/(pow(xsize*ysize*zsize,1/3)+1),.01/(pow(xsize*ysize*zsize,1/3)+1),.01/(pow(xsize*ysize*zsize,1/3)+1));
	glRotatef(thet,0,1,0);
	// glRotatef(thet,cos(thet)*sin(phi),sin(thet)*sin(phi),cos(phi));

	struct timeval stop, start;
	gettimeofday(&start, NULL);


	gettimeofday(&stop,NULL);
	// cout<<stop.tv_usec-start.tv_usec<<"\n";
	for(int a = 0; a<clustervalx; a++)
		for(int b = 0; b<clustervaly; b++)
			for(int c =0; c<clustervalz; c++)
				for(unsigned long d = 0; d<clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.size(); d++) {
					glColor3f(0,255,0);
					glBegin(GL_POINTS);
					glVertex3f(((float)(clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).x-xsize/2)),
					           ((float)(clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).y-ysize/2)),
					           ((float)(clusters[a*clustervaly*clustervalz+b*clustervalz+c].particles.at(d).z-zsize/2)));
					glEnd();

				}
	gettimeofday(&stop,NULL);
	//cout<<stop.tv_usec-start.tv_usec<<"end \n";
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
	if(key == ESCAPE) {
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
	window = glutCreateWindow("Particle Simulator");
	glutDisplayFunc(&draw);
	glutIdleFunc(&draw);
	glutFullScreen();
	glutReshapeFunc(&resize);
	glutKeyboardFunc(&keyPressed);
	glutKeyboardUpFunc(&keyUp);
	init(640,480);
	glutMainLoop();
}
void Window::update(vector<Particle> &p){

}
