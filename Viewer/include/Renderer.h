#pragma once
#include "Scene.h"
#include "Utils.h"
#include <iostream>
#include <complex>
#include <utility>
#include <tuple>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <stdlib.h>
#include <time.h>

#define PI atan(1)*4

class Renderer
{
public:
	Renderer(int viewportWidth, int viewportHeight);
	virtual ~Renderer();
	void Render(const Scene& scene);
	void SwapBuffers();
	void ClearColorBuffer(const glm::vec3& color);
	void SetViewport(const int width, const int height);

	int GetViewportWidth() const;
	int GetViewportHeight() const;

private:
	void PutPixel(const int i, const int j, const glm::vec3& color, float z);
	void DrawLine(const glm::ivec3& p1, const glm::ivec3& p2, const glm::vec3& color);
	float ComputeDepth(int x, int y, glm::ivec3 p1, glm::ivec3 p2);

	void DrawLineSanityCheck();
	void DrawFilledCircle(glm::vec3 center, float radius);
	void DrawFilledRectangle(glm::vec3 point, int width, int height, glm::vec3 color);

	void DrawModel(const MeshModel& model, const Camera& camera, const Light& light);
	void DrawFace(const Face& face, const MeshModel& model, const Camera& camera, const Light& light);
	void DrawNormal(const int& index, const Face& face, const MeshModel& model, const Camera& camera);
	void DrawNormalsVertices(const MeshModel& model, const Camera& camera);
	glm::vec3 TransVector(const glm::vec3& vec, const MeshModel& model, const Camera& camera);
	glm::vec3 TransVector(const glm::vec3& vec, const Light& light, const Camera& camera);
	static glm::vec3 ApplyTrans(glm::vec3 vec, glm::mat4x4 trans);

	glm::vec3 CalcAmbientReflection(const Light& light);

	void EdgeWalking(const Face& face, const MeshModel& model, const Camera& camera, const glm::vec3 color);
	bool Overlaps(const glm::vec3 v1, const glm::vec3 v2, const glm::vec3 v3, const glm::vec3 point);
	float EdgeFunction(glm::vec3 v1, glm::vec3 v2, glm::vec3 p);

	float ComputeDepth(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec2& point);
	void InitBufferZ();
	void FreeZbuffer();

	void DrawBoundingBox(const MeshModel& model, const Camera& camera);
	void DrawBoundingRectangle(const MeshModel& model, const Camera& camera, const Face& face);

	void DrawModelFrame(const MeshModel& model, const Camera& camera);
	void DrawWorldFrame(const Camera& camera);

	void CreateBuffers(int w, int h);
	void CreateOpenglBuffer();
	void InitOpenglRendering();

	float* color_buffer;
	int viewport_width;
	int viewport_height;
	GLuint gl_screen_tex;
	GLuint gl_screen_vtc;

	float** zBuffer;
};
