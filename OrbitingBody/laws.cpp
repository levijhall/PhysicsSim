#include "laws.h"

//force implementation?
//extended collisions

//returns the new v1 value
float ellasticCollision(float m1, float v1, float m2, float v2)
{
	float u1;
	u1 = (m1 - m2) / (m1 + m2) * v1 + (2 * m2) / (m1 + m2) * v2;
	return u1;
}

//returns the new v1 value
float inellasticCollision(float m1, float v1, float m2, float v2)
{
	float u;
	u = m1 / (m1 + m2) * v1;
	return u;
}