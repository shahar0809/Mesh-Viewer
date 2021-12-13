#pragma once
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glad/glad.h>

class CameraGUI_Attributes
{
public:
	static int SCREEN_ASPECT;
	static int width;
	static int height;

	static const int ORTHO = 0, PERSPECTIVE = 1;

	static float CameraTransValue_array[3];
	static float CameraRotateValue_array[3];

	static float CameraWorldTransValue_array[3];
	static float CameraWorldRotateValue_array[3];

	static float ortho_array[3];
	static float perspective_array[3];
	static float eye_array[3];
	static float at_array[3];
	static float up_array[3];

	static int cameraMode;
	static float Fovy;
	static float aspect;
};

