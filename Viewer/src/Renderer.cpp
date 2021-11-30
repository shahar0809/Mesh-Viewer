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
	std::cout << "Model Frame " << std::endl;

	glm::vec3 ModelOrigin = camera.GetViewportTrans(model.GetOrigin(), GetViewportWidth(), GetViewportHeight());
	glm::vec3 AxisX = camera.GetViewportTrans(model.GetAxisX(), GetViewportWidth(), GetViewportHeight());
	glm::vec3 AxisY = camera.GetViewportTrans(model.GetAxisY(), GetViewportWidth(), GetViewportHeight());
	glm::vec3 AxisZ = camera.GetViewportTrans(model.GetAxisZ(), GetViewportWidth(), GetViewportHeight());

	std::cout << glm::to_string(ModelOrigin) << std::endl;
	std::cout << glm::to_string(AxisX) << std::endl;
	std::cout << glm::to_string(AxisY) << std::endl;
	std::cout << glm::to_string(AxisZ) << std::endl;

	DrawLine(ModelOrigin, AxisX, { 0, 0, 0 });
	DrawLine(ModelOrigin, AxisY, { 0, 0, 0 });
	DrawLine(ModelOrigin, AxisZ, { 0, 0, 0 });
}

void Renderer::DrawWorldFrame()
{
	glm::vec3 WorldOrigin(GetViewportWidth() / 2, GetViewportHeight() / 2, 1);
	glm::vec3 AxisX(0, 0, 1);
	glm::vec3 AxisY(GetViewportWidth(), GetViewportHeight() / 2, 1);
	glm::vec3 AxisZ(GetViewportWidth() / 2, GetViewportHeight(), 1);

	DrawLine(WorldOrigin, AxisX, { 1, 0, 0 });
	DrawLine(WorldOrigin, AxisY, { 1, 0, 0 });
	DrawLine(WorldOrigin, AxisZ, { 1, 0, 0 });
}

void Renderer::DrawBoundingBox(const MeshModel& model, const Camera& camera)
{
	std::vector<glm::vec3> boundingBox = model.getBoundingBox();

	glm::mat4x4 inverseView = glm::inverse(camera.GetViewTransformation());
	glm::mat4x4 transform = camera.GetProjectionTransformation() * inverseView * model.GetTransformation();

	std::cout << "bounding box" << std::endl;
	for (int i = 0; i < 8; i++)
	{
		glm::vec4 homVec = Utils::ToHomogCoords(boundingBox[i]);
		homVec = transform * homVec;
		boundingBox[i] = camera.GetViewportTrans(Utils::FromHomogCoords(homVec), viewport_width, viewport_height);
		std::cout << glm::to_string(boundingBox[i]) << std::endl;
	}
	std::cout << "end bounding box" << std::endl;

	DrawLine(boundingBox[0], boundingBox[1], model.BoundingBoxColor);
	DrawLine(boundingBox[0], boundingBox[2], model.BoundingBoxColor);
	DrawLine(boundingBox[0], boundingBox[4], model.BoundingBoxColor);

	DrawLine(boundingBox[1], boundingBox[3], model.BoundingBoxColor);
	DrawLine(boundingBox[1], boundingBox[5], model.BoundingBoxColor);

	DrawLine(boundingBox[2], boundingBox[3], model.BoundingBoxColor);
	DrawLine(boundingBox[2], boundingBox[6], model.BoundingBoxColor);

	DrawLine(boundingBox[3], boundingBox[7], model.BoundingBoxColor);

	DrawLine(boundingBox[4], boundingBox[6], model.BoundingBoxColor);
	DrawLine(boundingBox[4], boundingBox[5], model.BoundingBoxColor);

	DrawLine(boundingBox[5], boundingBox[7], model.BoundingBoxColor);

	DrawLine(boundingBox[6], boundingBox[7], model.BoundingBoxColor);
}

