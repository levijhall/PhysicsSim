#pragma once

typedef unsigned int uint;


#include <GLFW/glfw3.h>
#include <cmath>
#include <cstdio>

#include "matrix.h"

template<unsigned dim>
using vec = matrix<dim, 1>;

typedef struct object_s {
	vec<2> pos;
	vec<2> prev_pos;
	vec<2> v;
	float vt;
	float m;
	float r;
	vec<3> rgb;
} object;