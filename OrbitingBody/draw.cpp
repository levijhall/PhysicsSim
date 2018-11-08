#include "draw.h"

void drawCircleMode(float radius, float x, float y, uint sides, uint MODE)
{
	float degree, centralAngle;

	degree = 0.f;
	centralAngle = 2 * pi / (float)sides;

	glBegin(MODE);
	do
	{
		glVertex2f(radius*cos(degree) + x, radius*sin(degree) + y);
		degree += centralAngle;
	} while (degree < 2 * pi);

	glEnd();
}

void drawCircle(float radius, float x, float y, uint sides /*= 32*/)
{
	drawCircleMode(radius, x, y, sides, GL_LINE_LOOP);
}

void drawCircleFilled(float radius, float x, float y, uint sides /*= 32*/)
{
	drawCircleMode(radius, x, y, sides, GL_POLYGON);
}