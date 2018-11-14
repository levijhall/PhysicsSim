#pragma once

typedef unsigned int uint;

#include <GLFW/glfw3.h>
#include <cmath>
#include <cstdio>

typedef struct object_s {
	float x, y;
	float xp, yp;
	float vx, vy, vt;
	float m;
	float r;
	float red, green, blue;
} object;