#include "draw.h"

void drawCircleMode(vec<2> center, double radius, unsigned sides, unsigned MODE)
{
	double degree, centralAngle;

	degree = 0.f;
	centralAngle = 2 * pi / (double)sides;

	glBegin(MODE);
	do
	{
		glVertex2d(radius*cos(degree) + center[0], radius*sin(degree) + center[1]);
		degree += centralAngle;
	} while (degree < 2 * pi);

	glEnd();
}

void drawCircle(vec<2> center, double radius, unsigned sides /*= 32*/)
{
	drawCircleMode(center, radius, sides, GL_LINE_LOOP);
}

void drawCircleFilled(vec<2> center, double radius, unsigned sides /*= 32*/)
{
	drawCircleMode(center, radius, sides, GL_POLYGON);
}

void drawDisc(vec<2> center, double r, double R, vec<4> color1, vec<4> color2, unsigned sides, double rotation)
{
	double xVertex, yVertex;
	double degree, centralAngle;

	xVertex = 0.f;
	yVertex = 0.f;
	degree = 0.f;
	centralAngle = 2 * pi / (double)sides;

	glBegin(GL_TRIANGLE_STRIP);
	for (unsigned n = 0; n <= sides; n++)
	{
		degree = n * centralAngle + rotation;
		xVertex = center[0] + r * cos(degree);
		yVertex = center[1] + r * sin(degree);
		glColor4d(color1[0], color1[1], color1[2], color1[3]);
		glVertex2d(xVertex, yVertex);
		xVertex = center[0] + R * cos(degree);
		yVertex = center[1] + R * sin(degree);
		glColor4d(color2[0], color2[1], color2[2], color2[3]);
		glVertex2d(xVertex, yVertex);
	}
	glEnd();
}

void drawArrow(vec<2> begin, vec<2> end, double size)
{
	glBegin(GL_LINES);
	glVertex2d(begin[0], begin[1]);
	glVertex2d(end[0], end[1]);
	glEnd();

	//TO-DO: Rotate arrow without stretching
	vec<2> norm = (end - begin) / magnitude(end - begin);
	vec<2> perp = { -norm[1], norm[0] };

	matrix<2> rotation = concatColumns(norm, perp);

	vec<2> leftArrow{ -0.05f * size, 0.025f * size };
	vec<2> rightArrow{ -0.05f * size, -0.025f * size };

	leftArrow = rotation * leftArrow;
	rightArrow = rotation * rightArrow;

	glBegin(GL_POLYGON);
	glVertex2d(end[0], end[1]);
	glVertex2d(end[0] + leftArrow[0], end[1] + leftArrow[1]);
	glVertex2d(end[0] + rightArrow[0], end[1] + rightArrow[1]);
	glEnd();
}