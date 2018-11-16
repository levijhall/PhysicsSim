#include "draw.h"
#include "laws.h"
#include "collisions.h"

template <typename T>
int sgn(T val)
{
	return (T(0) < val) - (val < T(0));
}

float kineticEnergy(object* ball)
{
	float m = ball->m;
	float vx = ball->vx;
	float vy = ball->vy;
	return 0.5f * m * fabs(vx * vx + vy * vy);
}

float potientalEnergy(object* ball)
{
	float m = ball->m;
	float y = ball->y;
	return m * fabs(g) * (y - (-1.f + ball->r));
}

float systemEnergy(object* ballpit, const size_t ballCount)
{
	float netEnergy = 0;

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
	float v = sqrtf(ball->vx * ball->vx + ball->vy * ball->vy);
	float vt = ball->vt;
	if (v > vt)
	{
		/**/printf("terminal\n");
		ball->vx = ball->vx * vt / v;
		ball->vy = ball->vy * vt / v;
	}
}

float dist(object* ball1, object* ball2)
{
	float x1 = ball1->x;
	float x2 = ball2->x;
	float y1 = ball1->y;
	float y2 = ball2->y;
	float dx = x1 - x2;
	float dy = y1 - y2;

	return sqrtf(dx * dx + dy * dy);
}

