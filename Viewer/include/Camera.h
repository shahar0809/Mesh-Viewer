#pragma once
#include <glm/glm.hpp>
#include "Utils.h"
#include "CameraGUI_Attributes.h"

enum class CameraMode
{
	Orthographic,
	Perspective
};

class Camera
{
public:
	Camera();
	Camera(glm::vec3 eye, glm::vec3 at, glm::vec3 up);
	virtual ~Camera();

	void SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);

	glm::mat4x4 GetTransformation() const;
	const glm::mat4x4& GetProjectionTransformation() const;
	const glm::mat4x4& GetViewTransformation() const;

	const glm::vec3& getEye() const;
	const glm::vec3& getAt() const;
	const glm::vec3& getUp() const;

	glm::mat4x4 GetViewportTrans(unsigned int width, unsigned int height) const;

	void SetOrthoViewVolume(float left, float right, float bottom, float top);
	void SetDepth(float nearParameter, float farParameter);
	void SetPerspectiveViewVolume(float left, float right, float bottom, float top);
	void SetPerspectiveViewVolume(float aspect, float fovy);

	void SetOrthoCamera();
	void SetPerspectiveCamera();
	const CameraMode& GetCameraMode() const;

	// Apply transformations on both model and world (dependant on previous values)
	void ApplyLocalRotate(double rotateX, double rotateY, double rotateZ);
	void ApplyLocalTranslate(double transX, double transY, double transZ);

	void ApplyWorldRotate(double rotateX, double rotateY, double rotateZ);
	void ApplyWorldTranslate(double transX, double transY, double transZ);

	// Set transformations - without depending on previous values
	void SetLocalRotate(double rotateX, double rotateY, double rotateZ);
	void SetLocalTranslate(double transX, double transY, double transZ);

	void SetWorldRotate(double rotateX, double rotateY, double rotateZ);
	void SetWorldTranslate(double transX, double transY, double transZ);

	CameraGUI_Attributes gui;

private:
	CameraMode mode;

	glm::mat4x4 view_transformation;
	glm::mat4x4 projection_transformation;

	glm::vec3 Eye, At, Up;

	// View volume
	float zNear = 0, zFar = 0;
	// Ortho
	float left = 0, right = 0, top = 0, bottom = 0;
	// Perspective
	float fovy, aspect;

	// Attributes to keep translate and rotation values
	glm::mat4x4 TranslateLocal, RotateLocal, RotateLocalX, RotateLocalY, RotateLocalZ;
	glm::mat4x4 TranslateWorld, RotateWorld, RotateWorldX, RotateWorldY, RotateWorldZ;
	glm::mat4x4 WorldTrans, LocalTrans;

	// Store current rotate values (to avoid using arcsin, arccos)
	glm::vec3 LocalRotateVal, WorldRotateVal;

	void CalcOrthoTrans();
	void CalcPerspectiveTrans();
};
