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
	int GetViewportWidth() const;
	int GetViewportHeight() const;

	//void fitInScreen(MeshLocal& model);
	
private:
	void PutPixel(const int i, const int j, const glm::vec3& color);
	void DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color);
	//void DrawingRectangle(int length, int width, int bold, glm::ivec2& startingPoint);
	//void DrawingCandle(const glm::ivec2& startPoint, const glm::ivec2& point1, const glm::ivec2& point2, const glm::ivec2& point3, const glm::ivec2& point4, const glm::ivec2& point5, const glm::ivec2& point6);
	//void Draw();

	void DrawLineSanityCheck();

	void DrawModel(const MeshModel& model, const Camera& camera);
	void DrawFace(const Face& face, const MeshModel& model, const Camera& camera);

	void DrawBoundingBox(const MeshModel& model);
	//void DrawAxis(glm::vec3 center);

	void CreateBuffers(int w, int h);
	void CreateOpenglBuffer();
	void InitOpenglRendering();

	float* color_buffer;
	int viewport_width;
	int viewport_height;
	GLuint gl_screen_tex;
	GLuint gl_screen_vtc;
};
