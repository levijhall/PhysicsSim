#ifndef PCH_H
#define PCH_H

#include <cmath>
#include <cstdio>
#include <memory>

#include <GLFW/glfw3.h>

#include "../Matrix/matrix.h"

#include "../ParticlesInABox/draw.h"
#include "../ParticlesInABox/laws.h"
#include "../ParticlesInABox/collisions.h"

//#include "../OrbitingBody/camera.h"
//#include "../OrbitingBody/mouse.h"
//#include "../OrbitingBody/instancePool.h"


template<unsigned dim>
using vec = matrix<dim, 1>;

struct pointMass {
	vec<2> pos;
	vec<2> prev_pos;
	vec<2> v;
	double vt;
	double m;
	double r;
	vec<3> rgb;
};

#endif //PCH_H
