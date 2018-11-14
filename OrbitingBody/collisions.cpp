#include "collisions.h"

float circularBodyTimeInstance(object* ball1, object* ball2)
{
	float ax = ball1->xp - ball2->xp;
	float ay = ball1->yp - ball2->yp;
	float bx = ball1->x + ball2->x - ax;
	float by = ball1->y + ball2->y - ay;
	float d = ball1->r + ball2->r;

	float a = (ax * ax + ay * ay - d * d);
	float b = (ax * bx + ay * by);
	float c = (bx * bx + by * by);

	float sqrt = b * b - a * c;

	//no solution, no collision
	if (sqrt < 0.f)
		return 1.f;

	//2s cancels out
	float t1 = (a + sqrt) / c;
	float t2 = (a - sqrt) / c;

	if (t1 > t2)
		 return t2;
	else
		return t1;
}

float trajectory(float x, float xp, float t)
{
	return x * t + xp * (1 - t);
}