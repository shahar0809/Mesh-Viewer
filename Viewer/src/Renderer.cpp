#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

#include "Renderer.h"
#include "InitShader.h"
#include <glm/gtx/string_cast.hpp>

#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)
#define Z_INDEX(width,x,y) ((x)+(y)*(width))

Renderer::Renderer(int viewport_width, int viewport_height) :
	viewport_width(viewport_width),
	viewport_height(viewport_height)
{
	srand(time(NULL));
	InitOpenglRendering();
	CreateBuffers(viewport_width, viewport_height);
}

Renderer::~Renderer()
{
	delete[] color_buffer;
}

void Renderer::PutPixel(int i, int j, const glm::vec3& color)
{
	if (i < 0) return; if (i >= viewport_width) return;
	if (j < 0) return; if (j >= viewport_height) return;

	color_buffer[INDEX(viewport_width, i, j, 0)] = color.x;
	color_buffer[INDEX(viewport_width, i, j, 1)] = color.y;
	color_buffer[INDEX(viewport_width, i, j, 2)] = color.z;
}

void Renderer::DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color)
{
	int x, y;

	int deltaX = p2.x - p1.x;
	int deltaY = p2.y - p1.y;

	// Calculate absoloute values of deltas
	int abs_deltaX = fabs(deltaX);
	int abs_deltaY = fabs(deltaY);

	// Precalculate the decision parameters
	int decisionParam_X = 2 * abs_deltaY - abs_deltaX;
	int decisionParam_Y = 2 * abs_deltaX - abs_deltaY;

	if (deltaX == 0 && deltaY == 0)
	{
		return;
	}

	// Slope < 1
	if (abs_deltaY <= abs_deltaX)
	{
		// If the first points is after the second one (on X coordinates), switch them
		if (deltaX < 0)
		{
			DrawLine(p2, p1, color);
			return;
		}

		y = p1.y;
		// Go over all X's from start to end
		for (x = p1.x; x < p2.x; x++)
		{
			if (decisionParam_X < 0)
			{
				decisionParam_X += 2 * abs_deltaY;
			}
			else
			{
				// If the slope is positive, we want to increase Y
				if ((deltaX > 0 && deltaY > 0) || (deltaX < 0 && deltaY < 0))
				{
					y++;
				}
				// Otherwise, we'll decrease it (in the direction of the line)
				else
				{
					y--;
				}
				decisionParam_X += 2 * (abs_deltaY - abs_deltaX);
			}
			PutPixel(x, y, color);
		}
	}
	// We go over the Y coordinates, instead of X ( Slope >= 1)
	else
	{
		int endY;
		glm::ivec2 start, end;

		// Change points order if necessary
		if (deltaY < 0)
		{
			start = p2;
			end = p1;
		}
		else
		{
			start = p1;
			end = p2;
		}

		int x = start.x;
		for (int y = start.y; y < end.y; y++)
		{
			if (decisionParam_Y < 0)
			{
				decisionParam_Y += 2 * abs_deltaX;
			}
			else
			{
				// If the slope is positive, we want to increase X
				if ((deltaX > 0 && deltaY > 0) || (deltaX < 0 && deltaY < 0))
				{
					x++;
				}
				// Otherwise, we'll decrease it (in the direction of the line)
				else
				{
					x--;
				}
				decisionParam_Y += 2 * (abs_deltaX - abs_deltaY);
			}
			PutPixel(x, y, color);
		}
	}
}

void Renderer::DrawLineSanityCheck()
{
	glm::ivec2 circleCenter(600, 400);
	glm::ivec2 currentPoint;
	glm::vec3 color({ 0, 0, 1 });

	int stepSize = 10, circleRadius = 150, fullCircle = 360;
	PutPixel(circleCenter.x, circleCenter.y, { 0,0,0 });

	int currentStep = 0;
	while (currentStep < fullCircle)
	{
		// Calculte current point on circle
		currentPoint.x = circleCenter.x + circleRadius * cos(currentStep * (PI / 180));
		currentPoint.y = circleCenter.y - circleRadius * sin(currentStep * (PI / 180));

		PutPixel(currentPoint.x, currentPoint.y, { 0,0,0 });

		// Draw line from center to current point
		DrawLine(circleCenter, currentPoint, color);

		// Increase step size
		currentStep += stepSize;
	}
}

