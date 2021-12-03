#include "MeshModel.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>

MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name) :
	faces(faces),
	vertices(vertices),
	normals(normals),
	verticesFacesNeighbors(vertices.size())
{
	WorldRotateVal = glm::vec3(0);
	ModelRotateVal = glm::vec3(0);

	TranslateModel = glm::mat4x4{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	TranslateWorld = glm::mat4x4{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	ScaleModel = glm::mat4x4{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	ScaleWorld = glm::mat4x4{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	RotateModelX = glm::mat4x4{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	RotateModelY = glm::mat4x4{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	RotateModelZ = glm::mat4x4{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};


	RotateModel = glm::mat4x4{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	RotateModelX = glm::mat4x4{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	RotateWorldY = glm::mat4x4{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	RotateWorldZ = glm::mat4x4{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	RotateWorld = glm::mat4x4{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	
	InitLocalFrame();
	InitverticesFacesNeighbors();
}

MeshModel::~MeshModel()
{
}

const Face& MeshModel::GetFace(int index) const
{
	return faces[index];
}

int MeshModel::GetFacesCount() const
{
	return faces.size();
}

int MeshModel::GetNormalsCount() const
{
	return normals.size();
}

const glm::vec3& MeshModel::GetVertice(int index) const
{
	return vertices[index];
}

const glm::vec3& MeshModel::GetNormal(int index) const
{
	return normals[index];
}

int MeshModel::GetVerticesCount() const
{
	return vertices.size();
}

const std::string& MeshModel::GetModelName() const
{
	return model_name;
}

const std::vector<glm::vec3> MeshModel::GetVertices() const
{
	return vertices;
}

void MeshModel::ApplyModelScale(double scaleX, double scaleY, double scaleZ)
{
	// Multiply current scale parameters (in diagonal) by new parameters
	ScaleModel[0][0] *= scaleX;
	ScaleModel[1][1] *= scaleY;
	ScaleModel[2][2] *= scaleZ;

	Origin = FromHomogCoords(GetTransformation() * ToHomogCoords(Origin));
	AxisX = FromHomogCoords(GetTransformation() * ToHomogCoords(AxisX));
	AxisY = FromHomogCoords(GetTransformation() * ToHomogCoords(AxisY));
	AxisZ = FromHomogCoords(GetTransformation() * ToHomogCoords(AxisZ));
}

void MeshModel::ApplyModelRotate(double rotateX, double rotateY, double rotateZ)
{
	ModelRotateVal.x += rotateX;
	ModelRotateVal.y += rotateY;
	ModelRotateVal.z += rotateZ;

	double radiansValX = ToRadians(ModelRotateVal.x);

	RotateModelX[1][1] = cos(radiansValX);
	RotateModelX[1][2] = -sin(radiansValX);
	RotateModelX[2][1] = sin(radiansValX);
	RotateModelX[2][2] = cos(radiansValX);

	double radiansValY = ToRadians(ModelRotateVal.y);

	RotateModelY[0][0] = cos(radiansValY);
	RotateModelY[0][2] = sin(radiansValY);
	RotateModelY[2][0] = -sin(radiansValY);
	RotateModelY[2][2] = cos(radiansValY);

	double radiansValZ = ToRadians(ModelRotateVal.z);

	RotateModelZ[0][0] = cos(radiansValZ);
	RotateModelZ[1][0] = -sin(radiansValZ);
	RotateModelZ[0][1] = sin(radiansValZ);
	RotateModelZ[1][1] = cos(radiansValZ);

	RotateModel = RotateModelZ * RotateModelY * RotateModelX;

	Origin = FromHomogCoords(GetTransformation() * ToHomogCoords(Origin));
	AxisX = FromHomogCoords(GetTransformation() * ToHomogCoords(AxisX));
	AxisY = FromHomogCoords(GetTransformation() * ToHomogCoords(AxisY));
	AxisZ = FromHomogCoords(GetTransformation() * ToHomogCoords(AxisZ));
}

void MeshModel::ApplyModelTranslate(double transX, double transY, double transZ)
{
	// Add current translation parameters (in last column) by new parameters
	TranslateModel[3][0] += transX;
	TranslateModel[3][1] += transY;
	TranslateModel[3][2] += transZ;

	Origin = FromHomogCoords(GetTransformation() * ToHomogCoords(Origin));
	AxisX = FromHomogCoords(GetTransformation() * ToHomogCoords(AxisX));
	AxisY = FromHomogCoords(GetTransformation() * ToHomogCoords(AxisY));
	AxisZ = FromHomogCoords(GetTransformation() * ToHomogCoords(AxisZ));
}

void MeshModel::ApplyWorldScale(double scaleX, double scaleY, double scaleZ)
{
	// Multiply current scale parameters (in diagonal) by new parameters
	ScaleWorld[0][0] *= scaleX;
	ScaleWorld[1][1] *= scaleY;
	ScaleWorld[2][2] *= scaleZ;

	Origin = FromHomogCoords(GetTransformation() * ToHomogCoords(Origin));
	AxisX = FromHomogCoords(GetTransformation() * ToHomogCoords(AxisX));
	AxisY = FromHomogCoords(GetTransformation() * ToHomogCoords(AxisY));
	AxisZ = FromHomogCoords(GetTransformation() * ToHomogCoords(AxisZ));
}

void MeshModel::ApplyWorldRotate(double rotateX, double rotateY, double rotateZ)
{
	WorldRotateVal.x += rotateX;
	WorldRotateVal.y += rotateY;
	WorldRotateVal.z += rotateZ;

	double radiansValX = ToRadians(WorldRotateVal.x);

	RotateWorldX[1][1] = cos(radiansValX);
	RotateWorldX[1][2] = -sin(radiansValX);
	RotateWorldX[2][1] = sin(radiansValX);
	RotateWorldX[2][2] = cos(radiansValX);

	double radiansValY = ToRadians(WorldRotateVal.y);

	RotateWorldY[0][0] = cos(radiansValY);
	RotateWorldY[0][2] = sin(radiansValY);
	RotateWorldY[2][0] = -sin(radiansValY);
	RotateWorldY[2][2] = cos(radiansValY);

	double radiansValZ = ToRadians(WorldRotateVal.z);

	RotateWorldZ[0][0] = cos(radiansValZ);
	RotateWorldZ[1][0] = -sin(radiansValZ);
	RotateWorldZ[0][1] = sin(radiansValZ);
	RotateWorldZ[1][1] = cos(radiansValZ);

	RotateWorld = RotateWorldZ * RotateWorldY * RotateWorldX;

	Origin = FromHomogCoords(GetTransformation() * ToHomogCoords(Origin));
	AxisX = FromHomogCoords(GetTransformation() * ToHomogCoords(AxisX));
	AxisY = FromHomogCoords(GetTransformation() * ToHomogCoords(AxisY));
	AxisZ = FromHomogCoords(GetTransformation() * ToHomogCoords(AxisZ));
}

void MeshModel::ApplyWorldTranslate(double transX, double transY, double transZ)
{
	// Add current translation parameters (in last column) by new parameters
	TranslateModel[3][0] += transX;
	TranslateModel[3][1] += transY;
	TranslateModel[3][2] += transZ;

	Origin = FromHomogCoords(GetTransformation() * ToHomogCoords(Origin));
	AxisX = FromHomogCoords(GetTransformation() * ToHomogCoords(AxisX));
	AxisY = FromHomogCoords(GetTransformation() * ToHomogCoords(AxisY));
	AxisZ = FromHomogCoords(GetTransformation() * ToHomogCoords(AxisZ));
}

void MeshModel::SetModelScale(double scaleX, double scaleY, double scaleZ)
{
	// Set current scale parameters (in diagonal) to be new parameters
	ScaleModel[0][0] = scaleX;
	ScaleModel[1][1] = scaleY;
	ScaleModel[2][2] = scaleZ;

	Origin = FromHomogCoords(GetTransformation() * ToHomogCoords(Origin));
	AxisX = FromHomogCoords(GetTransformation() * ToHomogCoords(AxisX));
	AxisY = FromHomogCoords(GetTransformation() * ToHomogCoords(AxisY));
	AxisZ = FromHomogCoords(GetTransformation() * ToHomogCoords(AxisZ));
}

void MeshModel::SetModelRotate(double rotateX, double rotateY, double rotateZ)
{
	ModelRotateVal.x = rotateX;
	ModelRotateVal.y = rotateY;
	ModelRotateVal.z = rotateZ;

	double radiansValX = ToRadians(ModelRotateVal.x);

	RotateModelX[1][1] = cos(radiansValX);
	RotateModelX[1][2] = -sin(radiansValX);
	RotateModelX[2][1] = sin(radiansValX);
	RotateModelX[2][2] = cos(radiansValX);

	double radiansValY = ToRadians(ModelRotateVal.y);

	RotateModelY[0][0] = cos(radiansValY);
	RotateModelY[0][2] = sin(radiansValY);
	RotateModelY[2][0] = -sin(radiansValY);
	RotateModelY[2][2] = cos(radiansValY);

	double radiansValZ = ToRadians(ModelRotateVal.z);

	RotateModelZ[0][0] = cos(radiansValZ);
	RotateModelZ[1][0] = -sin(radiansValZ);
	RotateModelZ[0][1] = sin(radiansValZ);
	RotateModelZ[1][1] = cos(radiansValZ);

	RotateModel = RotateModelZ * RotateModelY * RotateModelX;

	Origin = FromHomogCoords(GetTransformation() * ToHomogCoords(Origin));
	AxisX = FromHomogCoords(GetTransformation() * ToHomogCoords(AxisX));
	AxisY = FromHomogCoords(GetTransformation() * ToHomogCoords(AxisY));
	AxisZ = FromHomogCoords(GetTransformation() * ToHomogCoords(AxisZ));
}

void MeshModel::SetModelTranslate(double transX, double transY, double transZ)
{
	// Set current translation parameters (in last column) to be new parameters
	TranslateModel[3][0] = transX;
	TranslateModel[3][1] = transY;
	TranslateModel[3][2] = transZ;

	Origin = FromHomogCoords(GetTransformation() * ToHomogCoords(Origin));
	AxisX = FromHomogCoords(GetTransformation() * ToHomogCoords(AxisX));
	AxisY = FromHomogCoords(GetTransformation() * ToHomogCoords(AxisY));
	AxisZ = FromHomogCoords(GetTransformation() * ToHomogCoords(AxisZ));
}

void MeshModel::SetWorldScale(double scaleX, double scaleY, double scaleZ)
{
	// Set current scale parameters (in diagonal) to be new parameters
	ScaleWorld[0][0] = scaleX;
	ScaleWorld[1][1] = scaleY;
	ScaleWorld[2][2] = scaleZ;

	Origin = FromHomogCoords(GetTransformation() * ToHomogCoords(Origin));
	AxisX = FromHomogCoords(GetTransformation() * ToHomogCoords(AxisX));
	AxisY = FromHomogCoords(GetTransformation() * ToHomogCoords(AxisY));
	AxisZ = FromHomogCoords(GetTransformation() * ToHomogCoords(AxisZ));
}

void MeshModel::SetWorldRotate(double rotateX, double rotateY, double rotateZ)
{
	WorldRotateVal.x = rotateX;
	WorldRotateVal.y = rotateY;
	WorldRotateVal.z = rotateZ;

	double radiansValX = ToRadians(WorldRotateVal.x);

	RotateWorldX[1][1] = cos(radiansValX);
	RotateWorldX[1][2] = -sin(radiansValX);
	RotateWorldX[2][1] = sin(radiansValX);
	RotateWorldX[2][2] = cos(radiansValX);

	double radiansValY = ToRadians(WorldRotateVal.y);

	RotateWorldY[0][0] = cos(radiansValY);
	RotateWorldY[0][2] = -sin(radiansValY);
	RotateWorldY[2][0] = sin(radiansValY);
	RotateWorldY[2][2] = cos(radiansValY);

	double radiansValZ = ToRadians(WorldRotateVal.z);

	RotateWorldZ[0][0] = cos(radiansValZ);
	RotateWorldZ[1][0] = sin(radiansValZ);
	RotateWorldZ[0][1] = -sin(radiansValZ);
	RotateWorldZ[1][1] = cos(radiansValZ);

	RotateWorld = RotateWorldZ * RotateWorldY * RotateWorldX;

	Origin = FromHomogCoords(GetTransformation() * ToHomogCoords(Origin));
	AxisX = FromHomogCoords(GetTransformation() * ToHomogCoords(AxisX));
	AxisY = FromHomogCoords(GetTransformation() * ToHomogCoords(AxisY));
	AxisZ = FromHomogCoords(GetTransformation() * ToHomogCoords(AxisZ));
}

void MeshModel::SetWorldTranslate(double transX, double transY, double transZ)
{
	// Set current translation parameters (in last column) to be new parameters
	TranslateWorld[3][0] = transX;
	TranslateWorld[3][1] = transY;
	TranslateWorld[3][2] = transZ;

	Origin = FromHomogCoords(GetTransformation() * ToHomogCoords(Origin));
	AxisX = FromHomogCoords(GetTransformation() * ToHomogCoords(AxisX));
	AxisY = FromHomogCoords(GetTransformation() * ToHomogCoords(AxisY));
	AxisZ = FromHomogCoords(GetTransformation() * ToHomogCoords(AxisZ));
}

glm::mat4x4 MeshModel::GetTransformation() const
{
	glm::mat4x4 ModelTrans = TranslateModel * RotateModel * ScaleModel;
	glm::mat4x4 WorldTrans = TranslateWorld * RotateWorld * ScaleWorld;
	
	return WorldTrans * ModelTrans;
}

const glm::vec3& MeshModel::GetOrigin() const
{
	return Origin;
}

const glm::vec3& MeshModel::GetAxisX() const
{
	return AxisX;
}

const glm::vec3& MeshModel::GetAxisY() const
{
	return AxisY;
}

const glm::vec3& MeshModel::GetAxisZ() const
{
	return AxisZ;
}

void MeshModel::InitLocalFrame()
{
	auto minMax = GetMinMax(vertices);

	Origin = glm::vec3((std::get<0>(minMax.second) - std::get<0>(minMax.first)) / 2, 
		(std::get<1>(minMax.second) - std::get<1>(minMax.first)) / 2,
		(std::get<2>(minMax.second) - std::get<2>(minMax.first)) / 2);

	AxisX = glm::vec3(std::get<0>(minMax.first), std::get<1>(minMax.first), std::get<2>(minMax.first));

	AxisY = glm::vec3(std::get<0>(minMax.second),
		(std::get<1>(minMax.second) - std::get<1>(minMax.first)) / 2,
		(std::get<2>(minMax.second) - std::get<2>(minMax.first)) / 2);

	AxisZ = glm::vec3((std::get<0>(minMax.second) - std::get<0>(minMax.first)) / 2,
		std::get<1>(minMax.second),
		(std::get<2>(minMax.second) - std::get<2>(minMax.first)) / 2);
}

void MeshModel::InitverticesFacesNeighbors()
{
	verticesFacesNeighbors.reserve(GetVerticesCount());
	for (int i = 0; i < GetVerticesCount(); i++)
	{
		for (int j = 0; j < GetFacesCount(); j++)
		{
			for (int k = 0; k < 3; k++) 
			{
				verticesFacesNeighbors.at(GetFace(j).GetVertexIndex(k) - 1).push_back(j);
			}
		}
	}
}

glm::vec3 MeshModel::GetNormalVertix(int index) const
{
	glm::vec3 sum(0.0f);
	int face_count = 0;
	for (int j : verticesFacesNeighbors.at(index))
	{
		sum += GetFaceNormal(j);
		face_count++;
	}
	if (face_count == 0)
	{
		sum /= face_count;
	}
	return sum;
}

glm::vec3 MeshModel::GetFaceNormal(int index) const
{
	glm::vec3 point1 = GetVertice(GetFace(index).GetVertexIndex(1) - 1) - GetVertice(GetFace(index).GetVertexIndex(0) - 1);
	glm::vec3 point2 = GetVertice(GetFace(index).GetVertexIndex(2) - 1) - GetVertice(GetFace(index).GetVertexIndex(0) - 1);

	glm::vec3 normal = glm::normalize(glm::cross(point1, point2));
	return normal;
}

glm::vec3 MeshModel::GetFaceCenter(const Face& face) const
{
	glm::vec3 p1 = GetVertice(face.GetVertexIndex(0) - 1),
			p2 = GetVertice(face.GetVertexIndex(1) - 1),
			p3 = GetVertice(face.GetVertexIndex(2) - 1);

	std::cout << "center" << std::endl;
	std::cout << glm::to_string(glm::vec3((p1.x + p2.x + p3.x) / 3, (p1.y + p2.y + p3.y) / 3, (p1.z + p2.z + p3.z) / 3)) << std::endl;

	return glm::vec3((p1.x + p2.x + p3.x) / 3, (p1.y + p2.y + p3.y) / 3, (p1.z + p2.z + p3.z) / 3);
}


/**
 * @brief Calculates the 8 points of the model's bounding box.
	1. (minX, minY, minZ)
	2. (minX, minY, maxZ)
	3. (minX, maxY, minZ)
	4. (minX, maxY, maxZ)
	5. (maxX, minY, minZ)
	6. (maxX, minY, maxZ)
	7. (maxX, maxY, minZ)
	8. (maxX, maxY, maxZ)
*/
std::vector<glm::vec3> MeshModel::getBoundingBox() const
{
	auto minMax = GetMinMax(vertices);
	std::cout << "========================\nmin max" << std::endl;
	std::cout << std::get<0>(minMax.first) << std::endl;
	std::cout << std::get<1>(minMax.first) << std::endl;
	std::cout << std::get<2>(minMax.first) << std::endl;
	std::cout << std::get<0>(minMax.second) << std::endl;
	std::cout << std::get<1>(minMax.second) << std::endl;
	std::cout << std::get<2>(minMax.second) << std::endl;

	std::vector< glm::vec3> box;

	// (minX, minY, minZ)
	box.push_back(glm::vec3(std::get<0>(minMax.first), std::get<1>(minMax.first), std::get<2>(minMax.first)));
	// (minX, minY, maxZ)
	box.push_back(glm::vec3(std::get<0>(minMax.first), std::get<1>(minMax.first), std::get<2>(minMax.second)));
	// (minX, maxY, minZ)
	box.push_back(glm::vec3(std::get<0>(minMax.first), std::get<1>(minMax.second), std::get<2>(minMax.first)));
	// (minX, maxY, maxZ)
	box.push_back(glm::vec3(std::get<0>(minMax.first), std::get<1>(minMax.second), std::get<2>(minMax.second)));
	// (maxX, minY, minZ)
	box.push_back(glm::vec3(std::get<0>(minMax.second), std::get<1>(minMax.first), std::get<2>(minMax.first)));
	// (maxX, minY, maxZ)
	box.push_back(glm::vec3(std::get<0>(minMax.second), std::get<1>(minMax.first), std::get<2>(minMax.second)));
	// (maxX, maxY, minZ)
	box.push_back(glm::vec3(std::get<0>(minMax.second), std::get<1>(minMax.second), std::get<2>(minMax.first)));
	// (maxX, maxY, maxZ)
	box.push_back(glm::vec3(std::get<0>(minMax.second), std::get<1>(minMax.second), std::get<2>(minMax.second)));
	return box;
}

void MeshModel::TransformModelFrame(glm::mat4x4 trans)
{
	Origin = FromHomogCoords(trans * ToHomogCoords(Origin));
	AxisX = FromHomogCoords(trans * ToHomogCoords(AxisX));
	AxisY = FromHomogCoords(trans * ToHomogCoords(AxisY));
	AxisZ = FromHomogCoords(trans * ToHomogCoords(AxisZ));
}

/* Utils */
/**
 * @brief Calculates the minimum and maximum values of X,Y,Z axises.
 * @param vertices The vertices of the model
 * @return pair(tuple(min), tuple(max))
*/
std::pair<std::tuple<double, double, double>, std::tuple<double, double, double>> MeshModel::GetMinMax(std::vector<glm::vec3> vertices)
{
	double minX = vertices[0].x,
		minY = vertices[0].y,
		maxX = vertices[0].x,
		maxY = vertices[0].y,
		maxZ = vertices[0].z,
		minZ = vertices[0].z;

	for (glm::vec3 v : vertices)
	{
		if (v.x < minX)
		{
			minX = v.x;
		}
		if (v.y < minY)
		{
			minY = v.y;
		}
		if (v.x > maxX)
		{
			maxX = v.x;
		}
		if (v.y > maxY)
		{
			maxY = v.y;
		}
		if (v.z > maxZ)
		{
			maxZ = v.z;
		}
		if (v.z < minZ)
		{
			minZ = v.z;
		}
	}

	return std::pair<std::tuple<double, double, double>, std::tuple<double, double, double>>(std::make_tuple(minX, minY, minZ), std::make_tuple(maxX, maxY, maxZ));
}

double MeshModel::ToRadians(double value)
{
	return value * (M_PI / 180);
}

double MeshModel::ToDegrees(double value)
{
	return value * (180 / M_PI);
}

glm::vec4 MeshModel::ToHomogCoords(glm::vec3 vec)
{
	return glm::vec4(vec, 1.0f);
}

glm::vec3 MeshModel::FromHomogCoords(glm::vec4 vec)
{
	return glm::vec3(vec.x / vec.w, vec.y / vec.w, vec.z / vec.w);
}