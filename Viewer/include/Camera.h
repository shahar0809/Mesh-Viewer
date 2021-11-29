#pragma once
#include <glm/glm.hpp>
#include "Utils.h"

class Camera
{
public:
	Camera();
	virtual ~Camera();

	void SetCameraLookAt(const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up);

	glm::mat4x4 GetTransformation() const;
	const glm::mat4x4& GetProjectionTransformation() const;
	const glm::mat4x4& GetViewTransformation() const;

	const glm::vec4& getEye() const;
	const glm::vec4& getAt() const;
	const glm::vec4& getUp() const;

	glm::vec3 GetViewportTrans(glm::vec3 vec, unsigned int width, unsigned int height) const;

	void SetOrthoTrans(float left, float right, float bottum, float top, float nearParameter, float farParameter);
	glm::mat4x4 GetOrthoTrans() const;

	const glm::mat4x4& GetCameraInverse() const;

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

private:
	glm::mat4x4 view_transformation;			// Orthographic transf
	glm::mat4x4 projection_transformation;		// Projection tranformation
	glm::mat4x4 camera;							// C
	glm::mat4x4 camera_inverse;					// Cinv

	glm::vec4 Eye, At, Up;

	// Attributes to keep translate and rotation values
	glm::mat4x4 TranslateLocal, RotateLocal, RotateLocalX, RotateLocalY, RotateLocalZ;
	glm::mat4x4 TranslateWorld, RotateWorld, RotateWorldX, RotateWorldY, RotateWorldZ;

	glm::mat4x4 WorldTrans, LocalTrans;

	// Store current rotate values (to avoid using arcsin, arccos)
	glm::vec3 LocalRotateVal, WorldRotateVal;

	void CalcViewTrans();
};
