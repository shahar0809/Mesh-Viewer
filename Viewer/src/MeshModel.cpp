#include "MeshModel.h"
#include <iostream>

MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name) :
	faces(faces),
	vertices(vertices),
	normals(normals)
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

	RotateWorldX = glm::mat4x4{
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

const glm::vec3& MeshModel::GetVertice(int index) const
{
	return vertices[index];
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

float MeshModel::GetFirstScaleValue() const
{
	return FirstScaleValue;
}

float MeshModel::GetFirstTransValueX() const
{
	return FirstTransValueX;
}

float MeshModel::GetFirstTransValueY() const
{
	return FirstTransValueY;
}

void MeshModel::SetFirstScaleValue(float ScaleValue)
{
	FirstScaleValue = ScaleValue;
}

void MeshModel::SetFirstTransValueX(float TransValueX)
{
	FirstTransValueX = TransValueX;
}

void MeshModel::SetFirstTransValueY(float TransValueY)
{
	FirstTransValueY = TransValueY;
}

void MeshModel::ApplyModelScale(double scaleX, double scaleY, double scaleZ)
{
	// Multiply current scale parameters (in diagonal) by new parameters
	ScaleModel[0][0] *= scaleX;
	ScaleModel[1][1] *= scaleY;
	ScaleModel[2][2] *= scaleZ;
}

void MeshModel::ApplyModelRotate(double rotateX, double rotateY, double rotateZ)
{
	ModelRotateVal.x += rotateX;
	ModelRotateVal.y += rotateY;
	ModelRotateVal.z += rotateZ;

	//glm::mat4x4 rotMatrix = glm::eulerAngleXYZ(rotateX, rotateY, rotateZ);
	// TODO: Implement rotation around z,y 
	// Maybe there's something in the recording

	double radiansVal = ToRadians(ModelRotateVal.z);

	RotateModel[0][0] = cos(radiansVal);
	RotateModel[1][0] = -sin(radiansVal);
	RotateModel[0][1] = sin(radiansVal);
	RotateModel[1][1] = cos(radiansVal);
}

void MeshModel::ApplyModelTranslate(double transX, double transY, double transZ)
{
	// Add current translation parameters (in last column) by new parameters
	TranslateModel[3][0] += transX;
	TranslateModel[3][1] += transY;
	TranslateModel[3][2] += transZ;
}

void MeshModel::ApplyWorldScale(double scaleX, double scaleY, double scaleZ)
{
	// Multiply current scale parameters (in diagonal) by new parameters
	ScaleWorld[0][0] *= scaleX;
	ScaleWorld[1][1] *= scaleY;
	ScaleWorld[2][2] *= scaleZ;
}

void MeshModel::ApplyWorldRotate(double rotateX, double rotateY, double rotateZ)
{
	WorldRotateVal.x += rotateX;
	WorldRotateVal.y += rotateY;
	WorldRotateVal.z += rotateZ;

	double radiansVal = ToRadians(WorldRotateVal.z);

	RotateWorld[0][0] = cos(radiansVal);
	RotateWorld[1][0] = -sin(radiansVal);
	RotateWorld[0][1] = sin(radiansVal);
	RotateWorld[1][1] = cos(radiansVal);
}

void MeshModel::ApplyWorldTranslate(double transX, double transY, double transZ)
{
	// Add current translation parameters (in last column) by new parameters
	TranslateModel[3][0] += transX;
	TranslateModel[3][1] += transY;
	TranslateModel[3][2] += transZ;
}

void MeshModel::SetModelScale(double scaleX, double scaleY, double scaleZ)
{
	// Set current scale parameters (in diagonal) to be new parameters
	ScaleModel[0][0] = scaleX + FirstScaleValue;
	ScaleModel[1][1] = scaleY + FirstScaleValue;
	ScaleModel[2][2] = scaleZ + FirstScaleValue;
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
}

void MeshModel::SetModelTranslate(double transX, double transY, double transZ)
{
	// Set current translation parameters (in last column) to be new parameters
	TranslateModel[3][0] = transX + FirstTransValueX;
	TranslateModel[3][1] = transY + FirstTransValueY;
	TranslateModel[3][2] = transZ;
}

void MeshModel::SetWorldScale(double scaleX, double scaleY, double scaleZ)
{
	// Set current scale parameters (in diagonal) to be new parameters
	ScaleWorld[0][0] = scaleX;
	ScaleWorld[1][1] = scaleY;
	ScaleWorld[2][2] = scaleZ;
}

void MeshModel::SetWorldRotate(double rotateX, double rotateY, double rotateZ)
{
	WorldRotateVal.x = rotateX;
	WorldRotateVal.y = rotateY;
	WorldRotateVal.z = rotateZ;

	double radiansValX = ToRadians(ModelRotateVal.x);

	RotateWorldX[1][1] = cos(radiansValX);
	RotateWorldX[1][2] = -sin(radiansValX);
	RotateWorldX[2][1] = sin(radiansValX);
	RotateWorldX[2][2] = cos(radiansValX);

	double radiansValY = ToRadians(ModelRotateVal.y);

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
}

void MeshModel::SetWorldTranslate(double transX, double transY, double transZ)
{
	// Set current translation parameters (in last column) to be new parameters
	TranslateWorld[3][0] = transX;
	TranslateWorld[3][1] = transY;
	TranslateWorld[3][2] = transZ;
}

glm::mat4x4 MeshModel::GetTransformation() const
{
	glm::mat4x4 ModelTrans = TranslateModel * RotateModel * ScaleModel;
	glm::mat4x4 WorldTrans = TranslateWorld * RotateWorld * ScaleWorld;
	return ModelTrans *WorldTrans;
}

double MeshModel::ToRadians(double value)
{
	return value * (M_PI / 180);
}

double MeshModel::ToDegrees(double value)
{
	return value * (180 / M_PI);
}