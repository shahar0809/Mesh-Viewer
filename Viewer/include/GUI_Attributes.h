#pragma once
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glad/glad.h>

class GUI_Attributes
{
public:
	// Colors
	glm::vec3 color;
	glm::vec3 BoundingBoxColor;
	glm::vec3 FaceNormalsColor;
	glm::vec3 VerticsNormalsColor;
	glm::vec3 BoundingRectColor;

	/* Fields to draw attributes on Mesh Viewer */
	bool IsOnScreen = true;
	bool IsBoundingBoxOnScreen = false;
	bool IsFrameOnScreen = false;
	bool AreFaceNormalsOnScreen = false;
	bool AreVerticesNormalsOnScreen = false;
	bool IsBoundingRectOnScreen = false;
};

