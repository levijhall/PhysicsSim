// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

#ifndef PCH_H
#define PCH_H

#include <cmath>
#include <cstdio>

#include <GLFW/glfw3.h>

#include "../Matrix/matrix.h"

template<unsigned dim>
using vec = matrix<dim, 1>;

struct pointMass {
	vec<2> pos;
	vec<2> prev_pos;
	vec<2> v;
	float vt;
	float m;
	float r;
	vec<3> rgb;
};

#endif //PCH_H
