#pragma once
#include <glm/glm.hpp>
#include <string>
#include <tuple>
#include "MeshModel.h"

/* Initial values for colors */
static glm::vec3 clear_color = glm::vec3(0.4f, 0.55f, 0.6f);
static glm::vec3 model_color = glm::vec3{ 1, 1, 1 };
static glm::vec3 bounding_box_color = glm::vec3{ 0, 0, 0 };
static glm::vec3 face_normals_color = glm::vec3{ 1, 0, 1 };
static glm::vec3 vertics_normals_color = glm::vec3{ 0, 0, 1 };
static glm::vec3 bounding_rect_color = glm::vec3{ 0, 1, 0 };

static float const axes_length = 50.0f;
static float const normal_length = 50.0f;

static const std::string base_path = "C: ../../../Data/";

//static int const RAND_MAX = 500;

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

	static glm::vec3 GenerateRandomColor();

	static float CalcTriangleArea(const glm::vec3& p1, const glm::vec3& p2, const glm::vec2& p3);
	static glm::fvec3 triangleInterpolation(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 insidePoint);

	static glm::vec3 AdditiveColor(const glm::vec3& c1, const glm::vec3& c2);

	static std::pair<std::tuple<double, double, double>, std::tuple<double, double, double>> GetMinMax(std::vector<glm::vec3> vertices);
};
