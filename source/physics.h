//String-physics code
//Rejah Anuvar

#ifndef _PHYSICS_H_
#define _PHYSICS_H_


void computeAcceleration(struct newworld * string, struct point a[8]);
void checkCollission    (struct newworld * string, double * forces);

void Euler(struct newworld * string);
void RK4  (struct newworld * string);

#endif

