#pragma once

#include "core.h"

//values
const float g = -0.0001f;
const float timeScale = 1.f;

//returns the new v1 value
float ellasticCollision(float m1, float v1, float m2, float v2);

//returns the new v1 value
float inellasticCollision(float m1, float v1, float m2, float v2);