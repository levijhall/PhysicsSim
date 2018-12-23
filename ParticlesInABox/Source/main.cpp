#include "draw.h"
#include "laws.h"
#include "collisions.h"

#define unitTest false

void runTests()
{
	int res = 0;
	vec<2> xy1{ 1.f, 2.f }, xy2{ 3.f,4.f };
	vec<3> xyz1{ 5.f, 6.f, 7.f }, xyz2{ 8.f, 9.f, 10.f };

	printf("xy1 = ");
	print(xy1);
	printf("xy2 = ");
	print(xy2);

	printf("2 * xy1 = ");
	print(2.f * xy1);

	printf("xy1 + xy2 = ");
	print(xy1 + xy2);
	
	double dot = xy1.dotProduct(xy2);
	printf("xy1 dot xy2 = %f\n", dot);

	printf("\n");

	printf("xyz1 = ");
	print(xyz1);
	printf("xyz2 = ");
	print(xyz2);

	vec<3> cross = xyz1.crossProduct(xyz2);
	printf("xyz1 cross xyz2 =");
	print(cross);

	printf("\n");
}

template <typename T>
int sgn(T val)
{
	return (T(0) < val) - (val < T(0));
}

double kineticEnergy(object* ball)
{
	double m = ball->m;
	return 0.5f * m * fabs(ball->v.dotProduct(ball->v));
}

double potientalEnergy(object* ball)
{
	double m = ball->m;
	double y = ball->pos[1];
	return m * fabs(g) * (y - (-1.f + ball->r));
}

double systemEnergy(object* ballpit, const size_t ballCount)
{
	double netEnergy = 0;

	for (size_t i = 0; i < ballCount; i++)
	{
		object* ball = &ballpit[i];
		netEnergy += kineticEnergy(ball);
		netEnergy += potientalEnergy(ball);
	}

	return netEnergy;
}

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

double dist(object* ball1, object* ball2)
{
	vec<2> dist = ball1->pos - ball2->pos;

	return magnitude(dist);
}

