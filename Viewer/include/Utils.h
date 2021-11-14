#pragma once
#include <glm/glm.hpp>
#include <string>
#include <tuple>
#include "MeshModel.h"

#define M_PI 3.14159265358979323846264338327950288

class Utils
{
public:
	static glm::vec3 Vec3fFromStream(std::istream& issLine);
	static glm::vec2 Vec2fFromStream(std::istream& issLine);
	static std::shared_ptr<MeshModel> LoadMeshModel(const std::string& filePath);
	static std::string GetFileName(const std::string& filePath);

	static glm::vec4 ToHomogCoords(glm::vec3 vec);
	static glm::vec3 FromHomogCoords(glm::vec4 vec);

	static double ToRadians(double value);
	static double ToDegrees(double value);

	static std::pair<std::tuple<double, double, double>, std::tuple<double, double, double>> GetMin(std::vector<glm::vec3> vertices);
};
