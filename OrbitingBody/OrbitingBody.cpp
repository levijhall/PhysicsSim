// OrbitingBody.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "draw.h"
#include "camera.h"
#include "mouse.h"
#include "keyboard.h"
#include "instancePool.h"
#include <iostream>

int SCREEN_WIDTH = 480;
int SCREEN_HEIGHT = 480;
double SCREEN_ASPECT = (double)SCREEN_WIDTH / (double)SCREEN_HEIGHT;

void scrollCallback(GLFWwindow *window, double xOffset, double yOffset);
void windowSizeCallback(GLFWwindow *window, int width, int height);

/*Global camera*/
Camera cam;
Mouse mouse;

bool isPaused = false;

int state;

int main()
{
	vec<2> origin{ 0.f, 0.f };
	vec<2> CoM{ 0.f, 0.f };
	double tMass = 0.f;
	double G = 1.f;
	double td = 0.0001f;

	unsigned looped = 0;
	unsigned LOOP = 20;

	instancePool<pointMass> pool = instancePool<pointMass>();

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
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Orbiting Body", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/*Initialize input*/
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetCursorPosCallback(window, cursorPositionCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetWindowSizeCallback(window, windowSizeCallback);

	/*Main Loop*/
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClearColor(0.f, 0.f, 0.f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
		glLoadIdentity(); //Reset the drawing perspective
		glScaled(1.f, SCREEN_ASPECT, 1.f);

		/* Reset values */
		if (!isPaused)
		{
			//center of mass
			CoM[0] = 0.f;
			CoM[1] = 0.f;
			tMass = 0.f;
		}

		/*Game Logic*/
		do
		{
			for (unsigned i = 0; i < pool.count; i++)
			{
				pointMass *id = &pool.d[i];

				/* Update Object data */
				if (!isPaused)
				{
					id->prev_pos = id->pos;
					id->pos += id->v * td;

					CoM = (CoM * tMass + id->pos * id->m) / (tMass + id->m);
					tMass += id->m;

					for (unsigned j = i + 1; j < pool.count; j++)
					{
						pointMass *other = &pool.d[j];

						vec<2> dist = id->pos - other->pos;
						vec<2> norm = dist / magnitude(dist);

						double acc = -G * id->m * other->m / dist.dotProduct(dist);

						id->v += norm * acc * td;
						other->v -= norm * acc * td;
					}
				}
				else
				{
					looped = LOOP;
					break;
				}
			}
		} while (++looped < LOOP);
		looped = 0;

		for (unsigned i = 0; i < pool.count; i++)
		{
			pointMass *id = &pool.d[i];

			glColor3d(id->rgb[0], id->rgb[1], id->rgb[2]);
			drawCircleFilled(cam.zoomPos + (id->pos + cam.movePos - cam.zoomPos) * cam.zoomMag, id->r * cam.zoomMag);
		}

		/* Draw Center of Mass*/
		glColor3d(1.f, 0.f, 1.f);
		drawCircleFilled(cam.zoomPos + (CoM + cam.movePos - cam.zoomPos) * cam.zoomMag, 0.01f * cam.zoomMag);

		/* Poll for and process events */
		glfwPollEvents();

		/* Handle inputs */

		//Mouse inputs
		state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
		if (state == GLFW_PRESS)
		{
			if (mouse.state == idle)
			{
				mouse.state = move;

				glfwGetCursorPos(window, &mouse.prev_pos[0], &mouse.prev_pos[1]);
				mouse.prev_pos[0] = mouse.prev_pos[0] / SCREEN_WIDTH * 2 - 1.f;
				mouse.prev_pos[1] = (-mouse.prev_pos[1] / SCREEN_HEIGHT * 2 + 1.f) / SCREEN_ASPECT;
			}
			if (mouse.state == move)
			{
				glfwGetCursorPos(window, &mouse.pos[0], &mouse.pos[1]);
				mouse.pos[0] = mouse.pos[0] / SCREEN_WIDTH * 2 - 1.f;
				mouse.pos[1] = (-mouse.pos[1] / SCREEN_HEIGHT * 2 + 1.f) / SCREEN_ASPECT;

				drawDisc(mouse.prev_pos, 0.01f, 0.02f, { 1.f, 1.f, 1.f, 1.f }, { 1.f, 1.f, 1.f, 1.f });
				drawArrow(mouse.prev_pos, mouse.pos, 0.8f);

				cam.movePos -= (mouse.pos - mouse.prev_pos) * mouse.sensitivity;
			}
		}
		else
		{
			if (mouse.state == move)
				mouse.state = idle;
		}

		//Keyboard inputs
		if (keyPressedOnce(GLFW_KEY_SPACE))
		{
			isPaused = !isPaused;
		}

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void scrollCallback(GLFWwindow *window, double xOffset, double yOffset)
{
	//Need to move funtion inside of main loop
	if (yOffset != 0.f)
	{
		cam.zoom(yOffset);
	}
}

void windowSizeCallback(GLFWwindow *window, int width, int height) {
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
	SCREEN_ASPECT = (double)width / (double)height;
	glViewport(0, 0, width, height);
}