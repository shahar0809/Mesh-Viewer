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

	glm::vec3 DiffuseReflectionColor;
	glm::vec3 SpecularReflectionColor;
	glm::vec3 AmbientReflectionColor;

	// GUI controllers
	static float ModelScaleValue_array[3];
	static float ModelTransValue_array[3];
	static float ModelRotateValue_array[3];

	static float WorldScaleValue_array[3];
	static float WorldTransValue_array[3];
	static float WorldRotateValue_array[3];

	/* Fields to draw attributes on Mesh Viewer */
	bool IsOnScreen = false;
	bool IsBoundingBoxOnScreen = false;
	bool IsFrameOnScreen = false;
	bool AreFaceNormalsOnScreen = false;
	bool AreVerticesNormalsOnScreen = false;
	bool IsBoundingRectOnScreen = false;
	bool Grayscale = false;
};

