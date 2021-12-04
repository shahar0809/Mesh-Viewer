#pragma once
#include "Scene.h"
#include "Utils.h"
#include <iostream>
#include <complex>
#include <glad/glad.h>
#include <glm/glm.hpp>

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
	void PutPixel(const int i, const int j, const glm::vec3& color);
	void DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color);

	void DrawLineSanityCheck();

	void DrawModel(const MeshModel& model, const Camera& camera);
	void DrawFace(const Face& face, const MeshModel& model, const Camera& camera);
	void DrawNormal(const int& index, const Face& face, const MeshModel& model, const Camera& camera);
	void DrawNormalsVertices(const MeshModel& model, const Camera& camera);
	glm::vec3 TransVector(const glm::vec3& vec, const MeshModel& model, const Camera& camera);

	void DrawBoundingBox(const MeshModel& model, const Camera& camera);

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
};
