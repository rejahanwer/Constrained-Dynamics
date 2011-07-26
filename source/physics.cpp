// String physics code

#include "string.h"
#include "physics.h"
#include <iostream>
#include <gsl/gsl_math.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_vector.h>

using namespace std;

void checkCollission(struct newworld * string, double * forces)
{
    //Self collission:
	//for each bead, check for collision with all other beads.
	//if collide,
	//Add the penalty forces which would act opposite to the current velocity
	
    int jj;
    for(int i =1; i< 12; i++) 
    {  
     for(int j=0; j<12; j++)
        { 
          if(i!=j)
            {
            double r1square = (string->p[i].x-string->p[j].x)*(string->p[i].x-string->p[j].x) +
                              (string->p[i].y-string->p[j].y)*(string->p[i].y-string->p[j].y) ;	
            if(r1square<=(0.03*0.03))
                {
                //add a penalty force, proportional to its velocity but opposite at the bead no i.
                jj = (2 * i ) - 2;
                forces[jj]-= (20 * string->v[i].x);
                forces[jj+1]-= (20 * string->v[i].y);
                }
            }	
        }   
    }
}

// Computes acceleration at every control point of the string, 
// which is in state given by 'string'.
// Returns result in array 'a'

void computeAcceleration(struct newworld * string, struct point a[11])
{

	//create the matrices 
	string->p[0].x = 0;
	string->p[0].y = 0;
	
	string->v[0].x = 0;
	string->v[0].y = 0;
	
	//make mass matrix
	double M[22][22] ={0};	
	for(int i=0 ; i<22; i++)
      for(int j=0; j<22; j++)
        if(i==j)
           M[i][j] = string->mass; 
    
	
	//make constraint gradient matrix
	double Cgradient[12][22] = {0};
           Cgradient[ 0][ 0] = 2 * string->p[1].x;
           Cgradient[ 0][ 1] = 2 * string->p[1].y;
	
	int j = 0 ;
	for(int i =1; i<11; i++)
       {
        j = 2 *(i-1);	
        Cgradient[i][  j] = (-2) * (string->p[i+1].x - string->p[i].x);
        Cgradient[i][j+1] = (-2) * (string->p[i+1].y - string->p[i].y);
        Cgradient[i][j+2] =   2  * (string->p[i+1].x - string->p[i].x);
        Cgradient[i][j+3] =   2  * (string->p[i+1].y - string->p[i].y);
       }
	
        Cgradient[11][20] = 2 *  string->p[11].x;
        Cgradient[11][21] = 2 * (string->p[11].y-0.5);
	
	//get transpose of Cgradient
	double cGradT[22][12] = {0};
	for(int i =0;i<22;i++)
      for(int j =0; j<12;j++)
        cGradT[i][j] = Cgradient[j][i];
	
	
	//make external force matrix
	double Fext[22] = {0};
    for (int i =0; i<22; i++)
        {
         if(i%2!=0)   // odd - y values
		   {  
		   Fext[i] = 0.7;
		   if((Fext[i]<=5.0) && (Fext[i]>=-5.0))
               Fext[i]+=Fy;
		   }
		else // x - values
		   {
			if((Fext[i]<=5.0)&&(Fext[i]>=-5.0))
                Fext[i] +=Fx;
		   }
        }    
	
	
	double dampCoeff = 0;
	if(damping)
	dampCoeff= 0.9;
	
    //flag for baumgarte stabilization
	int    withBGS = 1; 
	double bConst = 10.0;

	
	for(int i =0,ii=1; i<22; i++)
       {
		if(i%2==0)   // even - x values
          Fext[i] += -1 * dampCoeff * string->v[ii].x;
		else         // odd - y values
         {
          Fext[i] += -1 * dampCoeff * string->v[ii].y;
          ii++;
         }	
       }
	
    //check for collission
	checkCollission(string,Fext);
	
	double gradCDot[12][22] = {0};
	gradCDot[0][0] = 2 * string->v[1].x;
	gradCDot[0][1] = 2 * string->v[1].y;
	
	for(int i=1; i<11; i++)
       {
		j = 2 *(i-1);
		gradCDot[i][j]   = (-2) * (string->v[i+1].x - string->v[i].x) ; 
		gradCDot[i][j+1] = (-2) * (string->v[i+1].y - string->v[i].y) ;
		gradCDot[i][j+2] =  (2) * (string->v[i+1].x - string->v[i].x) ;
		gradCDot[i][j+3] =  (2) * (string->v[i+1].y - string->v[i].y) ;
       }
	
	gradCDot[11][20] = 2 * string->v[11].x;
	gradCDot[11][21] = 2 * string->v[11].y;
	
	
	double V[22] = {0};
	for(int i =0,j=1; i<22; i++)
       {   
        if(i%2==0)
          V[i] = string->v[j].x;
        else 
         {
          V[i] = string->v[j].y;
          j++;    
         }  	   
       }
	
	//-1 * gradCDot
	for(int i=0 ; i<12; i++)
      for(int j=0; j<22; j++)
        gradCDot[i][j] = -1 * gradCDot[i][j];
		
	
	//multiply -gradCDot[12][22] * V[22][1] to get productGCDV[12][1]
	double productGCDV[12] = {0};
	for(int i=0; i< 12; i++)
      for(int k=0; k< 22; k++)
		productGCDV[i]= productGCDV[i]+ gradCDot[i][k] * V[k];
	
	//find Cgradient * qDot
	double productCgradQdot[12] = {0};
	
	for(int i=0; i< 12; i++)
      for(int k=0; k< 22; k++)
		productCgradQdot[i] = productCgradQdot[i]+ Cgradient[i][k] * V[k];
	
	//b * productCgradQdot
	for(int i=0; i< 12; i++)
      productCgradQdot[i] = bConst * productCgradQdot[i];
	
	//make C vector
	double C[12];
           C[ 0] = (string->p[1].x     * string->p[1].x    ) +
                   (string->p[1].y     * string->p[1].y    ) - 
                   (string->edgeLength * string->edgeLength) ;
	
	for(int i=1; i<11; i++)
        C[i] = ((string->p[i+1].x - string->p[i].x) * (string->p[i+1].x - string->p[i].x)) + 
               ((string->p[i+1].y - string->p[i].y) * (string->p[i+1].y - string->p[i].y)) - 
                (string->edgeLength*string->edgeLength);
	
    
    C[11] = (string->p[11].x     *  string->p[11].x)      + 
           ((string->p[11].y-0.5)* (string->p[11].y-0.5)) -
            (0.5 * 0.5);
	
	//(b*b) * C
	for(int i =0; i<12; i++)
		C[i] = bConst * bConst * C[i];
	
	//sum up for RHS matrix --> productGCDV - productCgradQdot - C
	double baumgarte[12];
	for(int i =0; i<12; i++)
		baumgarte[i] = productGCDV[i] - productCgradQdot[i] - C[i] ;
	
		
	//make the system matrices for the solver
	double bigMatrix[34][34] = {0};
	for(int i =0; i<22; i++)
      for(int j =0; j<22; j++)
        bigMatrix[i][j] = M[i][j]; 
	
	for(int i =22,ii=0; i<34; i++,ii++)
      for(int j =0; j<22; j++)
		bigMatrix[i][j] = Cgradient[ii][j];
	
	for(int i =0; i<22; i++)
      for(int j =22,jj=0; j<34; j++,jj++)
		bigMatrix[i][j] = cGradT[i][jj];
	
	double rhsMatrix[34][1];
	for(int i=0;i<22;i++)
      rhsMatrix[i][0] = Fext[i];
	
	
    if(withBGS==0)
     for(int i=22,ii=0;i<34;i++,ii++)
       rhsMatrix[i][0] = productGCDV[ii];
	
	else //with Baumgarte
      for(int i=22,ii=0;i<34;i++,ii++)
        rhsMatrix[i][0] = baumgarte[ii];
	
		
		
	gsl_matrix * A = gsl_matrix_alloc (34, 34); // A matrix
	
	for(int i = 0; i<34; i++)
      for(int j = 0 ; j<34; j++)
        gsl_matrix_set (A, i, j, bigMatrix[i][j]); // set values of A[][] as the bigMatrix
	
	// Perform SVD decomposition on bigMatrix
	gsl_matrix * V1   = gsl_matrix_alloc (34, 34);
	gsl_vector * S    = gsl_vector_alloc (34);
	gsl_vector * work = gsl_vector_alloc (34);
	
	int ah = gsl_linalg_SV_decomp(A, V1, S, work);
	
	gsl_vector * unknown = gsl_vector_alloc (34);
	gsl_vector * b       = gsl_vector_alloc (34);
	
	
	for(int i = 0; i <34; i++)
        gsl_vector_set (b, i, rhsMatrix[i][0]);
	
	int ah2;
	ah2 = gsl_linalg_SV_solve (A, V1, S, b, unknown);

	for(int i =0,ii=1; i<22; i++)
       {   
		if(i%2==0) // even - x value
		a[ii].x = gsl_vector_get(unknown, i );
		else  //odd- y value
		{
		a[ii].y = gsl_vector_get(unknown, i );
		ii++;
		}
       }
}

