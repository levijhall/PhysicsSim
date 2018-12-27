#pragma once

#include <GLFW/glfw3.h>
#include <cmath>
#include <cstdio>

#include "../Matrix/matrix.h"

template<unsigned dim>
using vec = matrix<dim, 1>;

struct object {
	vec<2> pos;
	vec<2> prev_pos;
	vec<2> v;
	double vt;
	double m;
	double r;
	vec<3> rgb;
};