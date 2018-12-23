#include "collisions.h"

double circularBodyTimeInstance(object* ball1, object* ball2)
{
	double ax = ball1->prev_pos[0] - ball2->prev_pos[0];
	double ay = ball1->prev_pos[1] - ball2->prev_pos[1];
	double bx = ball1->pos[0] + ball2->pos[0] - ax;
	double by = ball1->pos[1] + ball2->pos[1] - ay;
	double d = ball1->r + ball2->r;

	double a = (ax * ax + ay * ay - d * d);
	double b = (ax * bx + ay * by);
	double c = (bx * bx + by * by);

	double sqrt = b * b - a * c;

	//no solution, no collision
	if (sqrt < 0.f)
		return 1.f;

	//2s cancels out
	double t1 = (a + sqrt) / c;
	double t2 = (a - sqrt) / c;

	if (t1 > t2)
		 return t2;
	else
		return t1;
}

double trajectory(double x, double xp, double t)
{
	return x * t + xp * (1 - t);
}