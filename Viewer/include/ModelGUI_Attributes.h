#pragma once
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glad/glad.h>

class ModelGUI_Attributes
{
public:
	// Colors
	glm::vec3 color;
	glm::vec3 BoundingBoxColor;
	glm::vec3 FaceNormalsColor;
	glm::vec3 VerticsNormalsColor;
	glm::vec3 BoundingRectColor;

	glm::vec3 DiffuseReflectionColor = { 1, 1, 1 };
	glm::vec3 SpecularReflectionColor = { 1, 1, 1 };
	glm::vec3 AmbientReflectionColor = { 1, 1, 1 };

	// GUI controllers
	float ModelScaleValue_array[3] = { 1, 1, 1 };
	float ModelTransValue_array[3] = { 0, 0, 0 };
	float ModelRotateValue_array[3] = { 0, 0, 0 };

	float WorldScaleValue_array[3] = { 1, 1, 1 };
	float WorldTransValue_array[3] = { 0, 0, 0 };
	float WorldRotateValue_array[3] = { 0, 0, 0 };

	/* Fields to draw attributes on Mesh Viewer */
	bool IsOnScreen = false;
	bool IsBoundingBoxOnScreen = false;
	bool IsFrameOnScreen = false;
	bool AreFaceNormalsOnScreen = false;
	bool AreVerticesNormalsOnScreen = false;
	bool IsBoundingRectOnScreen = false;
	bool Grayscale = false;
};

