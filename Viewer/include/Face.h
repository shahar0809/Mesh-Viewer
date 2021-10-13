#pragma once
#include "vector"
#include <string>
#include <glm/glm.hpp>

class Face
{
public:
	Face(std::istream& issLine);
	int GetVertexIndex(int index) const;
	int GetNormalIndex(int index) const;
	int GetTextureIndex(int index) const;

private:
	std::vector<int> vertex_indices;
	std::vector<int> normal_indices;
	std::vector<int> texture_indices;
};