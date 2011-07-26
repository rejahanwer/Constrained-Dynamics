//String simulation using Contrained dynamics and Legrange mechanics
//Rejah Anuvar

#ifndef _STR_H_
#define _STR_H_

#ifdef WIN32
  #include <windows.h>
#endif

#include "openGL-headers.h"
#include "pic.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define pi 3.141592653589793238462643383279 

// number of images saved to disk so far
extern int sprite;

// mouse control
extern int g_vMousePos[2];
extern int g_iLeftMouseButton,
           g_iMiddleMouseButton,
           g_iRightMouseButton;

struct point 
{
   double x;
   double y;
   double z;
};

// these variables control what is displayed on the screen
extern int   toggle , damping    , 
             dopause, viewingMode,
             saveScreenToFile    ;
extern double Fx, Fy;
struct newworld
{
	int    N;
	int    num;
 	double dt; // timestep, e.g.. 0.001
	double mass; // mass of each of the 512 control points, mass assumed to be equal for every control point
	double edgeLength; //edge length
	struct point p[12]; // position of the 11 control points
	struct point v[12]; // velocities of the 11 control points
};

extern struct newworld string;

// computes crossproduct of two vectors
#define CROSSPRODUCTp(vector1,vector2,dest)           \
  CROSSPRODUCT( (vector1).x, (vector1).y, (vector1).z,\
                (vector2).x, (vector2).y, (vector2).z,\
                (dest).x, (dest).y, (dest).z )

// computes crossproduct of two vectors,
// which are specified by floating-point coordinates
#define CROSSPRODUCT(x1,y1,z1,x2,y2,z2,x,y,z)\
                                             \
  x = (y1) * (z2) - (y2) * (z1);             \
  y = (x2) * (z1) - (x1) * (z2);             \
  z = (x1) * (y2) - (x2) * (y1)

// normalizes vector dest
// must declare a double variable called 'length' somewhere inside the scope of the NORMALIZE macrp
// macro will change that variable
#define pNORMALIZE(dest)\
                        \
  length = sqrt((dest).x * (dest).x + (dest).y * (dest).y + (dest).z * (dest).z);\
  (dest).x /= length;   \
  (dest).y /= length;   \
  (dest).z /= length;

// copies vector source to vector dest
// struct point source,dest
#define pCPY(source,dest)\
                         \
  (dest).x = (source).x; \
  (dest).y = (source).y; \
  (dest).z = (source).z;
  
// assigns values x,y,z to point vector dest
// struct point dest
#define pMAKE(x,y,z,dest)\
                         \
  (dest).(x) = (x);      \
  (dest).(y) = (y);      \
  (dest).(z) = (z);

// sums points src1 and src2 to dest
// struct point src1,src2,dest
#define pSUM(src1,src2,dest)     \
                                 \
  (dest).x = (src1).x + (src2).x;\
  (dest).y = (src1).y + (src2).y;\
  (dest).z = (src1).z + (src2).z;

// dest = src2 - src1
// struct point src1,src2,dest
#define pDIFFERENCE(src1,src2,dest)\
                                   \
  (dest).x = (src1).x - (src2).x;  \
  (dest).y = (src1).y - (src2).y;  \
  (dest).z = (src1).z - (src2).z;

// mulitplies components of point src by scalar and returns the result in dest
// struct point src,dest
// double scalar
#define pMULTIPLY(src,scalar,dest)\
                                  \
  (dest).x = (src).x * (scalar);  \
  (dest).y = (src).y * (scalar);  \
  (dest).z = (src).z * (scalar);  

#endif

