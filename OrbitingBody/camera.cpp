/*  camera.cpp
 *  Allow the movement of the camera
 */

typedef struct Camera_s
{
	float x = 0;
	float y = 0;
	float width;
	float height;
	float zoom;
} Camera;