#include "MeshModel.h"

MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name) :
	faces(faces),
	vertices(vertices),
	normals(normals)
{
	WorldRotateVal = 0;
	ModelRotateVal = 0;

	SetModelScale(1, 1, 1);
	SetWorldScale(1, 1, 1);

	TranslateModel = glm::mat4{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	TranslateWorld = glm::mat4{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	ScaleModel = glm::mat4{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	ScaleWorld = glm::mat4{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	RotateModel = glm::mat4(0);
	RotateModel[3][3] = 1;
	RotateWorld = glm::mat4(0);
	RotateWorld[3][3] = 1;

	SetModelRotate(0);
	SetWorldRotate(0);
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

	RotateModel[0][0] = cos(ModelRotateVal);
	RotateModel[0][1] = -sin(ModelRotateVal);
	RotateModel[1][0] = sin(ModelRotateVal);
	RotateModel[1][1] = cos(ModelRotateVal);
}

void MeshModel::ApplyModelTranslate(double transX, double transY, double transZ)
{
	// Add current translation parameters (in last column) by new parameters
	TranslateModel[0][3] += transX;
	TranslateModel[1][3] += transY;
	TranslateModel[2][3] += transZ;
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

	RotateWorld[0][0] = cos(WorldRotateVal);
	RotateWorld[0][1] = -sin(WorldRotateVal);
	RotateWorld[1][0] = sin(WorldRotateVal);
	RotateWorld[1][1] = cos(WorldRotateVal);

}

void MeshModel::ApplyWorldTranslate(double transX, double transY, double transZ)
{
	// Add current translation parameters (in last column) by new parameters
	TranslateModel[0][3] += transX;
	TranslateModel[1][3] += transY;
	TranslateModel[2][3] += transZ;
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

	RotateModel[0][0] = cos(ModelRotateVal);
	RotateModel[0][1] = -sin(ModelRotateVal);
	RotateModel[1][0] = sin(ModelRotateVal);
	RotateModel[1][1] = cos(ModelRotateVal);
}

void MeshModel::SetModelTranslate(double transX, double transY, double transZ)
{
	// Set current translation parameters (in last column) to be new parameters
	TranslateModel[0][3] = transX;
	TranslateModel[1][3] = transY;
	TranslateModel[2][3] = transZ;
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

	RotateWorld[0][0] = cos(WorldRotateVal);
	RotateWorld[0][1] = -sin(WorldRotateVal);
	RotateWorld[1][0] = sin(WorldRotateVal);
	RotateWorld[1][1] = cos(WorldRotateVal);

}

void MeshModel::SetWorldTranslate(double transX, double transY, double transZ)
{
	// Set current translation parameters (in last column) to be new parameters
	TranslateModel[0][3] = transX;
	TranslateModel[1][3] = transY;
	TranslateModel[2][3] = transZ;
}

glm::mat4 MeshModel::GetTransformation() const
{
	glm::mat4 ModelTrans = ScaleModel * RotateModel * TranslateModel;
	glm::mat4 WorldTrans = ScaleWorld * RotateWorld * TranslateWorld;
	return WorldTrans * ModelTrans;
}