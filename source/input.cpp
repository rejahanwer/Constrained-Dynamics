#include "string.h"
#include "input.h"
#include <iostream>

using namespace std;


//mouseDrag not used
void mouseMotionDrag(int x, int y)
{ 
}

void mouseMotion (int x, int y)
{
  g_vMousePos[0] = x;
  g_vMousePos[1] = y;
}

void mouseButton(int button, int state, int x, int y)
{
  switch (button)
  {
    case GLUT_LEFT_BUTTON:
      g_iLeftMouseButton = (state==GLUT_DOWN);
      break;
    case GLUT_MIDDLE_BUTTON:
      g_iMiddleMouseButton = (state==GLUT_DOWN);
      break;
    case GLUT_RIGHT_BUTTON:
      g_iRightMouseButton = (state==GLUT_DOWN);
      break;
  }
 
  g_vMousePos[0] = x;
  g_vMousePos[1] = y;
}

// gets called whenever a key is pressed
void keyboardFunc (unsigned char key, int x, int y)
{
  switch (key)
  {
    case 27:
      exit(0);
      break;

    case 'p':
      dopause = 1 - dopause;
      break;
    
    //toggle between RK4 and Euler      
	case 't':  
	  toggle = 1 - toggle;
	  break;
	
    //damping/no damping      
	case 'd':  
	  damping = 1- damping;
	  break;
		  
    case ' ':
      saveScreenToFile = 1 - saveScreenToFile;
      break;
  }
}

void specialKey(int key, int x, int y) 
{ // called on special key pressed	
   
	// Check which (arrow) key is pressed
    switch(key) {
                case GLUT_KEY_LEFT :        // Arrow key left
                     if(Fx>=-1.0)
                        Fx -= 0.03f;
                     break;
            
                case GLUT_KEY_RIGHT :    	// Arrow key right
                     if(Fx<=1.0)
                        Fx +=  0.03f;
                     break;
            
                case GLUT_KEY_UP :        	// Arrow key up
                     if(Fy>=-1.0)
                        Fy -= 0.03f;
                     break;
            
                case GLUT_KEY_DOWN :    	// Arrow key down
                     if(Fy<=1.0)
                        Fy += 0.03f;
                     break;    
				}
}


void initString(struct newworld * string)
{
	//initialize the string variables
	string->N          = 11;
	string->num        = 5;
	string->dt         = 0.0007;
	string->mass       = 1.0/12.0;
	string->edgeLength = 1.0/11.0;
	int j              = 0;
	
    for(int i =0;i<12;i++)
	{	
		if(i>=7)
		  {	
		  string->p[i].x = string->p[i-1].x + 1.0/11;
		  string->p[i].y = string->p[i-1].y;
		  j++;
		  }
		else
          {
		  string->p[i].x = 0;
		  string->p[i].y = 0 + (i * 1.0/11);
		  }
		  string->p[i].z = 0;
	}
    
	for(int i =0;i<12;i++)
	{	
		string->v[i].x = 0;
		string->v[i].y = 0;
		string->v[i].z = 0;		
	}
}

// Write a screenshot in the PPM format
void saveScreenshot(int windowWidth, int windowHeight, char *filename)
{
    if (filename == NULL)
        return;
    //allocate picture buffer  
    Pic * in = pic_alloc(windowWidth, windowHeight, 3, NULL);
    printf("File to save to: %s\n", filename);
    for(int i=windowHeight-1; i>=0; i--) 
    {
        glReadPixels(0, windowHeight-i-1, windowWidth, 1, GL_RGB, GL_UNSIGNED_BYTE,
                     &in->pix[i*in->nx*in->bpp]);
    }
    if (ppm_write(filename, in))
        printf("File saved Successfully\n");
    else
        printf("Error in Saving\n");
    
    pic_free(in);
}


