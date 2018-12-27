#include "collisions.h"

void applyTerminalVelocity(object* ball)
{
	//method: similar triangles
	double v = magnitude(ball->v);
	double vt = ball->vt;
	if (v > vt)
	{
		/**/printf("terminal\n");
		ball->v *= vt / v;
	}
}

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

void circularBodyCollision(object *ball, object *other, double (*momentumTransfor)(double, double, double, double), double timeScale)
{
	vec<2> dist = ball->pos - other->pos;
	dist.apply(fabs);
	double Rdist = ball->r + other->r;

	if (dist[0] < Rdist && dist[1] < Rdist)
	{
		if (Rdist * Rdist > dist.dotProduct(dist))
		{
			double t = circularBodyTimeInstance(ball, other);
			if (t < 1.f)
			{
				//reverse to earliest point of contact
				ball->pos = ball->pos  * t + ball->prev_pos  * (1 - t);
				other->pos = other->pos * t + other->prev_pos * (1 - t);
				dist = ball->pos - other->pos;

				//normal vector along contact point
				double magn = magnitude(dist); //t is not precise enough (magn != r + R)
				vec<2> norm = (ball->pos - other->pos) / magn;
				vec<2> perp{ -norm[1], norm[0] };

				//projection of velocities along normal and perpendicular coordinates
				matrix<2> coord = concatColumns(norm, perp);
				ball->v = T(coord) * ball->v;
				other->v = T(coord) * other->v;

				double newNormBall  = (*momentumTransfor)(ball->m, ball->v[0], other->m, other->v[0]);
				double newNormOther = (*momentumTransfor)(other->m, other->v[0], ball->m, ball->v[0]);
			
				ball->v[0] = newNormBall;
				other->v[0] = newNormOther;

				//revert coordinates
				ball->v = coord * ball->v;
				other->v = coord * other->v;

				//use velocity for new direction with remaining turn
				ball->pos += ball->v  * (1 - t) * timeScale;
				other->pos += other->v * (1 - t) * timeScale;
			}
		}
	}
}