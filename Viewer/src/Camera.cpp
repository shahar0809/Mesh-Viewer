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

void Camera::ApplyLocalScale(double scaleX, double scaleY, double scaleZ)
{
	// Multiply current scale parameters (in diagonal) by new parameters
	ScaleLocal[0][0] *= scaleX;
	ScaleLocal[1][1] *= scaleY;
	ScaleLocal[2][2] *= scaleZ;

	camera = ScaleLocal * camera;
	camera_inverse = camera_inverse * glm::inverse(ScaleLocal);
}

void Camera::ApplyLocalRotate(double rotateX, double rotateY, double rotateZ)
{
	LocalRotateVal.x += rotateX;
	LocalRotateVal.y += rotateY;
	LocalRotateVal.z += rotateZ;

	double radiansValX = Utils::ToRadians(LocalRotateVal.x);

	RotateLocalX[1][1] = cos(radiansValX);
	RotateLocalX[1][2] = -sin(radiansValX);
	RotateLocalX[2][1] = sin(radiansValX);
	RotateLocalX[2][2] = cos(radiansValX);

	double radiansValY = Utils::ToRadians(LocalRotateVal.y);

	RotateLocalY[0][0] = cos(radiansValY);
	RotateLocalY[0][2] = sin(radiansValY);
	RotateLocalY[2][0] = -sin(radiansValY);
	RotateLocalY[2][2] = cos(radiansValY);

	double radiansValZ = Utils::ToRadians(LocalRotateVal.z);

	RotateLocalZ[0][0] = cos(radiansValZ);
	RotateLocalZ[1][0] = -sin(radiansValZ);
	RotateLocalZ[0][1] = sin(radiansValZ);
	RotateLocalZ[1][1] = cos(radiansValZ);

	RotateLocal = RotateLocalZ * RotateLocalY * RotateLocalX;
	camera = RotateLocal * camera;
	camera_inverse = camera_inverse * glm::inverse(RotateLocal);
}

void Camera::ApplyLocalTranslate(double transX, double transY, double transZ)
{
	// Add current translation parameters (in last column) by new parameters
	TranslateLocal[3][0] += transX;
	TranslateLocal[3][1] += transY;
	TranslateLocal[3][2] += transZ;

	camera = TranslateLocal * camera;
	camera_inverse = camera_inverse * glm::inverse(TranslateLocal);
}

void Camera::ApplyWorldScale(double scaleX, double scaleY, double scaleZ)
{
	// Multiply current scale parameters (in diagonal) by new parameters
	ScaleWorld[0][0] *= scaleX;
	ScaleWorld[1][1] *= scaleY;
	ScaleWorld[2][2] *= scaleZ;

	camera = ScaleWorld * camera;
	camera_inverse = camera_inverse * glm::inverse(ScaleWorld);
}

void Camera::ApplyWorldRotate(double rotateX, double rotateY, double rotateZ)
{
	WorldRotateVal.x += rotateX;
	WorldRotateVal.y += rotateY;
	WorldRotateVal.z += rotateZ;

	double radiansValX = Utils::ToRadians(WorldRotateVal.x);

	RotateWorldX[1][1] = cos(radiansValX);
	RotateWorldX[1][2] = -sin(radiansValX);
	RotateWorldX[2][1] = sin(radiansValX);
	RotateWorldX[2][2] = cos(radiansValX);

	double radiansValY = Utils::ToRadians(WorldRotateVal.y);

	RotateWorldY[0][0] = cos(radiansValY);
	RotateWorldY[0][2] = sin(radiansValY);
	RotateWorldY[2][0] = -sin(radiansValY);
	RotateWorldY[2][2] = cos(radiansValY);

	double radiansValZ = Utils::ToRadians(WorldRotateVal.z);

	RotateWorldZ[0][0] = cos(radiansValZ);
	RotateWorldZ[1][0] = -sin(radiansValZ);
	RotateWorldZ[0][1] = sin(radiansValZ);
	RotateWorldZ[1][1] = cos(radiansValZ);

	RotateWorld = RotateWorldZ * RotateWorldY * RotateWorldX;

	camera = RotateWorld * camera;
	camera_inverse = camera_inverse * glm::inverse(RotateWorld);
}

void Camera::ApplyWorldTranslate(double transX, double transY, double transZ)
{
	// Add current translation parameters (in last column) by new parameters
	TranslateLocal[3][0] += transX;
	TranslateLocal[3][1] += transY;
	TranslateLocal[3][2] += transZ;

	camera = TranslateLocal * camera;
	camera_inverse = camera_inverse * glm::inverse(TranslateLocal);
}

void Camera::SetLocalScale(double scaleX, double scaleY, double scaleZ)
{
	// Set current scale parameters (in diagonal) to be new parameters
	ScaleLocal[0][0] = scaleX;
	ScaleLocal[1][1] = scaleY;
	ScaleLocal[2][2] = scaleZ;

	camera = ScaleLocal * camera;
	camera_inverse = camera_inverse * glm::inverse(ScaleLocal);
}

