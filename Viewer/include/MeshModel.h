#pragma once
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <string>
#include <memory>
#include "Face.h"

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
	void ApplyModelRotate(double rotateVal);
	void ApplyModelTranslate(double transX, double transY, double transZ);

	void ApplyWorldScale(double scaleX, double scaleY, double scaleZ);
	void ApplyWorldRotate(double rotateVal);
	void ApplyWorldTranslate(double transX, double transY, double transZ);

	// Set transformations - without depending on previous values
	void SetModelScale(double scaleX, double scaleY, double scaleZ);
	void SetModelRotate(double rotateVal);
	void SetModelTranslate(double transX, double transY, double transZ);

	void SetWorldScale(double scaleX, double scaleY, double scaleZ);
	void SetWorldRotate(double rotateVal);
	void SetWorldTranslate(double transX, double transY, double transZ);

	glm::mat4x4 GetTransformation() const;

private:
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::string model_name;

	glm::mat4x4 ScaleModel, TranslateModel, RotateModel;
	glm::mat4x4 ScaleWorld, TranslateWorld, RotateWorld;

	// Store current rotate values (to avoid using arcsin, arccos)
	double ModelRotateVal, WorldRotateVal;
};
