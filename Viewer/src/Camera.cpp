#include "Camera.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>

Camera::Camera()
{
	aspect = 1.0f;
	fovy = 30.0f;

	projection_transformation = glm::mat4x4{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	view_transformation = glm::mat4x4{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	Eye = glm::vec4(0, 0, 0, 1);
	At = glm::vec4(0, 0, 1, 0);
	Up = glm::vec4(0, 1, 0, 1);
}

Camera::~Camera()
{
	
}

const glm::mat4x4& Camera::GetProjectionTransformation() const
{
	//std::cout << "projection camera " << std::endl;
	//std::cout << glm::to_string(projection_transformation) << std::endl;
	return projection_transformation;
}

const glm::mat4x4& Camera::GetViewTransformation() const
{
	//std::cout << "view trans " << std::endl;
	//std::cout << glm::to_string(view_transformation) << std::endl;
	return view_transformation;
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
	//camera = RotateLocal * camera;
	//camera_inverse = camera_inverse * glm::inverse(RotateLocal);
}

void Camera::ApplyLocalTranslate(double transX, double transY, double transZ)
{
	// Add current translation parameters (in last column) by new parameters
	TranslateLocal[3][0] += transX;
	TranslateLocal[3][1] += transY;
	TranslateLocal[3][2] += transZ;

	//camera = TranslateLocal * camera;
	//camera_inverse = camera_inverse * glm::inverse(TranslateLocal);
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

	//camera = RotateWorld * camera;
	//camera_inverse = camera_inverse * glm::inverse(RotateWorld);
}

void Camera::ApplyWorldTranslate(double transX, double transY, double transZ)
{
	// Add current translation parameters (in last column) by new parameters
	TranslateLocal[3][0] += transX;
	TranslateLocal[3][1] += transY;
	TranslateLocal[3][2] += transZ;

	//camera = TranslateLocal * camera;
	//camera_inverse = camera_inverse * glm::inverse(TranslateLocal);
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

	//camera = RotateLocal * camera;
	//camera_inverse = camera_inverse * glm::inverse(RotateLocal);
}

void Camera::SetLocalTranslate(double transX, double transY, double transZ)
{
	// Set current translation parameters (in last column) to be new parameters
	TranslateLocal[3][0] = transX;
	TranslateLocal[3][1] = transY;
	TranslateLocal[3][2] = transZ;

	//camera = TranslateLocal * camera;
	//camera_inverse = camera_inverse * glm::inverse(TranslateLocal);
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

	//camera = RotateWorld * camera;
	//camera_inverse = camera_inverse * glm::inverse(RotateWorld);
}

void Camera::SetWorldTranslate(double transX, double transY, double transZ)
{
	// Set current translation parameters (in last column) to be new parameters
	TranslateWorld[3][0] = transX;
	TranslateWorld[3][1] = transY;
	TranslateWorld[3][2] = transZ;

	//camera = TranslateWorld * camera;
	//camera_inverse = camera_inverse * glm::inverse(TranslateWorld);
}

void Camera::SetOrthoCamera()
{
	mode = CameraMode::Orthographic;
}

void Camera::SetPerspectiveCamera()
{
	mode = CameraMode::Perspective;
}

const CameraMode& Camera::GetCameraMode() const
{
	return mode;
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

	view_transformation = glm::mat4x4(x, y, z, glm::vec4(0, 0, 0, 1));
	SetLocalTranslate(-eye.x, -eye.y, -eye.z);
}

void Camera::SetOrthoViewVolume(float left, float right, float bottom, float top)
{
	this->right = right;
	this->left = left;
	this->top = top;
	this->bottom = bottom;	

	CalcOrthoTrans();
}

void Camera::SetDepth(float nearParameter, float farParameter)
{
	this->zNear = nearParameter;
	this->zFar = farParameter;
}

void Camera::SetPerspectiveViewVolume(float fovy, float aspect)
{
	this->fovy = fovy;
	this->aspect = aspect;
	CalcPerspectiveTrans();
}

void Camera::CalcOrthoTrans()
{
	projection_transformation[0][0] = 2 / (right - left);	
	projection_transformation[1][1] = 2 / (top - bottom);
	projection_transformation[2][2] = 2 / (zNear - zFar);
	projection_transformation[3][3] = 1;
	projection_transformation[3][0] = -(right + left) / (right - left);
	projection_transformation[3][1] = -(top + bottom) / (top - bottom);
	projection_transformation[3][2] = -(zFar + zNear) / (zFar - zNear);
}

void Camera::CalcPerspectiveTrans()
{
	//std::cout << "arg " << (fovy * M_PI / 180) / 2.0f << std::endl;
	//std::cout << "angle " << tan((fovy * M_PI / 180) / 2.0f) << std::endl;
	//float scale = 1.0f / tan((fovy * M_PI / 180) / 2.0f);
	//std::cout << "scale " << scale << std::endl;
	//float rangeInv = 1 / (zNear - zFar);
	//std::cout << "range " << rangeInv << std::endl;

	//float alpha = (zNear + zFar) * rangeInv,
	//	beta = 2 * zNear * zFar * rangeInv;

	//std::cout << "alpha " << alpha << std::endl;
	//std::cout << "beta " << beta << std::endl;


	//projection_transformation = glm::mat4x4{
	//	scale / aspect, 0, 0, 0,
	//	0, scale, 0, 0,
	//	0, 0, alpha, -1,
	//	0, 0, beta, 0
	//};
	//std::cout << "perspective trans " << std::endl;
	//std::cout << glm::to_string(projection_transformation) << std::endl;
	projection_transformation = glm::perspective(fovy, aspect, zNear, zFar);
}

glm::mat4x4 Camera::GetViewportTrans(unsigned int width, unsigned int height) const
{
	glm::mat4x4 scaleTrans {
		width / 2.0f, 0, 0, 0,
		0, height / 2.0f, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	glm::mat4x4 translateTran {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		1, 1, 1, 1
	};

	return scaleTrans * translateTran;
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
	glm::mat4x4 LocalTrans = TranslateLocal * RotateLocal;
	glm::mat4x4 WorldTrans = TranslateWorld * RotateWorld;

	return WorldTrans * LocalTrans;
}