#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Utils.h"

glm::vec3 Utils::Vec3fFromStream(std::istream& issLine)
{
	float x, y, z;
	issLine >> x >> std::ws >> y >> std::ws >> z;
	return glm::vec3(x, y, z);
}

glm::vec2 Utils::Vec2fFromStream(std::istream& issLine)
{
	float x, y;
	issLine >> x >> std::ws >> y;
	return glm::vec2(x, y);
}

std::shared_ptr<MeshModel> Utils::LoadMeshModel(const std::string& filePath)
{
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::ifstream ifile(filePath.c_str());

	// while not end of file
	while (!ifile.eof())
	{
		// get line
		std::string curLine;
		std::getline(ifile, curLine);

		// read the type of the line
		std::istringstream issLine(curLine);
		std::string lineType;

		issLine >> std::ws >> lineType;

		// based on the type parse data
		if (lineType == "v")
		{
			// Print vertices when loading mesh model
			glm::vec3 vertice = Utils::Vec3fFromStream(issLine);
			std::cout << "V: (" << vertice.x << "," << vertice.y << "," << vertice.z << ")" << std::endl;

			vertices.push_back(vertice);
		}
		else if (lineType == "vn")
		{
			normals.push_back(Utils::Vec3fFromStream(issLine));
		}
		else if (lineType == "vt")
		{
			// TODO: Handle texture coordinates
		}
		else if (lineType == "f")
		{
			Face face(issLine);
			std::cout << "F: (" << face.GetVertexIndex(0) << "," << face.GetVertexIndex(1) 
				<< "," << face.GetVertexIndex(2) << ")" << std::endl;

			faces.push_back(face);
		}
		else if (lineType == "#" || lineType == "")
		{
			// comment / empty line
		}
		else
		{
			std::cout << "Found unknown line Type \"" << lineType << "\"";
		}
	}

	std::shared_ptr<MeshModel> model = std::make_shared<MeshModel>(faces, vertices, normals, Utils::GetFileName(filePath));

	model->color = clear_color;
	model->BoundingBoxColor = clear_color;
	model->FaceNormalsColor = face_normals_color;
	model->VerticsNormalsColor = vertics_normals_color;

	return model;
}

std::string Utils::GetFileName(const std::string& filePath)
{
	if (filePath.empty()) {
		return {};
	}

	auto len = filePath.length();
	auto index = filePath.find_last_of("/\\");

	if (index == std::string::npos) {
		return filePath;
	}

	if (index + 1 >= len) {

		len--;
		index = filePath.substr(0, len).find_last_of("/\\");

		if (len == 0) {
			return filePath;
		}

		if (index == 0) {
			return filePath.substr(1, len - 1);
		}

		if (index == std::string::npos) {
			return filePath.substr(0, len);
		}

		return filePath.substr(index + 1, len - index - 1);
	}

	return filePath.substr(index + 1, len - index);
}

glm::vec4 Utils::ToHomogCoords(glm::vec3 vec)
{
	return glm::vec4(vec, 1.0f);
}

glm::vec3 Utils::FromHomogCoords(glm::vec4 vec)
{
	return glm::vec3(vec.x / vec.w, vec.y / vec.w, vec.z / vec.w);
}

/**
 * @brief Calculates the minimum and maximum values of X,Y,Z axises.
 * @param vertices The vertices of the model
 * @return pair(tuple(min), tuple(max))
*/
std::pair<std::tuple<double, double, double>, std::tuple<double, double, double>> Utils::GetMinMax(std::vector<glm::vec3> vertices)
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

double Utils::ToRadians(double value)
{
	return value * (M_PI / 180);
}

double Utils::ToDegrees(double value)
{
	return value * (180 / M_PI);
}