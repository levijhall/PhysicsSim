// OrbitingBody.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "draw.h"
#include <memory>

bool isPaused = false;

class instancePool
{
public:
	unsigned count;
	pointMass *d;

	instancePool()
	{
		size = 4;
		d = (pointMass*)malloc(size * sizeof(pointMass));
		count = 0;
	}

	instancePool(unsigned size)
	{
		d = (pointMass*)malloc(size * sizeof(pointMass));
		count = 0;
	}

	~instancePool()
	{
		free(d);
	}

	void addInstance(pointMass *id)
	{
		count++;
		if (count == size)
			reallocPool();
		std::uninitialized_copy(id, id + 1, &d[count - 1]); //why?
	}

	void removeInstance(unsigned i)
	{
		d[i] = d[count];
		count--;
	}

private:
	unsigned size;

	void reallocPool()
	{
		size *= 2;
		d = (pointMass*)realloc(d, size * sizeof(pointMass));
	}
};

int main()
{
	vec<2> origin{ 0.f, 0.f };
	vec<2> CoM{ 0.f, 0.f };
	float tMass = 0.f;
	float G = 1.f;
	float td = 0.01f;

	instancePool pool = instancePool();

	pointMass orb1 = { {0.9700436f, -0.24308753f}, origin, {0.466203685f, 0.43236573f},   0.f, 1.f, 0.05f, {1.f, 0.f, 0.f} };
	pointMass orb2 = { -orb1.pos ,  origin , orb1.v, 0.f, 1.f, 0.05f, {0.f, 1.f, 0.f} };
	pointMass orb3 = { origin , origin , -2.f * orb1.v, 0.f, 1.f, 0.05f, {0.f, 0.f, 1.f} };

	pool.addInstance(&orb1);
	pool.addInstance(&orb2);
	pool.addInstance(&orb3);

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(480, 480, "Orbiting Body", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/*Initialize variables*/

	/*Main Loop*/
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClearColor(0.f, 0.f, 0.f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
		glLoadIdentity(); //Reset the drawing perspective

		//center of mass
		CoM[0] = 0.f;
		CoM[1] = 0.f;
		tMass = 0.f;

		/*Game Logic*/
		for (unsigned i = 0; i < pool.count; i++)
		{
			pointMass *id = &pool.d[i];

			id->prev_pos = id->pos;
			id->pos += id->v * td;
			
			CoM = (CoM * tMass + id->pos * id->m) / (tMass + id->m);
			tMass += id->m;

			for (unsigned j = i + 1; j < pool.count; j++)
			{
				pointMass *other = &pool.d[j];

				vec<2> dist = id->pos - other->pos;
				vec<2> norm = dist / magnitude(dist);

				float acc = -G * id->m * other->m / dist.dotProduct(dist);
				
				id->v += norm * acc * td;
				other->v -= norm * acc * td;
			}

			/*Draw Objects*/
			//Objects will be drawn according to where the "camera" is located, as well as the magnification.
			//Actual values (such as position and velocity) will not be effected as it is purely visual.
			glColor3f(id->rgb[0], id->rgb[1], id->rgb[2]);
			//zoom out
			drawCircleFilled(id->pos / 2.f, id->r / 2.f);
		}
		glColor3f(1.f, 0.f, 1.f);
		drawCircleFilled(CoM / 2.f, 0.01f);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

	}

	glfwTerminate();
	return 0;
}