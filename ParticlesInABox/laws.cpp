#include "laws.h"

//force implementation?
//extended collisions

//returns the new v1 value
double ellasticCollision(double m1, double v1, double m2, double v2)
{
	double u1;
	u1 = (m1 - m2) / (m1 + m2) * v1 + (2 * m2) / (m1 + m2) * v2;
	return u1;
}

//returns the new v1 value
double inellasticCollision(double m1, double v1, double m2, double v2)
{
	double u;
	u = m1 / (m1 + m2) * v1;
	return u;
}