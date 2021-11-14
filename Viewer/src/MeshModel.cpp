#include "MeshModel.h"

MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name) :
	faces(faces),
	vertices(vertices),
	normals(normals)
{
	WorldRotateVal = 0;
	ModelRotateVal = 0;

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

	RotateModel = glm::mat4x4{
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
	if (index >= vertices.size())
	{
		printf("ERORR ************************");
	}
	else
	{
		return vertices[index];
	}
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
}

void MeshModel::ApplyModelRotate(double rotateVal)
{
	ModelRotateVal += rotateVal;
	double radiansVal = Utils::ToRadians(ModelRotateVal);

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

void MeshModel::ApplyWorldRotate(double rotateVal)
{
	WorldRotateVal += rotateVal;
	double radiansVal = Utils::ToRadians(WorldRotateVal);

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
	ScaleModel[0][0] = scaleX;
	ScaleModel[1][1] = scaleY;
	ScaleModel[2][2] = scaleZ;
}

void MeshModel::SetModelRotate(double rotateVal)
{
	ModelRotateVal = rotateVal;
	double radiansVal = Utils::ToRadians(rotateVal);

	RotateModel[0][0] = cos(radiansVal);
	RotateModel[1][0] = -sin(radiansVal);
	RotateModel[0][1] = sin(radiansVal);
	RotateModel[1][1] = cos(radiansVal);
}

void MeshModel::SetModelTranslate(double transX, double transY, double transZ)
{
	// Set current translation parameters (in last column) to be new parameters
	TranslateModel[3][0] = transX;
	TranslateModel[3][1] = transY;
	TranslateModel[3][2] = transZ;
}

void MeshModel::SetWorldScale(double scaleX, double scaleY, double scaleZ)
{
	// Set current scale parameters (in diagonal) to be new parameters
	ScaleWorld[0][0] = scaleX;
	ScaleWorld[1][1] = scaleY;
	ScaleWorld[2][2] = scaleZ;
}

void MeshModel::SetWorldRotate(double rotateVal)
{
	WorldRotateVal = rotateVal;
	double radiansVal = Utils::ToRadians(WorldRotateVal);

	RotateWorld[0][0] = cos(radiansVal);
	RotateWorld[1][0] = -sin(radiansVal);
	RotateWorld[0][1] = sin(radiansVal);
	RotateWorld[1][1] = cos(radiansVal);

}

void MeshModel::SetWorldTranslate(double transX, double transY, double transZ)
{
	// Set current translation parameters (in last column) to be new parameters
	TranslateModel[3][0] = transX;
	TranslateModel[3][1] = transY;
	TranslateModel[3][2] = transZ;
}

glm::mat4x4 MeshModel::GetTransformation() const
{
	glm::mat4x4 ModelTrans = ScaleModel * RotateModel * TranslateModel;
	glm::mat4x4 WorldTrans = ScaleWorld * RotateWorld * TranslateWorld;
	return WorldTrans * ModelTrans;
}