void Renderer::DrawModel(const MeshModel& model, const Camera& camera)
{
	std::cout << "*************************" << std::endl;
	DrawModelFrame(model, camera);
	DrawBoundingBox(model, camera);
	std::cout << "*************************" << std::endl;

	for (int i = 0; i < model.GetFacesCount(); i++)
	{
		Face currFace = model.GetFace(i);
		DrawFace(currFace, model, camera);
	}
}

void Renderer::DrawFace(const Face& face, const MeshModel& model, const Camera& camera)
{
	//std::vector<glm::vec3> verticeMesh = model.getVertices();

	glm::mat4x4 inverseView = glm::inverse(camera.GetViewTransformation());
	glm::mat4x4 transform = camera.GetProjectionTransformation() * inverseView * model.GetTransformation();

	//glm::mat4x4 modelTrans = camera.GetCameraInverse() * model.GetTransformation();

	//std::cout << glm::to_string(modelTrans) << std::endl;
	// cout << camera.GetCameraInverse()[0][0] << " " << camera.GetCameraInverse()[1][1] << " " << camera.GetCameraInverse()[2][2] << " " << camera.GetCameraInverse()[3][3] << " " << endl;

	std::vector<glm::vec3> transformedVecs;

	// Apply transformation on vertices
	for (int i = 0; i < 3; i++)
	{
		std::cout << glm::to_string(model.GetVertice(face.GetVertexIndex(i) - 1)) << std::endl;

		glm::vec4 homVec = Utils::ToHomogCoords(model.GetVertice(face.GetVertexIndex(i) - 1));

		std::cout << glm::to_string(homVec) << std::endl;
		glm::vec4 res = transform * homVec;

		std::cout << "res" << std::endl;
		std::cout << glm::to_string(res) << std::endl;

		res[3] = 1;

		// Apply camera viewport transformation
		transformedVecs.push_back(camera.GetViewportTrans(Utils::FromHomogCoords(res), viewport_width, viewport_height));
	}
	std::cout << "trans vecs" << std::endl;
	std::cout << glm::to_string(transformedVecs[0]) << std::endl;
	std::cout << glm::to_string(transformedVecs[1]) << std::endl << std::endl;

	
	DrawLine(transformedVecs[0], transformedVecs[1], model.color);
	DrawLine(transformedVecs[1], transformedVecs[2], model.color);
	DrawLine(transformedVecs[2], transformedVecs[0], model.color);
}

void Renderer::DrawFaceNormals(const Face& face, const MeshModel& model, const Camera& camera)
{
	for (int i = 0; i < model.GetNormalsCount(); i++)
	{
		D
	}
}

glm::vec3 Renderer::TransfVector(const glm::vec3& vec, const MeshModel& model, const Camera& camera)
{
	glm::mat4x4 inverseView = glm::inverse(camera.GetViewTransformation());
	glm::mat4x4 transform = camera.GetProjectionTransformation() * inverseView * model.GetTransformation();

	glm::vec4 homVec = Utils::ToHomogCoords(vec);
	glm::vec4 res = transform * homVec;

	// Apply camera viewport transformation
	 return camera.GetViewportTrans(Utils::FromHomogCoords(res), viewport_width, viewport_height);
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
	// TODO: Replace this code with real scene rendering code
	int half_width = viewport_width / 2;
	int half_height = viewport_height / 2;

	const Camera &camera = scene.GetCamera(scene.GetActiveCameraIndex());
	

	// Draw mesh triangles
	for (int i = 0; i < scene.GetModelCount(); i++)
	{
		MeshModel currModel = scene.GetModel(i);
		if (!currModel.IsOnScreen)
			DrawModel(currModel, camera);
	}

	for (int i = 0; i < scene.GetModelCount(); i++)
	{
		MeshModel currModel = scene.GetModel(i);
		if (currModel.IsOnScreen)
			DrawModel(currModel, camera);
	}

	DrawWorldFrame();
}

int Renderer::GetViewportWidth() const
{
	return viewport_width;
}

int Renderer::GetViewportHeight() const
{
	return viewport_height;
}