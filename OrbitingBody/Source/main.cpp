#include "draw.h"
#include "laws.h"
#include "collisions.h"

template <typename T>
int sgn(T val)
{
	return (T(0) < val) - (val < T(0));
}

float systemEnergy(object* ballpit, const size_t ballCount)
{
	float netEnergy = 0;

	for (size_t i = 0; i < ballCount; i++)
	{
		object* ball = &ballpit[i];

		float m = ball->m;
		float y = ball->y;
		float vx = ball->vx;
		float vy = ball->vy;
		//1/2 mv^2 + mgh
		netEnergy += 0.5f * m * fabs(vx * vx + vy * vy) + fabs(m * g * (y - 1.f));
	}

	return netEnergy;
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(480, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	///                  x,      y,    xp,     yp,     vx,  xy,    vt,   m,     r, red, grn, blu
	object ball1 = { -0.5f, -0.75f, -0.5f, -0.75f, 0.014f, 0.f,  0.1f, 0.5f, 0.05f, 1.f, 0.f, 0.f };
	object ball2 = {  0.5f, -0.75f,  0.5f, -0.75f, 0.003f, 0.f, 0.05f,  1.f, 0.1f, 0.f, 1.f, 0.f };
	object ball3 = {   0.f,  -0.9f,   0.f,   0.9f,    0.f, 0.f,  0.2f,  8.f, 0.2f, 0.f,  0.f, 1.f };

	object ball4 = { -0.5f, 0.75f, -0.5f, -0.75f, 0.014f, 0.f,  0.1f, 0.5f, 0.05f, 1.f, 0.5f, 0.2f };
	object ball5 = { 0.5f, 0.75f, -0.5f, -0.75f, 0.014f, 0.f,  0.1f, 0.5f, 0.05f, 1.f, 0.2f, 0.5f };

	const size_t ballCount = 5;
	object ballpit[] = { ball1, ball2, ball3, ball4, ball5 };

	float netEnergy = 0.f;
	float prevEnergy = 0.f;
	float avgEnergy = 0.f;
	float maxEnergy = 0.f;
	float minEnergy = 0.f;
	float lastEnergy = 0.f;
	uint count = 0;

	prevEnergy = systemEnergy(ballpit, ballCount);
	minEnergy = prevEnergy;
	maxEnergy = prevEnergy;
	avgEnergy = prevEnergy;
	lastEnergy = prevEnergy;

	printf("Starting energy: %f\n", prevEnergy);

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

			//velocity
			ball->x += ball->vx;
			ball->y += ball->vy;

			//acceleration
			//ball->vy += g;

			//terminal net velocity
			//method: similar triangles
			float v = sqrtf(ball->vx * ball->vx + ball->vy * ball->vy);
			if (v > ball->vt)
			{
				ball->vx = ball->vx * ball->vt / v;
				ball->vy = ball->vy * ball->vt / v;
			}
				
			

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
							//momentum converation
							//**/printf("hit (%f > %f %f at [%f])\n", Rdist, Xdist, Ydist, t);
							float newVX = ellasticCollision(ball->m, ball->vx, other->m, other->vx);
							float newVY = ellasticCollision(ball->m, ball->vy, other->m, other->vy);

							other->vx = ellasticCollision(other->m, other->vx, ball->m, ball->vx);
							other->vy = ellasticCollision(other->m, other->vy, ball->m, ball->vy);

							ball->vx = newVX;
							ball->vy = newVY;

							//point of collision
							ball->x = ball->x * t + ball->xp * (1 - t);
							ball->y = ball->y * t + ball->yp * (1 - t);
							other->x = other->x * t + other->xp * (1 - t);
							other->y = other->y * t + other->yp * (1 - t);

							//use velocity for new direction
							ball->x += ball->vx * (1 - t);
							ball->y += ball->vy * (1 - t);
							other->x += other->vx * (1 - t);
							other->y += other->vy * (1 - t);

							//netEnergy = systemEnergy(ballpit, ballCount);
							/*
							float changeInEnergy = netEnergy - lastEnergy;
							if (fabs(changeInEnergy) > 0.0001f)
								printf("changed: %f\n", changeInEnergy);
							*/
						}
						
					}
				}
			}

			glColor3f(ball->red, ball->green, ball->blue);
			drawCircleFilled(ball->r, ball->x, ball->y);
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

		printf("net energy: %f, average: %f, min: %f, max: %f\r", netEnergy, avgEnergy, minEnergy, maxEnergy);

		if (netEnergy - prevEnergy > 0.001f)
		{
			prevEnergy = netEnergy;
			/**/printf("\nchange\n");
		}
			

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}