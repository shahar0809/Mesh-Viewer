#include "Camera.h"

Camera::Camera()
{
	projection_transformation = glm::mat4x4{
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 1
	};

	view_transformation = glm::mat4x4{
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
	};
}

Camera::~Camera()
{
	
}

const glm::mat4x4& Camera::GetProjectionTransformation() const
{
	return projection_transformation;
}

const glm::mat4x4& Camera::GetViewTransformation() const
{
	return view_transformation;
}

void Camera::SetOrthoTransformation(float left, float right, float bottum, float top, float nearParameter, float farParameter) {
	view_transformation[0][0] = 2 / (right - left);
	view_transformation[0][3] = -(right + left) / (right - left);
	view_transformation[1][1] = 2 / (top - bottum);
	view_transformation[1][3] = -(top + bottum) / (top - bottum);
	view_transformation[2][2] = 2 / (nearParameter - farParameter);
	view_transformation[2][3] = -(nearParameter + farParameter) / (nearParameter - farParameter);
}