#pragma once
#include <glm/glm.hpp>
#include <string>
#include <tuple>
#include "MeshModel.h"

static glm::vec4 clear_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);
static glm::vec3 model_color = glm::vec3{ 0, 0, 1 };

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

	static std::pair<std::tuple<double, double, double>, std::tuple<double, double, double>> GetMinMax(std::vector<glm::vec3> vertices);
};