void Renderer::DrawModelFrame(const MeshModel& model, const Camera& camera)
{
	glm::mat4x4 transform = camera.GetViewportTrans(viewport_width, viewport_height) * camera.GetProjectionTransformation()
		* glm::inverse(camera.GetViewTransformation()) * model.GetWorldTransformation();

	glm::vec3 ModelOrigin = ApplyTrans(model.GetOrigin(), transform);
	glm::vec3 AxisX = ApplyTrans(model.GetAxisX(), transform);
	glm::vec3 AxisY = ApplyTrans(model.GetAxisY(), transform);
	glm::vec3 AxisZ = ApplyTrans(model.GetAxisZ(), transform);

	DrawLine(ModelOrigin, AxisX, { 1, 0, 0 }); // X - Red
	DrawLine(ModelOrigin, AxisY, { 0, 1, 0 }); // Y - Green
	DrawLine(ModelOrigin, AxisZ, { 0, 0, 1 }); // Z - Blue
}

void Renderer::DrawWorldFrame(const Camera& camera)
{
	glm::mat4x4 transform = camera.GetProjectionTransformation()
		* glm::inverse(camera.GetViewTransformation());

	glm::vec3 WorldOrigin(viewport_width / 2, viewport_height / 2, 0);
	glm::vec3 AxisX(viewport_width, viewport_height / 2, 150);
	glm::vec3 AxisY(viewport_width / 2, viewport_height, 150);
	glm::vec3 AxisZ(viewport_width / 2, viewport_height / 2, 300);

	glm::vec3 MAxisX(0, viewport_height / 2, 150);
	glm::vec3 MAxisY(viewport_width / 2, 0, 150);
	glm::vec3 MAxisZ(viewport_width / 2, viewport_height / 2, 0);

	DrawLine(WorldOrigin, MAxisX, { 1, 0, 0 });
	DrawLine(WorldOrigin, MAxisY, { 0, 1, 0 });
	DrawLine(WorldOrigin, MAxisZ, { 1, 0, 0 });

	DrawLine(WorldOrigin, AxisX, { 1, 0, 0 });
	DrawLine(WorldOrigin, AxisY, { 0, 1, 0 });
	DrawLine(WorldOrigin, AxisZ, { 1, 0, 0 });
}

void Renderer::DrawBoundingBox(const MeshModel& model, const Camera& camera)
{
	std::vector<glm::vec3> boundingBox = model.getBoundingBox();

	for (int i = 0; i < 8; i++)
	{
		boundingBox[i] = TransVector(boundingBox[i], model, camera);
	}

	DrawLine(boundingBox[0], boundingBox[1], model.gui.BoundingBoxColor);
	DrawLine(boundingBox[0], boundingBox[2], model.gui.BoundingBoxColor);
	DrawLine(boundingBox[0], boundingBox[4], model.gui.BoundingBoxColor);

	DrawLine(boundingBox[1], boundingBox[3], model.gui.BoundingBoxColor);
	DrawLine(boundingBox[1], boundingBox[5], model.gui.BoundingBoxColor);

	DrawLine(boundingBox[2], boundingBox[3], model.gui.BoundingBoxColor);
	DrawLine(boundingBox[2], boundingBox[6], model.gui.BoundingBoxColor);

	DrawLine(boundingBox[3], boundingBox[7], model.gui.BoundingBoxColor);

	DrawLine(boundingBox[4], boundingBox[6], model.gui.BoundingBoxColor);
	DrawLine(boundingBox[4], boundingBox[5], model.gui.BoundingBoxColor);

	DrawLine(boundingBox[5], boundingBox[7], model.gui.BoundingBoxColor);

	DrawLine(boundingBox[6], boundingBox[7], model.gui.BoundingBoxColor);
}

void Renderer::DrawBoundingRectangle(const MeshModel& model, const Camera& camera, const Face& face)
{
	std::vector<glm::vec3> transformedVecs;
	for (int i = 0; i < 3; i++)
	{
		transformedVecs.push_back(TransVector(model.GetVertice(face.GetVertexIndex(i) - 1), model, camera));
	}

	auto rectanglePoints = model.GetBoundingRectangle(transformedVecs);
	glm::vec3 randColor = Utils::GenerateRandomColor();
	DrawLine(rectanglePoints[0], rectanglePoints[1], randColor);
	DrawLine(rectanglePoints[1], rectanglePoints[2], randColor);
	DrawLine(rectanglePoints[2], rectanglePoints[3], randColor);
	DrawLine(rectanglePoints[3], rectanglePoints[0], randColor);
}

void Renderer::DrawModel(const MeshModel& model, const Camera& camera)
{
	for (int i = 0; i < model.GetFacesCount(); i++)
	{
		Face currFace = model.GetFace(i);		

		if (model.gui.IsFrameOnScreen)
			DrawModelFrame(model, camera);
		if (model.gui.AreFaceNormalsOnScreen)
			DrawNormal(i, currFace, model, camera);
		if (model.gui.AreVerticesNormalsOnScreen)
			DrawNormalsVertices(model, camera);
		if (model.gui.IsBoundingBoxOnScreen)
			DrawBoundingBox(model, camera);
		if (model.gui.IsBoundingRectOnScreen)
			DrawBoundingRectangle(model, camera, currFace);

		DrawFace(currFace, model, camera);
	}
}

