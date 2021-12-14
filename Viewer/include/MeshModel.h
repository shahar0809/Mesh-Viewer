#pragma once
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glad/glad.h>
#include <string>
#include <tuple>
#include <memory>
#include "Face.h"
#include "ModelGUI_Attributes.h"

#define M_PI 3.14159265358979323846264338327950288

class MeshModel
{
public:
	MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name);
	virtual ~MeshModel();
	const Face& GetFace(int index) const;
	int GetFacesCount() const;
	const glm::vec3& GetVertice(int index) const;
	const glm::vec3& GetNormal(int index) const;
	int GetVerticesCount() const;
	int GetNormalsCount() const;
	const std::string& GetModelName() const;
	const std::vector<glm::vec3> GetVertices() const;

	std::vector<glm::vec3> getBoundingBox() const;

	const glm::vec3& GetOrigin() const;
	const glm::vec3& GetAxisX() const;
	const glm::vec3& GetAxisY() const;
	const glm::vec3& GetAxisZ() const;

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
	glm::mat4x4 GetWorldTransformation() const;

	void InitLocalFrame();
	void InitverticesFacesNeighbors();
	
	glm::vec3 GetNormalVertix(int index) const;
	glm::vec3 GetFaceNormal(int index) const;

	glm::vec3 GetFaceCenter(const Face& face) const;
	std::vector<glm::vec3> GetBoundingRectangle(std::vector<glm::vec3> vectors) const;

	static std::pair<std::tuple<double, double, double>, std::tuple<double, double, double>> GetMinMax(std::vector<glm::vec3> vertices);
	ModelGUI_Attributes gui;

private:
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<std::vector<int>> verticesFacesNeighbors;
	std::string model_name;

	// Keep the frame of the model
	glm::vec3 Origin, AxisX, AxisY, AxisZ;

	glm::mat4x4 ScaleModel, TranslateModel, RotateModel, RotateModelX, RotateModelY, RotateModelZ;
	glm::mat4x4 ScaleWorld, TranslateWorld, RotateWorld, RotateWorldX, RotateWorldY, RotateWorldZ;

	glm::mat4x4 WorldTrans, ModelTrans;

	// Store current rotate values (to avoid using arcsin, arccos)
	glm::vec3 ModelRotateVal, WorldRotateVal;

	static double ToRadians(double value);
	static double ToDegrees(double value);

	static glm::vec4 ToHomogCoords(glm::vec3 vec);
	static glm::vec3 FromHomogCoords(glm::vec4 vec);

	void TransformModelFrame(glm::mat4x4 trans);

};