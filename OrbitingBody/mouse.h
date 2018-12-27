#pragma once
#include "pch.h"

enum mouseStates
{
	idle,
	move,
	create,
	length
};

struct Mouse
{
	mouseStates state = idle;
	double sensitivity = 0.01f;
	vec<2> pos = { 0.f, 0.f };
	vec<2> prev_pos = { 0.f, 0.f };
};

void cursorPositionCallback(GLFWwindow *window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);