// performs one step of Euler Integration
// updates the string 
void Euler(struct newworld * string)
{
	int i,j,k;
    //acceleration of the mass points
	point a[12]         =      {0};								
	computeAcceleration(string, a);
	
	for (int i=1; i<12; i++)
        {
         string->p[i].x += string->dt * string->v[i].x;
		 string->p[i].y += string->dt * string->v[i].y;

		 string->v[i].x += string->dt * a[i].x;
		 string->v[i].y += string->dt * a[i].y;
        }	
}


// performs one step of RK4 Integration
// update string

void RK4(struct newworld * string)
{
	point F1p[12], F1v[12], 
          F2p[12], F2v[12],
          F3p[12], F3v[12],
          F4p[12], F4v[12];
	
	point a[12] = {0};
	
	struct newworld buffer;
	int    i,j,k;

	buffer = *string; // make a copy of string
	
	computeAcceleration(string, a);
	for (i=1; i<12; i++)
        {
		pMULTIPLY(string->v[i],string->dt,F1p[i]);
		pMULTIPLY(a[i],string->dt,F1v[i]);
		pMULTIPLY(F1p[i],0.5,buffer.p[i]);
		pMULTIPLY(F1v[i],0.5,buffer.v[i]);
		pSUM(string->p[i],buffer.p[i],buffer.p[i]);
		pSUM(string->v[i],buffer.v[i],buffer.v[i]);
        }
	
	computeAcceleration(&buffer, a);
	for (i=0; i<12; i++)
        {
		// F2p = dt * buffer.v;
		pMULTIPLY(buffer.v[i],string->dt,F2p[i]);
		// F2v = dt * a(buffer.p,buffer.v);     
		pMULTIPLY(a[i],string->dt,F2v[i]);
		pMULTIPLY(F2p[i],0.5,buffer.p[i]);
		pMULTIPLY(F2v[i],0.5,buffer.v[i]);
		pSUM(string->p[i],buffer.p[i],buffer.p[i]);
		pSUM(string->v[i],buffer.v[i],buffer.v[i]);
        }
	
	computeAcceleration(&buffer, a);
	for (i=0; i<12; i++)
        {
		// F3p = dt * buffer.v;
		pMULTIPLY(buffer.v[i],string->dt,F3p[i]);
		// F3v = dt * a(buffer.p,buffer.v);     
		pMULTIPLY(a[i],string->dt,F3v[i]);
		pMULTIPLY(F3p[i],0.5,buffer.p[i]);
		pMULTIPLY(F3v[i],0.5,buffer.v[i]);
		pSUM(string->p[i],buffer.p[i],buffer.p[i]);
		pSUM(string->v[i],buffer.v[i],buffer.v[i]);
        }
	
	computeAcceleration(&buffer, a);
	for (i=0; i<12; i++)
        {
		// F3p = dt * buffer.v;
		pMULTIPLY(buffer.v[i],string->dt,F4p[i]);
		// F3v = dt * a(buffer.p,buffer.v);     
		pMULTIPLY(a[i],string->dt,F4v[i]);
		
		pMULTIPLY(F2p[i],2,buffer.p[i]);
		pMULTIPLY(F3p[i],2,buffer.v[i]);
		pSUM(buffer.p[i],buffer.v[i],buffer.p[i]);
		pSUM(buffer.p[i],F1p[i],buffer.p[i]);
		pSUM(buffer.p[i],F4p[i],buffer.p[i]);
		pMULTIPLY(buffer.p[i],1.0 / 6,buffer.p[i]);
		pSUM(buffer.p[i],string->p[i],string->p[i]);
		
		pMULTIPLY(F2v[i],2,buffer.p[i]);
		pMULTIPLY(F3v[i],2,buffer.v[i]);
		pSUM(buffer.p[i],buffer.v[i],buffer.p[i]);
		pSUM(buffer.p[i],F1v[i],buffer.p[i]);
		pSUM(buffer.p[i],F4v[i],buffer.p[i]);
		pMULTIPLY(buffer.p[i],1.0 / 6,buffer.p[i]);
		pSUM(buffer.p[i],string->v[i],string->v[i]);
        }
	
	return;  
}
