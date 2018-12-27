#pragma once

#include "core.h"

void applyTerminalVelocity(object* ball);

double circularBodyTimeInstance(object* ball1, object* ball2);

double trajectory(double x, double xp, double t);

void circularBodyCollision(object *ball, object *other, double(*momentumTransfor)(double, double, double, double), double timeScales);