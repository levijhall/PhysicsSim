#include "draw.h"
#include "laws.h"
#include "collisions.h"
#include "utility.h"
#include <memory>

#define unitTest false

unsigned iter = 0;
const unsigned maxIter = 160;
const double timeScale = 0.005f;

class trail
{
public:
	void drawTrail()
	{
		for (unsigned i = 0; i < count; i++)
		{
			glColor4d(1.f, 1.f, 1.f, 1.f);
			drawCircle(d[i], 0.001f, 6);
		}
	}
	void addPoint(vec<2> point)
	{
		if (count < size)
		{
			std::uninitialized_copy(&point, &point + 1, &d[curr]);
			curr++;
			count++;
		}
		else
		{
			curr = curr % size;
			d[curr] = point;
			curr++;
		}
		
	}
private:
	const unsigned size = 5000;
	unsigned count = 0;
	unsigned curr = 0;

	vec<2> d[5000];
};

struct box
{
	vec<2> pos = { 0.f, 0.f };
	vec<2> prev_pos = { 0.f, 0.f };
	vec<2> v = { 0.f, 0.f };
	vec<2> dimensions = { 1.9f, 1.9f };
	double m = 50.f;
	double vt = 0.1f;
};

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

int main(void)
{
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
	object ball3 = { {0.f,  -0.7f},  {0.f,   0.9f},   { -0.00275f,    0.f},  0.2f,  8.f,  0.2f,  {0.f,  0.f, 1.f} };

	object ball4 = { {-0.5f, 0.75f}, {-0.5f, -0.75f}, {0.014f, 0.f},  0.1f, 0.5f,  0.05f, {1.f, 0.5f, 0.2f} };
	object ball5 = { {0.5f, 0.75f},  {-0.5f, -0.75f}, {0.014f, 0.f},  0.1f, 0.5f,  0.05f, {1.f, 0.2f, 0.5f} };
	object empty = { { 0.f,   0.f},  {0.f,    0.f},   {0.f,    0.f},  0.f,  0.f,   0.f,   {0.f,  0.f,  0.f} };

	const size_t ballCount = 5;
	object ballpit[] = { ball1, ball2, ball3, ball4, ball5};

	box walls;

	trail box;

	double netEnergy = 0.f;
	double prevEnergy = 0.f;
	double avgEnergy = 0.f;
	double maxEnergy = 0.f;
	double minEnergy = 0.f;
	double lastEnergy = 0.f;
	unsigned count = 0;
	unsigned lastCount = 0;

	prevEnergy = systemEnergy(ballpit, ballCount) + 0.5f * walls.m * fabs(walls.v.dotProduct(walls.v));
	minEnergy = prevEnergy;
	maxEnergy = prevEnergy;
	avgEnergy = prevEnergy;
	lastEnergy = prevEnergy;

	printf("Starting energy: %f\n", prevEnergy);
	//**/printf("kinetic: %f, potiential: %f\n", kineticEnergy(&ball1), potientalEnergy(&ball1));

	unsigned drawCount = 0;

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
			walls.prev_pos = walls.pos;
			walls.pos += walls.v * timeScale;

			for (size_t i = 0; i < ballCount; i++)
			{
				object* ball = &ballpit[i];

				//previous positions
				ball->prev_pos = ball->pos;

				//acceleration
				ball->v[1] += g * timeScale;

				//terminal net velocity
				applyTerminalVelocity(ball);

				//velocity
				ball->pos += ball->v * timeScale;	

				//wall collisions
				if (ball->pos[0] > walls.pos[0] + walls.dimensions[0] / 2.f - ball->r)
				{
					double t = (walls.pos[0] + walls.dimensions[0] / 2.f - ball->r - ball->prev_pos[0]) / (ball->pos[0] - ball->prev_pos[0]);
					if (t > 1.f)
						t = 1.f;
					if (t < -1.f)
						t = -1.f;
					ball->pos[0] = ball->prev_pos[0] * (1 - t) + ball->pos[0] * t;
					walls.pos[0] = walls.prev_pos[0] * (1 - t) + walls.pos[0] * t;

					double ballNewV = ellasticCollision(ball->m, ball->v[0], walls.m, walls.v[0]);
					double wallNewV = ellasticCollision(walls.m, walls.v[0], ball->m, ball->v[0]);

					ball->v[0] = ballNewV;
					walls.v[0] = wallNewV;

					applyTerminalVelocity(ball);
					double v = magnitude(walls.v);
					double vt = walls.vt;
					if (v > vt)
					{
						/**/printf("terminal wall\n");
						walls.v *= vt / v;
					}

					ball->pos[0] += ball->v[0] * (1 - t) * timeScale;
					walls.pos[0] += walls.v[0] * (1 - t) * timeScale;
				}
				if (ball->pos[0] < walls.pos[0] - walls.dimensions[0] / 2.f + ball->r)
				{
					double t = (walls.pos[0] - walls.dimensions[0] / 2.f + ball->r - ball->prev_pos[0]) / (ball->pos[0] - ball->prev_pos[0]);
					if (t > 1.f)
						t = 1.f;
					if (t < -1.f)
						t = -1.f;
					ball->pos[0] = ball->prev_pos[0] * (1 - t) + ball->pos[0] * t;
					walls.pos[0] = walls.prev_pos[0] * (1 - t) + walls.pos[0] * t;

					double ballNewV = ellasticCollision(ball->m, ball->v[0], walls.m, walls.v[0]);
					double wallNewV = ellasticCollision(walls.m, walls.v[0], ball->m, ball->v[0]);

					ball->v[0] = ballNewV;
					walls.v[0] = wallNewV;

					applyTerminalVelocity(ball);
					double v = magnitude(walls.v);
					double vt = walls.vt;
					if (v > vt)
					{
						/**/printf("terminal wall\n");
						walls.v *= vt / v;
					}

					ball->pos[0] += ball->v[0] * (1 - t) * timeScale;
					walls.pos[0] += walls.v[0] * (1 - t) * timeScale;
				}

				if (ball->pos[1] > walls.pos[1] + walls.dimensions[1] / 2.f - ball->r)
				{
					double t = (walls.pos[1] + walls.dimensions[1] / 2.f - ball->r - ball->prev_pos[1]) / (ball->pos[1] - ball->prev_pos[1]);
					if (t > 1.f)
						t = 1.f;
					if (t < -1.f)
						t = -1.f;
					ball->pos[1] = ball->prev_pos[1] * (1 - t) + ball->pos[1] * t;
					walls.pos[1] = walls.prev_pos[1] * (1 - t) + walls.pos[1] * t;

					double ballNewV = ellasticCollision(ball->m, ball->v[1], walls.m, walls.v[1]);
					double wallNewV = ellasticCollision(walls.m, walls.v[1], ball->m, ball->v[1]);

					ball->v[1] = ballNewV;
					walls.v[1] = wallNewV;

					applyTerminalVelocity(ball);
					double v = magnitude(walls.v);
					double vt = walls.vt;
					if (v > vt)
					{
						/**/printf("terminal wall\n");
						walls.v *= vt / v;
					}

					ball->pos[1] += ball->v[1] * (1 - t) * timeScale;
					walls.pos[1] += walls.v[1] * (1 - t) * timeScale;
				}
				if (ball->pos[1] < walls.pos[1] - walls.dimensions[1] / 2.f + ball->r)
				{
					double t = (walls.pos[1] - walls.dimensions[1] / 2.f + ball->r - ball->prev_pos[1]) / (ball->pos[1] - ball->prev_pos[1]);
					if (t > 1.f)
						t = 1.f;
					if (t < -1.f)
						t = -1.f;
					ball->pos[1] = ball->prev_pos[1] * (1 - t) + ball->pos[1] * t;
					walls.pos[1] = walls.prev_pos[1] * (1 - t) + walls.pos[1] * t;

					double ballNewV = ellasticCollision(ball->m, ball->v[1], walls.m, walls.v[1]);
					double wallNewV = ellasticCollision(walls.m, walls.v[1], ball->m, ball->v[1]);

					ball->v[1] = ballNewV;
					walls.v[1] = wallNewV;

					applyTerminalVelocity(ball);
					double v = magnitude(walls.v);
					double vt = walls.vt;
					if (v > vt)
					{
						/**/printf("terminal wall\n");
						walls.v *= vt / v;
					}

					ball->pos[1] += ball->v[1] * (1 - t) * timeScale;
					walls.pos[1] += walls.v[1] * (1 - t) * timeScale;
				}

				//ball collisions
				for (size_t j = i + 1; j < ballCount; j++)
				{
					object* other = &ballpit[j];

					circularBodyCollision(ball, other, ellasticCollision, timeScale);

					applyTerminalVelocity(ball);
					applyTerminalVelocity(other);
				}
			}
		} while (++iter < maxIter);
		iter = 0;

		if (drawCount++ > 5)
		{
			drawCount = 0;
			box.addPoint(walls.pos);
		}
		box.drawTrail();

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

		/* Draw Walls */
		glColor4d(1.f, 1.f, 1.f, 1.f);
		glBegin(GL_LINE_LOOP);
		glVertex2d(walls.pos[0] + walls.dimensions[0] / 2, walls.pos[1] + walls.dimensions[1] / 2);
		glVertex2d(walls.pos[0] - walls.dimensions[0] / 2, walls.pos[1] + walls.dimensions[1] / 2);
		glVertex2d(walls.pos[0] - walls.dimensions[0] / 2, walls.pos[1] - walls.dimensions[1] / 2);
		glVertex2d(walls.pos[0] + walls.dimensions[0] / 2, walls.pos[1] - walls.dimensions[1] / 2);
		glEnd();

		netEnergy = systemEnergy(ballpit, ballCount) + 0.5f * walls.m * fabs(walls.v.dotProduct(walls.v));
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