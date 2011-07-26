//Display string code
#include "string.h"
#include "showString.h"
#include <iostream>

using namespace std;

void showString(struct newworld * string)
{
	glLineWidth(1);
    glPointSize(5);
	
	if (fabs(string->p[0].x > 10))
        {
		printf ("The string somehow escaped way out!\n");
		exit(0);
        }
	
	for(int i =0;i < 12;i++)
	{
	GLUquadricObj *quadratic;
	quadratic=gluNewQuadric();
    // Create Smooth Normals    
	gluQuadricNormals(quadratic, GLU_SMOOTH);	 
		
	// Draw point	
	glBegin   (GL_POINTS); 
	glColor4f (1.0,0,0,0);  
	glVertex3f(string->p[i].x,string->p[i].y,string->p[i].z); 
	glEnd     ();
	
	glTranslated(string->p[i].x,string->p[i].y,string->p[i].z);	
	gluSphere   (quadratic,0.03f,32,32);
	glTranslated(-string->p[i].x,-string->p[i].y,-string->p[i].z);	

    if(i<11)
        {
        glColor4f(0.7,1.0,1.0,0);
        glBegin(GL_LINES);    
        glVertex3f(string->p[i].x,string->p[i].y,string->p[i].z);
        glVertex3f(string->p[i+1].x,string->p[i+1].y,string->p[i+1].z);
        glEnd();	
        }
    }
}


void showCircle()
{
	//Draws ellpise made out of points.
	glTranslatef(0.0f,0.5f,0.0f);
    glColor3f   (1.0 ,   0,   0);
    
    float x = 10,
          y = 10,
          z =  0;
    int   t; 
	
	glBegin(GL_POINTS); 
	
		for(t = 0; t <= 360; t++)	
		{   
            x++;
			y++;
			x = 0.5*sin(t);
			y = 0.5*cos(t);
			glVertex3f(x,y,0);
		}       
		glEnd();
	glTranslatef(0.0f,-0.5f,0.0f);
	
	GLUquadricObj *quadratic;
	quadratic=gluNewQuadric();			
	gluQuadricNormals(quadratic, GLU_SMOOTH);	
}