void Renderer::DrawFace(const Face& face, const MeshModel& model, const Camera& camera)
{
	std::vector<glm::vec3> transformedVecs;

	// Apply transformation on vertices
	for (int i = 0; i < 3; i++)
	{
		transformedVecs.push_back(TransVector(model.GetVertice(face.GetVertexIndex(i) - 1), model, camera));
	}

	EdgeWalking(face, model, camera, Utils::GenerateRandomColor());

	DrawLine(transformedVecs[0], transformedVecs[1], model.gui.color);
	DrawLine(transformedVecs[1], transformedVecs[2], model.gui.color);
	DrawLine(transformedVecs[2], transformedVecs[0], model.gui.color);
}

/**
 * @brief Draws normal of a face
 * @param index The index of the face in the model	
 * @param face The Face
 * @param model The model
 * @param camera The active camera
*/
void Renderer::DrawNormal(const int& index, const Face& face, const MeshModel& model, const Camera& camera)
{
	DrawLine(TransVector(model.GetFaceCenter(face), model, camera), TransVector(model.GetFaceNormal(index) + model.GetFaceCenter(face), model, camera), model.gui.FaceNormalsColor);
}

void Renderer::DrawNormalsVertices(const MeshModel& model, const Camera& camera)
{
	for (int i = 0; i < model.GetFacesCount(); i++)
	{
		Face currFace = model.GetFace(i);

		for (int j = 0; j < 3; j++)
		{
			glm::vec3 vertex = model.GetVertice(currFace.GetVertexIndex(j) - 1);
			glm::vec3 normal = model.GetNormal(currFace.GetNormalIndex(j) - 1) + vertex;
			DrawLine(TransVector(vertex, model, camera), TransVector(normal, model, camera), model.gui.VerticsNormalsColor);
		}
	}
}

float Renderer::EdgeFunction(glm::vec3 v1, glm::vec3 v2, glm::vec3 p)
{
	return (p.x - v1.x) * (v2.y - v1.y) - (p.y - v1.y) * (v2.x - v1.x);
}

/**
 * @brief Checks if a point overlaps a triangle.
 * @param v1 First vertex in triangle
 * @param v2 Second vertex in triangle
 * @param v3 Third vertex in triangle
 * @param point The point
 * @return If it overlaps th triangle
*/
bool Renderer::Overlaps(const glm::vec3 v1, const glm::vec3 v2, const glm::vec3 v3, const glm::vec3 point)
{
	bool doesOverlap = true;

	std::vector<float> isLeft;
	isLeft.push_back(EdgeFunction(v2, v3, point));
	isLeft.push_back(EdgeFunction(v3, v1, point));
	isLeft.push_back(EdgeFunction(v1, v2, point));

	// Get edges of triangles
	std::vector<glm::vec3> edges;
	edges.push_back(v3 - v2);
	edges.push_back(v1 - v3);
	edges.push_back(v2 - v1);

	for (int i = 0; i < 3; i++)
	{
		if (isLeft[i])
		{
			doesOverlap &= isLeft[i] > 0;
		}
		else
		{
			// Check if the edge is a left edge or top edge
			doesOverlap &= (edges[i].y == 0 && edges[i].x > 0) || (edges[i].y > 0);
		}
	}

	return doesOverlap;
}

void Renderer::EdgeWalking(const Face& face, const MeshModel& model, const Camera& camera, const glm::vec3 color)
{
	std::vector<glm::vec3> transformedVecs;
	for (int i = 0; i < 3; i++)
	{
		transformedVecs.push_back(TransVector(model.GetVertice(face.GetVertexIndex(i) - 1), model, camera));
	}

	auto boundingRect = model.GetBoundingRectangle(transformedVecs);
	for (int i = boundingRect[0].x; i <= boundingRect[1].x; i++)
	{
		for (int j = boundingRect[2].y; j <= boundingRect[1].y; j++)
		{
			glm::vec3 currPoint(i, j, 0);
			if (Overlaps(transformedVecs[0], transformedVecs[1], transformedVecs[2], currPoint))
			{
				PutPixel(i, j, color);
			}
		}
	}
}

