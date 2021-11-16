#pragma once
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/euler_angles.hpp>
#include <string>
#include <memory>
#include "Face.h"

#define M_PI 3.14159265358979323846264338327950288

class MeshModel
{
public:
	MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name);
	virtual ~MeshModel();
	const Face& GetFace(int index) const;
	int GetFacesCount() const;
	const glm::vec3& GetVertice(int index) const;
	int GetVerticesCount() const;
	const std::string& GetModelName() const;

	const std::vector<glm::vec3> GetVertices() const;

	// Apply transformations on both model and world (dependant on previous values)
	void ApplyModelScale(double scaleX, double scaleY, double scaleZ);
	void ApplyModelRotate(double rotateX, double rotateY, double rotateZ);
	void ApplyModelTranslate(double transX, double transY, double transZ);

	void ApplyWorldScale(double scaleX, double scaleY, double scaleZ);
	void ApplyWorldRotate(double rotateX, double rotateY, double rotateZ);
	void ApplyWorldTranslate(double transX, double transY, double transZ);

	// Set transformations - without depending on previous values
	void SetModelScale(double scaleX, double scaleY, double scaleZ);
	void SetModelRotate(double rotateX, double rotateY, double rotateZ);
	void SetModelTranslate(double transX, double transY, double transZ);

	void SetWorldScale(double scaleX, double scaleY, double scaleZ);
	void SetWorldRotate(double rotateX, double rotateY, double rotateZ);
	void SetWorldTranslate(double transX, double transY, double transZ);

	glm::mat4x4 GetTransformation() const;

	float GetFirstScaleValue() const;
	float GetFirstTransValueX() const;
	float GetFirstTransValueY() const;
	void SetFirstScaleValue(float ScaleValue);
	void SetFirstTransValueX(float TransValueX);
	void SetFirstTransValueY(float TransValueY);

	glm::vec3 color;
	bool IsOnScreen = false;

private:
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::string model_name;

	glm::mat4x4 ScaleModel, TranslateModel, RotateModel, RotateModelX, RotateModelY, RotateModelZ;
	glm::mat4x4 ScaleWorld, TranslateWorld, RotateWorld, RotateWorldX, RotateWorldY, RotateWorldZ;

	glm::mat4x4 WorldTrans, ModelTrans;

	// Store current rotate values (to avoid using arcsin, arccos)
	glm::vec3 ModelRotateVal, WorldRotateVal;

	float FirstScaleValue;
	float FirstTransValueX;
	float FirstTransValueY;

	static double ToRadians(double value);
	static double ToDegrees(double value);
};