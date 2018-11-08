#include "draw.h"

typedef struct object_s {
	float x;
	float y;
	float vx;
	float vy;
	float m;
	float r;
} object;

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

	object ball = { 0.f, -0.75f, 0.007f, 0.f, 1.f, 0.1f };
	const float g = 0.0001f;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClearColor(0.f, 0.f, 0.f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
		glLoadIdentity(); //Reset the drawing perspective

		ball.x += ball.vx;
		ball.y += ball.vy;

		if (ball.vy < 0.1f)
			ball.vy += ball.m * g;
		if (ball.vy > 0.1f)
			ball.vy = 0.1f;

		if (ball.y > 1.f - ball.r)
		{
			ball.y = 1.f - ball.r;
			ball.vy = -ball.vy / 1.1;
		}

		if (ball.x > 1.f - ball.r)
		{
			ball.x = 1.f - ball.r;
			ball.vx = -ball.vx / 1.1;
		}
		if (ball.x < -1.f + ball.r)
		{
			ball.x = -1.f + ball.r;
			ball.vx = -ball.vx / 1.1;
		}	

		glColor3f(1.f, 1.f, 1.f);
		drawCircleFilled(0.1f, ball.x, ball.y);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}