/**
 * @brief Apply renderer transformation on a vertex.
 * @param vec The vertex
 * @param model The model of the vertex
 * @param camera The active camera
 * @return The vector after applying transformation
*/
glm::vec3 Renderer::TransVector(const glm::vec3& vec, const MeshModel& model, const Camera& camera)
{
	glm::mat4x4 transform = camera.GetViewportTrans(viewport_width, viewport_height) * camera.GetProjectionTransformation()
		* glm::inverse(camera.GetViewTransformation()) * model.GetTransformation();
	return ApplyTrans(vec, transform);
}

glm::vec3 Renderer::ApplyTrans(glm::vec3 vec, glm::mat4x4 trans)
{
	return Utils::FromHomogCoords(trans * Utils::ToHomogCoords(vec));
}

void Renderer::CreateBuffers(int w, int h)
{
	CreateOpenglBuffer(); //Do not remove this line.
	color_buffer = new float[3 * w * h];
	ClearColorBuffer(glm::vec3(0.0f, 0.0f, 0.0f));
}

//##############################
//##OpenGL stuff. Don't touch.##
//##############################

// Basic tutorial on how opengl works:
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
// don't linger here for now, we will have a few tutorials about opengl later.
void Renderer::InitOpenglRendering()
{
	// Creates a unique identifier for an opengl texture.
	glGenTextures(1, &gl_screen_tex);

	// Same for vertex array object (VAO). VAO is a set of buffers that describe a renderable object.
	glGenVertexArrays(1, &gl_screen_vtc);

	GLuint buffer;

	// Makes this VAO the current one.
	glBindVertexArray(gl_screen_vtc);

	// Creates a unique identifier for a buffer.
	glGenBuffers(1, &buffer);

	// (-1, 1)__(1, 1)
	//	     |\  |
	//	     | \ | <--- The exture is drawn over two triangles that stretch over the screen.
	//	     |__\|
	// (-1,-1)    (1,-1)
	const GLfloat vtc[] = {
		-1, -1,
		 1, -1,
		-1,  1,
		-1,  1,
		 1, -1,
		 1,  1
	};

	const GLfloat tex[] = {
		0,0,
		1,0,
		0,1,
		0,1,
		1,0,
		1,1 };

	// Makes this buffer the current one.
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// This is the opengl way for doing malloc on the gpu. 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc) + sizeof(tex), NULL, GL_STATIC_DRAW);

	// memcopy vtc to buffer[0,sizeof(vtc)-1]
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);

	// memcopy tex to buffer[sizeof(vtc),sizeof(vtc)+sizeof(tex)]
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);

	// Loads and compiles a sheder.
	GLuint program = InitShader("vshader.glsl", "fshader.glsl");

	// Make this program the current one.
	glUseProgram(program);

	// Tells the shader where to look for the vertex position data, and the data dimensions.
	GLint  vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Same for texture coordinates data.
	GLint  vTexCoord = glGetAttribLocation(program, "vTexCoord");
	glEnableVertexAttribArray(vTexCoord);
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)sizeof(vtc));

	//glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );

	// Tells the shader to use GL_TEXTURE0 as the texture id.
	glUniform1i(glGetUniformLocation(program, "texture"), 0);
}

void Renderer::CreateOpenglBuffer()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, gl_screen_tex);

	// malloc for a texture on the gpu.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, viewport_width, viewport_height, 0, GL_RGB, GL_FLOAT, NULL);
	glViewport(0, 0, viewport_width, viewport_height);
}

void Renderer::SwapBuffers()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, gl_screen_tex);

	// memcopy's colorBuffer into the gpu.
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, viewport_width, viewport_height, GL_RGB, GL_FLOAT, color_buffer);

	// Tells opengl to use mipmapping
	glGenerateMipmap(GL_TEXTURE_2D);

	// Make glScreenVtc current VAO
	glBindVertexArray(gl_screen_vtc);

	// Finally renders the data.
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::ClearColorBuffer(const glm::vec3& color)
{
	for (int i = 0; i < viewport_width; i++)
	{
		for (int j = 0; j < viewport_height; j++)
		{
			PutPixel(i, j, color);
		}
	}
}

void Renderer::Render(const Scene& scene)
{
	int half_width = viewport_width / 2;
	int half_height = viewport_height / 2;

	const Camera& camera = scene.GetCamera(scene.GetActiveCameraIndex());

	// Draw mesh triangles
	for (int i = 0; i < scene.GetModelCount(); i++)
	{
		MeshModel currModel = scene.GetModel(i);
		if (currModel.gui.IsOnScreen)
			DrawModel(currModel, camera);
	}

	DrawWorldFrame(camera);
}

int Renderer::GetViewportWidth() const
{
	return viewport_width;
}

int Renderer::GetViewportHeight() const
{
	return viewport_height;
}

void Renderer::SetViewport(int width, int height)
{
	viewport_height = height;
	viewport_width = width;
	CreateBuffers(width, height);
}