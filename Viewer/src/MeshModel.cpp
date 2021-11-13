#include "MeshModel.h"

MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name) :
	faces(faces),
	vertices(vertices),
	normals(normals)
{

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