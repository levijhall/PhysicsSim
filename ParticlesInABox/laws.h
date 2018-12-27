#pragma once

#include "core.h"

//values
const double g = -0.0001f * 0.f;

//returns the new v1 value
double ellasticCollision(double m1, double v1, double m2, double v2);

//returns the new v1 value
double inellasticCollision(double m1, double v1, double m2, double v2);