int main(void)
{
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
	///                  x,      y,    xp,     yp,     vx,  vy,    vt,   m,     r, red, grn, blu
	object ball1 = {   0.f,  0.75f,  0.0f,  0.75f,  0.01f, 0.f,  0.2f, 0.5f, 0.05f, 1.f, 0.f, 0.f };
	object ball2 = {  0.5f, -0.75f,  0.5f, -0.75f, 0.003f, 0.f, 0.05f,  1.f, 0.1f, 0.f, 1.f, 0.f };
	object ball3 = {   0.f,  -0.7f,   0.f,   0.9f,    0.f, 0.f,  0.2f,  8.f, 0.2f, 0.f,  0.f, 1.f };

	object ball4 = { -0.5f, 0.75f, -0.5f, -0.75f, 0.014f, 0.f,  0.1f, 0.5f, 0.05f, 1.f, 0.5f, 0.2f };
	object ball5 = {  0.5f, 0.75f, -0.5f, -0.75f, 0.014f, 0.f,  0.1f, 0.5f, 0.05f, 1.f, 0.2f, 0.5f };
	object empty = {   0.f,   0.f,   0.f,    0.f,    0.f, 0.f,   0.f,  0.f,   0.f, 0.f,  0.f,  0.f };

	const size_t ballCount = 5;
	object ballpit[] = { ball1, ball2, ball3, ball4, ball5};

	float netEnergy = 0.f;
	float prevEnergy = 0.f;
	float avgEnergy = 0.f;
	float maxEnergy = 0.f;
	float minEnergy = 0.f;
	float lastEnergy = 0.f;
	uint count = 0;
	uint lastCount = 0;

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

		for (size_t i = 0; i < ballCount; i++)
		{
			object* ball = &ballpit[i];

			//previous positions
			ball->xp = ball->x;
			ball->yp = ball->y;

			//acceleration
			ball->vy += g * timeScale;

			//velocity
			ball->x += ball->vx * timeScale;
			ball->y += ball->vy * timeScale;

			//terminal net velocity
			applyTerminalVelocity(ball);

			//wall collisions
			if (ball->y > 1.f - ball->r)
			{
				ball->y = 2.f - 2 * ball->r - ball->y;
				ball->vy = -ball->vy;
			}
			if (ball->y < -1.f + ball->r)
			{
				ball->y = -2.f + 2 * ball->r - ball->y;
				ball->vy = -ball->vy;
			}

			if (ball->x > 1.f - ball->r)
			{
				ball->x = 2.f - 2 * ball->r - ball->x;
				ball->vx = -ball->vx;
			}
			if (ball->x < -1.f + ball->r)
			{
				ball->x = -2.f + 2 * ball->r - ball->x;
				ball->vx = -ball->vx;
			}
			/*
			if (ball->y < -1.f + ball->r)
			{
				float t = (-1.f + ball->r - ball->yp) / (ball->y - ball->yp);
				ball->y = trajectory(ball->y, ball->yp, t);
				//printf("kinetic: %f, potiential: %f\n", kineticEnergy(ball), potientalEnergy(ball));

				ball->vy = -ball->vy + 2.f * g * timeScale * (1 - t);
				ball->y += ball->vy * (1 - t) * timeScale;

				if (ball->y < -1.f + ball->r)
					ball->y = -1.f + ball->r;
			}
			*/

			if (ball->vy + g * timeScale < 0.f && ball->vy > 0.f)
			{
				//**/printf("kinetic: %f, potiential: %f\n", kineticEnergy(ball), potientalEnergy(ball));
			}
				
			//ball collisions
			for (size_t j = i + 1; j < ballCount; j++)
			{
				object* other = &ballpit[j];

				float Xdist = fabs(ball->x - other->x);
				float Ydist = fabs(ball->y - other->y);
				float Rdist = ball->r + other->r;

				if (Xdist < Rdist && Ydist < Rdist)
				{
					//**/printf("close (x(%f), y(%f) < r(%f)\n", Xdist, Ydist, Rdist);
					if (Rdist * Rdist > Xdist * Xdist + Ydist * Ydist)
					{
						float t = circularBodyTimeInstance(ball, other);
						if (t < 1.f)
						{
							//test0
							float energy0, energy1, energy2, energy3;
							energy0 = kineticEnergy(ball) + kineticEnergy(other);

							//reverse to earliest point of contact
							ball->x  = ball->x  * t + ball->xp  * (1 - t);
							ball->y  = ball->y  * t + ball->yp  * (1 - t);
							other->x = other->x * t + other->xp * (1 - t);
							other->y = other->y * t + other->yp * (1 - t);

							//normal vector along contact point
							float magn  =  ball->r + other->r;
							float distance = dist(ball, other); //t is not precise enough (magn != distance)
							float xNorm = (ball->x - other->x) / distance;
							float yNorm = (ball->y - other->y) / distance;
							float xPerp = -yNorm;
							float yPerp =  xNorm;

							//projection of velocities along normal and perpendicular
							float normBall  = xNorm * ball->vx  + yNorm * ball->vy;
							float normOther = xNorm * other->vx + yNorm * other->vy;
							float perpBall  = xPerp * ball->vx  + yPerp * ball->vy;
							float perpOther = xPerp * other->vx + yPerp * other->vy;

							//test1
							empty.m  = ball->m;
							empty.vx = normBall;
							empty.vy = perpBall;
							energy1 = kineticEnergy(&empty);

							empty.m  = other->m;
							empty.vx = normOther;
							empty.vy = perpOther;
							energy1 += kineticEnergy(&empty);

							if (fabs(energy0 - energy1) > 0.0001f)
								printf("changed kinetic across projection!%f\n", energy0 - energy1);

							//momentum converation
							float newNormBall  = ellasticCollision(ball->m,  normBall,  other->m, normOther);
							float newNormOther = ellasticCollision(other->m, normOther, ball->m,  normBall);

							//test2
							empty.m = ball->m;
							empty.vx = newNormBall;
							empty.vy = perpBall;
							energy2 = kineticEnergy(&empty);

							empty.m = other->m;
							empty.vx = newNormOther;
							empty.vy = perpOther;
							energy2 += kineticEnergy(&empty);

							if (fabs(energy1 - energy2) > 0.0001f)
								printf("changed kinetic across momentum transfer! %f\n", energy1 - energy2);

							//revert coordinates
							ball->vx  = newNormBall  * xNorm + perpBall  * xPerp;
							ball->vy  = newNormBall  * yNorm + perpBall  * yPerp;
							other->vx = newNormOther * xNorm + perpOther * xPerp;
							other->vy = newNormOther * yNorm + perpOther * yPerp;

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
							ball->x  += ball->vx  * (1 - t) * timeScale;
							ball->y  += ball->vy  * (1 - t) * timeScale;
							other->x += other->vx * (1 - t) * timeScale;
							other->y += other->vy * (1 - t) * timeScale;
						}
					}
				}
			}

			//fill
			glColor3f(ball->red, ball->green, ball->blue);
			drawCircleFilled(ball->r, ball->x, ball->y);
			//outline
			glColor3f(1.f, 1.f, 1.f);
			drawCircle(ball->r, ball->x, ball->y);
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