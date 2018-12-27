#pragma once
#include "pch.h"

//Must be set via glfwSetKeyCallback() in order to use helper functions
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

//Precondition: keyCallback has been set within glfwSetKeyCallback() in the main thread.
//returns true or false
int keyPressedOnce(int key);
int keyReleasedOnce(int key);
int keyPressed(int key);
int keyReleased(int key);