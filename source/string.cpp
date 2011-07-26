#include "performanceCounter.h"
#include "string.h"
#include "showString.h"
#include "input.h"
#include "physics.h"
#include<iostream>

using namespace std;
PerformanceCounter counter;

// mouse control.
int g_iMenuId           ;
int g_vMousePos[2]      ;
int g_iLeftMouseButton  ,
    g_iMiddleMouseButton,
    g_iRightMouseButton ;

// number of images saved to disk so far
int sprite=0;

// these variables control what is displayed on screen
int toggle       = 1,
    damping      = 1,
    dopause      = 0,
viewingMode      = 0,
saveScreenToFile = 0;

double Fx = 0;
double Fy = 0;

struct newworld     tstring     ;
int    windowWidth, windowHeight;

void myinit()
{
  glMatrixMode   (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective (90.0, 1.0, 0.01, 1000.0);

  // set background color
  glClearColor  (0.4, 0.3, 0.5, 0.0);

  glCullFace    (GL_BACK);
  glEnable      (GL_CULL_FACE);

  glShadeModel  (GL_SMOOTH);
  glEnable      (GL_POLYGON_SMOOTH);
  glEnable      (GL_LINE_SMOOTH);

  return; 
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  //camera   
  gluLookAt(0.0,  0.5, -1.0,
            0.0,  0.5,  3.0,
            0.0, -1.0,  0.0);

  
  // global ambient light
  GLfloat aGa[] = { 0.0, 0.0, 0.0, 0.0 };
  
  // light 's ambient, diffuse, specular
  GLfloat lKa0[] = { 0.0, 0.0, 0.0, 1.0 };
  GLfloat lKd0[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat lKs0[] = { 1.0, 1.0, 1.0, 1.0 };

  GLfloat lKa1[] = { 0.0, 0.0, 0.0, 1.0 };
  GLfloat lKd1[] = { 1.0, 0.0, 0.0, 1.0 };
  GLfloat lKs1[] = { 1.0, 0.0, 0.0, 1.0 };

  GLfloat lKa2[] = { 0.0, 0.0, 0.0, 1.0 };
  GLfloat lKd2[] = { 1.0, 1.0, 0.0, 1.0 };
  GLfloat lKs2[] = { 1.0, 1.0, 0.0, 1.0 };

  GLfloat lKa3[] = { 0.0, 0.0, 0.0, 1.0 };
  GLfloat lKd3[] = { 0.0, 1.0, 1.0, 1.0 };
  GLfloat lKs3[] = { 0.0, 1.0, 1.0, 1.0 };

  GLfloat lKa4[] = { 0.0, 0.0, 0.0, 1.0 };
  GLfloat lKd4[] = { 0.0, 0.0, 1.0, 1.0 };
  GLfloat lKs4[] = { 0.0, 0.0, 1.0, 1.0 };

  GLfloat lKa5[] = { 0.0, 0.0, 0.0, 1.0 };
  GLfloat lKd5[] = { 1.0, 0.0, 1.0, 1.0 };
  GLfloat lKs5[] = { 1.0, 0.0, 1.0, 1.0 };

  GLfloat lKa6[] = { 0.0, 0.0, 0.0, 1.0 };
  GLfloat lKd6[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat lKs6[] = { 1.0, 1.0, 1.0, 1.0 };

  GLfloat lKa7[] = { 0.0, 0.0, 0.0, 1.0 };
  GLfloat lKd7[] = { 0.0, 1.0, 1.0, 1.0 };
  GLfloat lKs7[] = { 0.0, 1.0, 1.0, 1.0 };

  // light positions and directions
  GLfloat lP0[] = { -1.999, -1.999, -1.999, 1.0 };
  GLfloat lP1[] = {  1.999, -1.999, -1.999, 1.0 };
  GLfloat lP2[] = {  1.999,  1.999, -1.999, 1.0 };
  GLfloat lP3[] = { -1.999,  1.999, -1.999, 1.0 };
  GLfloat lP4[] = { -1.999, -1.999,  1.999, 1.0 };
  GLfloat lP5[] = {  1.999, -1.999,  1.999, 1.0 };
  GLfloat lP6[] = {  1.999,  1.999,  1.999, 1.0 };
  GLfloat lP7[] = { -1.999,  1.999,  1.999, 1.0 };
  
  // material color

  GLfloat mKa[] = { 0.0, 0.0, 0.0, 1.0 };
  GLfloat mKd[] = { 0.3, 0.3, 0.3, 1.0 };
  GLfloat mKs[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat mKe[] = { 0.0, 0.0, 0.0, 1.0 };

  /* set up lighting */
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT,          aGa);
  glLightModelf (GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
  glLightModelf (GL_LIGHT_MODEL_TWO_SIDE,    GL_FALSE);

  // set up bead colors
  glMaterialfv(GL_FRONT, GL_AMBIENT,  mKa);
  glMaterialfv(GL_FRONT, GL_DIFFUSE,  mKd);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mKs);
  glMaterialfv(GL_FRONT, GL_EMISSION, mKe);
  glMaterialf (GL_FRONT, GL_SHININESS, 120);
    
  // macro to set up light i
  #define LIGHTSETUP(i)\
  glLightfv(GL_LIGHT##i, GL_POSITION, lP##i);\
  glLightfv(GL_LIGHT##i, GL_AMBIENT, lKa##i);\
  glLightfv(GL_LIGHT##i, GL_DIFFUSE, lKd##i);\
  glLightfv(GL_LIGHT##i, GL_SPECULAR, lKs##i);\
  glEnable(GL_LIGHT##i)
  
  LIGHTSETUP (0);
  LIGHTSETUP (1);
  LIGHTSETUP (2);
  LIGHTSETUP (3);
  LIGHTSETUP (4);
  LIGHTSETUP (5);
  LIGHTSETUP (6);
  LIGHTSETUP (7);

  // enable lighting
  glEnable(GL_LIGHTING);    
  glEnable(GL_DEPTH_TEST);

  //Draw the string and the Circle	
  showString(&tstring);	
  showCircle();	

  glDisable(GL_LIGHTING);
  glutSwapBuffers();
}

void doIdle()
{
  counter.StopCounter();
  //Use to check framerates  
  //cout<<"FPS:" << 1/counter.GetElapsedTime() <<endl;
  counter.StartCounter();	
	
  char s[20]="picxxxx.ppm";
  int i;
  
  // save screen to file
  s[3] = 48 + (sprite / 1000);
  s[4] = 48 + (sprite % 1000) / 100;
  s[5] = 48 + (sprite % 100 ) / 10;
  s[6] = 48 +  sprite % 10;

  if (saveScreenToFile==1)
  {
    saveScreenshot(windowWidth, windowHeight, s);
    // save only once, change this if we want continuos image generation (i.e. animation)
    // saveScreenToFile=0; 
    sprite++;
  }

  if (sprite >= 300) // allow only 300 snapshots
  {
    exit(0);	
  }

  if (!dopause)
  {
      {
	  if(toggle)
		  Euler(&tstring);
	  else
          RK4(&tstring);
	  }
  }

  glutPostRedisplay();
}

int main (int argc, char ** argv)
{
  initString(&tstring);		
  glutInit  (&argc,argv);
  
  // double buffered window, use depth testing, 640x480
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  
  windowWidth  = 640;
  windowHeight = 480;
  glutInitWindowSize (windowWidth, windowHeight);
  glutInitWindowPosition (500,0);
  glutCreateWindow ("String simulation");

  // tells glut to use a particular display function to redraw
  glutDisplayFunc(display);

  // replace with any animate code
  glutIdleFunc(doIdle);

  // callback for mouse drags
  glutMotionFunc(mouseMotionDrag);

  // callback for mouse movement
  glutPassiveMotionFunc(mouseMotion);

  // callback for mouse button changes
  glutMouseFunc(mouseButton);

  // register for keyboard events
  glutKeyboardFunc(keyboardFunc);
  glutSpecialFunc(specialKey);    	

  // do initialization
  myinit();

  // loop
  glutMainLoop();

  return(0);
}

