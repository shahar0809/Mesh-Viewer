#pragma once
#include <glm/glm.hpp>
#include "Utils.h"

class Camera
{
public:
	Camera();
	virtual ~Camera();

	void SetCameraLookAt(const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up);

	const glm::mat4x4& GetProjectionTransformation() const;
	const glm::mat4x4& GetViewTransformation() const;

	const glm::vec4& getEye() const;
	const glm::vec4& getAt() const;
	const glm::vec4& getUp() const;

	// Apply transformations on both model and world (dependant on previous values)
	void ApplyLocalScale(double scaleX, double scaleY, double scaleZ);
	void ApplyLocalRotate(double rotateX, double rotateY, double rotateZ);
	void ApplyLocalTranslate(double transX, double transY, double transZ);

	void ApplyWorldScale(double scaleX, double scaleY, double scaleZ);
	void ApplyWorldRotate(double rotateX, double rotateY, double rotateZ);
	void ApplyWorldTranslate(double transX, double transY, double transZ);

	// Set transformations - without depending on previous values
	void SetLocalScale(double scaleX, double scaleY, double scaleZ);
	void SetLocalRotate(double rotateX, double rotateY, double rotateZ);
	void SetLocalTranslate(double transX, double transY, double transZ);

	void SetWorldScale(double scaleX, double scaleY, double scaleZ);
	void SetWorldRotate(double rotateX, double rotateY, double rotateZ);
	void SetWorldTranslate(double transX, double transY, double transZ);

private:
	glm::mat4x4 view_transformation;
	glm::mat4x4 projection_transformation;
	glm::mat4x4 camera;
	glm::mat4x4 camera_inverse;

	glm::vec4 Eye, At, Up;

	glm::mat4x4 ScaleLocal, TranslateLocal, RotateLocal, RotateLocalX, RotateLocalY, RotateLocalZ;
	glm::mat4x4 ScaleWorld, TranslateWorld, RotateWorld, RotateWorldX, RotateWorldY, RotateWorldZ;

	glm::mat4x4 WorldTrans, LocalTrans;

	// Store current rotate values (to avoid using arcsin, arccos)
	glm::vec3 LocalRotateVal, WorldRotateVal;

	void CalcViewTrans();
	void OrthographicProj(float left, float right, float top, float bottom);
};