void Camera::SetLocalRotate(double rotateX, double rotateY, double rotateZ)
{
	LocalRotateVal.x = rotateX;
	LocalRotateVal.y = rotateY;
	LocalRotateVal.z = rotateZ;

	double radiansValX = Utils::ToRadians(LocalRotateVal.x);

	RotateLocalX[1][1] = cos(radiansValX);
	RotateLocalX[1][2] = -sin(radiansValX);
	RotateLocalX[2][1] = sin(radiansValX);
	RotateLocalX[2][2] = cos(radiansValX);

	double radiansValY = Utils::ToRadians(LocalRotateVal.y);

	RotateLocalY[0][0] = cos(radiansValY);
	RotateLocalY[0][2] = sin(radiansValY);
	RotateLocalY[2][0] = -sin(radiansValY);
	RotateLocalY[2][2] = cos(radiansValY);

	double radiansValZ = Utils::ToRadians(LocalRotateVal.z);

	RotateLocalZ[0][0] = cos(radiansValZ);
	RotateLocalZ[1][0] = -sin(radiansValZ);
	RotateLocalZ[0][1] = sin(radiansValZ);
	RotateLocalZ[1][1] = cos(radiansValZ);

	RotateLocal = RotateLocalZ * RotateLocalY * RotateLocalX;

	camera = RotateLocal * camera;
	camera_inverse = camera_inverse * glm::inverse(RotateLocal);
}

void Camera::SetLocalTranslate(double transX, double transY, double transZ)
{
	// Set current translation parameters (in last column) to be new parameters
	TranslateLocal[3][0] = transX;
	TranslateLocal[3][1] = transY;
	TranslateLocal[3][2] = transZ;

	camera = TranslateLocal * camera;
	camera_inverse = camera_inverse * glm::inverse(TranslateLocal);
}

void Camera::SetWorldScale(double scaleX, double scaleY, double scaleZ)
{
	// Set current scale parameters (in diagonal) to be new parameters
	ScaleWorld[0][0] = scaleX;
	ScaleWorld[1][1] = scaleY;
	ScaleWorld[2][2] = scaleZ;

	camera = ScaleWorld * camera;
	camera_inverse = camera_inverse * glm::inverse(ScaleWorld);
}

void Camera::SetWorldRotate(double rotateX, double rotateY, double rotateZ)
{
	WorldRotateVal.x = rotateX;
	WorldRotateVal.y = rotateY;
	WorldRotateVal.z = rotateZ;

	double radiansValX = Utils::ToRadians(WorldRotateVal.x);

	RotateWorldX[1][1] = cos(radiansValX);
	RotateWorldX[1][2] = -sin(radiansValX);
	RotateWorldX[2][1] = sin(radiansValX);
	RotateWorldX[2][2] = cos(radiansValX);

	double radiansValY = Utils::ToRadians(WorldRotateVal.y);

	RotateWorldY[0][0] = cos(radiansValY);
	RotateWorldY[0][2] = sin(radiansValY);
	RotateWorldY[2][0] = -sin(radiansValY);
	RotateWorldY[2][2] = cos(radiansValY);

	double radiansValZ = Utils::ToRadians(WorldRotateVal.z);

	RotateWorldZ[0][0] = cos(radiansValZ);
	RotateWorldZ[1][0] = -sin(radiansValZ);
	RotateWorldZ[0][1] = sin(radiansValZ);
	RotateWorldZ[1][1] = cos(radiansValZ);

	RotateWorld = RotateWorldZ * RotateWorldY * RotateWorldX;

	camera = RotateWorld * camera;
	camera_inverse = camera_inverse * glm::inverse(RotateWorld);
}

void Camera::SetWorldTranslate(double transX, double transY, double transZ)
{
	// Set current translation parameters (in last column) to be new parameters
	TranslateWorld[3][0] = transX;
	TranslateWorld[3][1] = transY;
	TranslateWorld[3][2] = transZ;

	camera = TranslateWorld * camera;
	camera_inverse = camera_inverse * glm::inverse(TranslateWorld);
}

/**
 * @brief Sets the look-at of a camera given the 3 vectors.
 * @param eye The position of the camera
 * @param at The direction the camera is looking to
 * @param up The direction of the Y coordinate
*/
void Camera::SetCameraLookAt(const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up)
{
	//glm::lookAt(eye, at, up)
	Eye = eye; At = at; Up = up;

	glm::vec4 z = glm::normalize(at - eye);
	glm::vec4 x = glm::normalize(glm::vec4(glm::cross(glm::vec3(up), glm::vec3(z)), 1.0f));
	glm::vec4 y = glm::normalize(glm::vec4(glm::cross(glm::vec3(z), glm::vec3(x)), 1.0f));

	camera = glm::mat4x4(x, y, z, glm::vec4(0, 0, 0, 1));
	SetLocalTranslate(-eye.x, -eye.y, -eye.z);
}

void Camera::SetOrthoTransformation(float left, float right, float bottum, float top, float nearParameter, float farParameter) {
	view_transformation[0][0] = 2 / (right - left);
	view_transformation[0][3] = -(right + left) / (right - left);
	view_transformation[1][1] = 2 / (top - bottum);
	view_transformation[1][3] = -(top + bottum) / (top - bottum);
	view_transformation[2][2] = 2 / (nearParameter - farParameter);
	view_transformation[2][3] = -(nearParameter + farParameter) / (nearParameter - farParameter);
}

void Camera::CalcViewTrans()
{

}

const glm::vec4& Camera::getEye() const
{
    return Eye;
}

const glm::vec4& Camera::getAt() const
{
    return At;
}

const glm::vec4& Camera::getUp() const
{
    return Up;
}

glm::mat4x4 Camera::GetTransformation() const
{
	glm::mat4x4 LocalTrans = TranslateLocal * RotateLocal * ScaleLocal;
	glm::mat4x4 WorldTrans = TranslateWorld * RotateWorld * ScaleWorld;

	return WorldTrans * LocalTrans;
}

const glm::mat4x4& Camera::GetCameraInverse() const
{
	return camera_inverse;
}