int main(void)
{
	unsigned iter = 0;
	const unsigned maxIter = 20;

	if (unitTest)
	{
		runTests();
	}
	
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(480, 480, "Particals in a box", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	///                  x,     y,     xp,      yp,        vx,  vy,     vt,    m,     r,  red,  grn, blu
	object ball1 = { {0.f,  0.75f},  {0.0f,  0.75f},  {0.01f,  0.f},  0.2f,  0.5f, 0.05f, {1.f, 0.f, 0.f} };
	object ball2 = { {0.5f, -0.75f}, {0.5f, -0.75f},  {0.003f, 0.f},  0.05f, 1.f,  0.1f,  {0.f, 1.f, 0.f} };
	object ball3 = { {0.f,  -0.7f},  {0.f,   0.9f},   {0.f,    0.f},  0.2f,  8.f,  0.2f,  {0.f,  0.f, 1.f} };

	object ball4 = { {-0.5f, 0.75f}, {-0.5f, -0.75f}, {0.014f, 0.f},  0.1f, 0.5f,  0.05f, {1.f, 0.5f, 0.2f} };
	object ball5 = { {0.5f, 0.75f},  {-0.5f, -0.75f}, {0.014f, 0.f},  0.1f, 0.5f,  0.05f, {1.f, 0.2f, 0.5f} };
	object empty = { { 0.f,   0.f},  {0.f,    0.f},   {0.f,    0.f},  0.f,  0.f,   0.f,   {0.f,  0.f,  0.f} };

	const size_t ballCount = 5;
	object ballpit[] = { ball1, ball2, ball3, ball4, ball5};

	double netEnergy = 0.f;
	double prevEnergy = 0.f;
	double avgEnergy = 0.f;
	double maxEnergy = 0.f;
	double minEnergy = 0.f;
	double lastEnergy = 0.f;
	unsigned count = 0;
	unsigned lastCount = 0;

	prevEnergy = systemEnergy(ballpit, ballCount);
	minEnergy = prevEnergy;
	maxEnergy = prevEnergy;
	avgEnergy = prevEnergy;
	lastEnergy = prevEnergy;

	printf("Starting energy: %f\n", prevEnergy);
	//**/printf("kinetic: %f, potiential: %f\n", kineticEnergy(&ball1), potientalEnergy(&ball1));

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClearColor(0.f, 0.f, 0.f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
		glLoadIdentity(); //Reset the drawing perspective

		/* Repeat physical calculations scaled with accuracy*/
		do
		{
			for (size_t i = 0; i < ballCount; i++)
			{
				object* ball = &ballpit[i];

				//previous positions
				ball->prev_pos = ball->pos;

				//acceleration
				ball->v[1] += g * timeScale;

				//velocity
				ball->pos += ball->v * timeScale;

				//terminal net velocity
				applyTerminalVelocity(ball);

				//wall collisions
				double x = ball->pos[0];
				double y = ball->pos[1];
				double vx = ball->v[0];
				double vy = ball->v[1];
				if (ball->pos[1] > 1.f - ball->r)
				{
					ball->pos[1] = 2.f - 2 * ball->r - ball->pos[1];
					ball->v[1] *= -1.f;
				}
				if (ball->pos[1] < -1.f + ball->r)
				{
					ball->pos[1] = -2.f + 2 * ball->r - ball->pos[1];
					ball->v[1] *= -1;
				}

				if (ball->pos[0] > 1.f - ball->r)
				{
					ball->pos[0] = 2.f - 2 * ball->r - ball->pos[0];
					ball->v[0] *= -1.f;
				}
				if (ball->pos[0] < -1.f + ball->r)
				{
					ball->pos[0] = -2.f + 2 * ball->r - ball->pos[0];
					ball->v[0] *= -1.f;
				}

				//Removed for the time being.
				//Handles time-reversals, but only for the current object.
				//TO-DO: Path vectors, with time correspondence.
				/*
				if (ball->y < -1.f + ball->r)
				{
					double t = (-1.f + ball->r - ball->yp) / (ball->y - ball->yp);
					ball->y = trajectory(ball->y, ball->yp, t);
					//printf("kinetic: %f, potiential: %f\n", kineticEnergy(ball), potientalEnergy(ball));

					ball->vy = -ball->vy + 2.f * g * timeScale * (1 - t);
					ball->y += ball->vy * (1 - t) * timeScale;

					if (ball->y < -1.f + ball->r)
						ball->y = -1.f + ball->r;
				}
				*/

				//heighest point
				if (ball->v[1] + g * timeScale < 0.f && ball->v[1] > 0.f)
				{
					//**/printf("kinetic: %f, potiential: %f\n", kineticEnergy(ball), potientalEnergy(ball));
				}

				//ball collisions
				for (size_t j = i + 1; j < ballCount; j++)
				{
					object* other = &ballpit[j];

					vec<2> dist = ball->pos - other->pos;
					dist.apply(fabs);
					double Rdist = ball->r + other->r;

					if (dist[0] < Rdist && dist[1] < Rdist)
					{
						//**/printf("close (x(%f), y(%f) < r(%f)\n", Xdist, Ydist, Rdist);
						if (Rdist * Rdist > dist.dotProduct(dist))
						{
							double t = circularBodyTimeInstance(ball, other);
							if (t < 1.f)
							{
								//test0
								double energy0, energy1, energy2, energy3;
								energy0 = kineticEnergy(ball) + kineticEnergy(other);

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

								//test1
								energy1 = kineticEnergy(ball) + kineticEnergy(other);
								if (fabs(energy0 - energy1) > 0.0001f)
									printf("changed kinetic across projection! %f\n", energy0 - energy1);

								//momentum converation
								double newNormBall = ellasticCollision(ball->m, ball->v[0], other->m, other->v[0]);
								double newNormOther = ellasticCollision(other->m, other->v[0], ball->m, ball->v[0]);

								ball->v[0] = newNormBall;
								other->v[0] = newNormOther;

								//test2
								energy2 = kineticEnergy(ball) + kineticEnergy(other);
								if (fabs(energy1 - energy2) > 0.0001f)
									printf("changed kinetic across momentum transfer! %f\n", energy1 - energy2);

								//revert coordinates
								ball->v = coord * ball->v;
								other->v = coord * other->v;

								//ball->vy  = g * (1 - t) * timeScale;
								//other->vy = g * (1 - t) * timeScale;

								//test3
								energy3 = kineticEnergy(ball) + kineticEnergy(other);
								if (fabs(energy2 - energy3) > 0.0001f)
									printf("changed kinetic across reverse projection! %f\n", energy2 - energy3);

								//limit new velocity
								applyTerminalVelocity(ball);
								applyTerminalVelocity(other);

								//use velocity for new direction with remaining turn
								ball->pos += ball->v  * (1 - t) * timeScale;
								other->pos += other->v * (1 - t) * timeScale;
							}
						}
					}
				}
			}
		} while (++iter < maxIter);
		iter = 0;

		/* Draw Objects */
		for (size_t i = 0; i < ballCount; i++)
		{

			object* ball = &ballpit[i];

			//fill
			glColor3d(ball->rgb[0], ball->rgb[1], ball->rgb[2]);
			drawCircleFilled(ball->pos, ball->r);
			//outline
			glColor3d(1.f, 1.f, 1.f);
			drawCircle(ball->pos, ball->r);
			//arrows
			drawArrow(ball->pos, ball->pos + 5 * ball->v, 0.75f);
		}

		netEnergy = systemEnergy(ballpit, ballCount);
		lastEnergy = netEnergy;
		avgEnergy = (avgEnergy * count + netEnergy) / (count + 1);
		count++;

		if (netEnergy > maxEnergy)
			maxEnergy = netEnergy;
		if (netEnergy < minEnergy)
			minEnergy = netEnergy;

		/**/printf("net energy: %f, average: %f, min: %f, max: %f\r", netEnergy, avgEnergy, minEnergy, maxEnergy);

		if (netEnergy - prevEnergy > 0.001f)
		{
			printf("\nSystem changed by %f over %d iterations!\n", netEnergy - prevEnergy, count - lastCount);
			lastCount = count;
			prevEnergy = netEnergy;
		}

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}