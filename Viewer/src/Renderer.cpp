#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

#include "Renderer.h"
#include "InitShader.h"
#include <glm/gtx/string_cast.hpp>

#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)

Renderer::Renderer(int viewport_width, int viewport_height) :
	viewport_width(viewport_width),
	viewport_height(viewport_height)
{
	srand(time(NULL));
	InitOpenglRendering();

	InitBufferZ();
	CreateBuffers(viewport_width, viewport_height);
}

Renderer::~Renderer()
{
	delete[] color_buffer;
	FreeZbuffer();
}

void Renderer::InitBufferZ()
{
	zBuffer = new float* [viewport_width];

	for (int i = 0; i < viewport_width; i++)
	{
		zBuffer[i] = new float[viewport_height];
		for (int j = 0; j < viewport_height; j++)
		{
			zBuffer[i][j] = std::numeric_limits<float>::max();
		}
	}
}

void Renderer::FreeZbuffer()
{
	for (int i = 0; i < viewport_width; i++)
	{
		delete[] zBuffer[i];
	}
	delete[] zBuffer;
}

void Renderer::PutPixel(int i, int j, const glm::vec3& color, float z = 0)
{
	if (i < 0) return; if (i >= viewport_width) return;
	if (j < 0) return; if (j >= viewport_height) return;

	if (zBuffer[i][j] > z)
	{
		zBuffer[i][j] = z;

		color_buffer[INDEX(viewport_width, i, j, 0)] = color.x;
		color_buffer[INDEX(viewport_width, i, j, 1)] = color.y;
		color_buffer[INDEX(viewport_width, i, j, 2)] = color.z;
	}
}

void Renderer::DrawLine(const glm::ivec3& p1, const glm::ivec3& p2, const glm::vec3& color)
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

			PutPixel(x, y, color, ComputeDepth(x, y, p1, p2));
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

			PutPixel(x, y, color, ComputeDepth(x, y, p1, p2));
		}
	}
}

/**
 * @brief Computes z component of a point using linear interpolation.
 * @param x The x coordinate of the point
 * @param y The y coordinate of the point
 * @param p1 Source of line
 * @param p2 Dest of line
 * @return Z coordinate
*/
float Renderer::ComputeDepth(int x, int y, glm::ivec3 p1, glm::ivec3 p2)
{
	glm::vec2 point(x, y);
	float totalDist = glm::distance(glm::vec2(p1), glm::vec2(p2));
	// Compute distance between current point and source and dest
	float d1 = glm::distance(glm::vec2(p1), point), d2 = glm::distance(point, glm::vec2(p2));
	// Normalize distance
	float nd1 = d1 / totalDist, nd2 = d2 / totalDist;
	// Linear interpolation
	return nd1 * p1.z + nd2 * p2.z;
}

/* -------------------------------------------------- Draw differnet objects -----------------------------------------------*/
void Renderer::DrawLineSanityCheck()
{
	glm::ivec3 circleCenter(600, 400, 0);
	glm::ivec3 currentPoint;
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

void Renderer::DrawFilledCircle(glm::vec3 center, float radius)
{

}

void Renderer::DrawFilledRectangle(glm::vec3 point, int width, int height, glm::vec3 color)
{
	for (int i = point.x; i < point.x + width; i++)
	{
		for (int j = point.y; j < point.y + height; j++)
		{
			PutPixel(i, j, color);
		}
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
			glm::vec3 normal = model.GetNormalVertix(currFace.GetVertexIndex(j) - 1) + vertex;
			DrawLine(TransVector(vertex, model, camera), TransVector(normal, model, camera), model.gui.VerticsNormalsColor);
		}
	}
}

/* -------------------------------------------------- Draw models -----------------------------------------------*/
void Renderer::DrawModel(const MeshModel& model, const Scene& scene)
{
	Camera camera = scene.GetCamera(scene.GetActiveCameraIndex());
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
		DrawFace(currFace, model, scene, i);
	}
}

void Renderer::DrawFace(const Face& face, const MeshModel& model, const Scene& scene, const int& index)
{
	EdgeWalking(face, model, scene, index);
}

/* -------------------------------------------------- Post Processing -----------------------------------------------*/

void Renderer::ApplyFog(const Scene& scene, const Camera& camera)
{
	float pixelDistance;
	float fogDistance;
	for (int i = 0; i < viewport_width; i++)
	{
		for (int j = 0; j < viewport_height; j++)
		{
			pixelDistance = glm::distance(camera.getEye(), glm::vec3(i, j, zBuffer[i][j]));
			fogDistance = glm::clamp((scene.fogEnd - pixelDistance) / (scene.fogEnd - scene.fogStart), 0.0f, 1.0f);
			fog_color_buffer[INDEX(viewport_width, i, j, 0)] = glm::clamp(fogDistance * fog_color_buffer[INDEX(viewport_width, i, j, 0)] + (1 - fogDistance) * scene.fogColor, 0.0f, 1.0f)[0];
			fog_color_buffer[INDEX(viewport_width, i, j, 1)] = glm::clamp(fogDistance * fog_color_buffer[INDEX(viewport_width, i, j, 1)] + (1 - fogDistance) * scene.fogColor, 0.0f, 1.0f)[1];
			fog_color_buffer[INDEX(viewport_width, i, j, 2)] = glm::clamp(fogDistance * fog_color_buffer[INDEX(viewport_width, i, j, 2)] + (1 - fogDistance) * scene.fogColor, 0.0f, 1.0f)[2];
		}
	}
}

void Renderer::ApplyBlur()
{

}

/* -------------------------------------------------- Lights & Shading -----------------------------------------------*/
/**
 * @brief Performs Flat Shading.
 * @param model 
 * @param light 
 * @param camera 
 * @param face 
 * @param point 
 * @param index 
 * @return 
*/
glm::vec3 Renderer::FlatShading(const MeshModel& model, const Light& light, const Camera& camera, const Face& face, const glm::vec3& point, const int& index)
{
	// In flat shading, we use normal per face
	glm::vec3 normal = model.GetFaceNormal(index) + model.GetFaceCenter(face);
	return GetVertexColor(model, light, camera, face, point, ApplyTrans(normal, model.GetTransformation()));
}

/**
 * @brief Performs Gouraud Shading.
 * @param model 
 * @param light 
 * @param camera 
 * @param face 
 * @param point 
 * @param index 
 * @return 
*/
glm::vec3 Renderer::GouraudShading(const MeshModel& model, const Light& light, const Camera& camera, const Face& face, const glm::vec3& point, const int& index)
{
	std::vector<glm::vec3> verticesColors;
	std::vector<glm::vec3> vertices;
	/* Get colors of all face vertices, based on lighting type */
	for (int i = 0; i < 3; i++)
	{
		vertices.push_back(TransVector(model.GetVertice(face.GetVertexIndex(i) - 1), model, camera));
		glm::vec3 normal = model.GetNormalVertix(face.GetVertexIndex(i));
		normal = TransVector((normal + model.GetVertice(face.GetVertexIndex(i) - 1)), model, camera);
		normal = normal - vertices[i];
		verticesColors.push_back(GetVertexColor(model, light, camera, face, vertices[i], normal));
	}

	/* Use baycentric coordinates to linearly interpolate the colors of the vertices */
	float area1 = Utils::CalcTriangleArea(vertices[1], vertices[2], point),
		area2 = Utils::CalcTriangleArea(vertices[0], vertices[2], point),
		area3 = Utils::CalcTriangleArea(vertices[0], vertices[1], point);
	float area = area1 + area2 + area3;

	return (area1 / area) * verticesColors[0] + (area2 / area) * verticesColors[1] + (area3 / area) * verticesColors[2];
}

glm::vec3 Renderer::PhongShading(const MeshModel& model, const Light& light, const Camera& camera, const Face& face, const glm::vec3& point, const int& index)
{
	//std::vector<glm::vec3> normalizedNormals;
	std::vector<glm::vec3> vertices;

	for (int i = 0; i < 3; i++)
	{
		vertices.push_back(model.GetVertice(face.GetVertexIndex(i) - 1));
		//normalizedNormals.push_back(glm::normalize(model.GetNormalVertix(i)));
	}
	///* Use baycentric coordinates to linearly interpolate the normals */
	//float area1 = Utils::CalcTriangleArea(vertices[1], vertices[2], point),
	//	area2 = Utils::CalcTriangleArea(vertices[0], vertices[2], point),
	//	area3 = Utils::CalcTriangleArea(vertices[0], vertices[1], point);

	//float area = area1 + area2 + area3;
	//glm::vec3 normal = (area1 / area) * normalizedNormals[0] + (area2 / area) * normalizedNormals[1] + (area3 / area) * normalizedNormals[2];

	//return GetVertexColor(model, light, camera, face, point, normal);

	glm::fvec3 weights = Utils::triangleInterpolation(vertices[0], vertices[1], vertices[2], glm::fvec2(point.x, point.y));
	return glm::vec3(0);
}

/**
 * @brief Calculates the color of a face vertex based on the lighting type.
 * @param model vertix's model
 * @param light 
 * @param camera Active camera
 * @param face vertex's face
 * @param point the vertex
 * @param index Face index in model
 * @return Color of the vertex
*/
glm::vec3 Renderer::GetVertexColor(const MeshModel& model, const Light& light, const Camera& camera, const Face& face, const glm::vec3& point, glm::vec3 normal)
{
	return CalcColor(model, light, camera, face, point, normal);
}

glm::vec3 Renderer::GetColor(const MeshModel& model, const Scene& scene, const Face& face, const glm::vec3& point, const int& index)
{		
	glm::vec3 finalColor(0);
	Camera camera = scene.GetCamera(scene.GetActiveCameraIndex());

	for (int i = 0; i < scene.GetLightCount(); i++)
	{	
		Light currLight = scene.GetLight(i);
		switch (currLight.GetShadingType())
		{
		case (ShadingType::FLAT):
		{
			finalColor = FlatShading(model, currLight, camera, face, point, index);
			break;
		}
		case (ShadingType::GOURAUD):
		{
			finalColor = GouraudShading(model, currLight, camera, face, point, index);
			break;
		}
		case (ShadingType::PHONG):
		{
			finalColor = PhongShading(model, currLight, camera, face, point, index);
			break;
		}
		default:
			throw std::runtime_error("No shading type chosen");
			break;
		}
	}
	return finalColor;
}

glm::vec3 Renderer::CalcColor(const MeshModel& model, const Light& light, const Camera& camera, const Face& face, const glm::vec3& point, glm::vec3 normal)
{
	glm::vec3 finalColor(0);
	glm::vec3 AmbientLight, DiffuseLight, SpecularLight;
	/*Camera camera = scene.GetCamera(scene.GetActiveCameraIndex());*/

		/*Light currLight = scene.GetLight(i);*/
		LightType direction = light.GetLightType();
		if (direction == LightType::LIGHT_POINT) {
			glm::vec3 lightDirection = TransVector(light.GetSource(), light, camera) - TransVector(point, model, camera);
			glm::vec3 cameraDirection = TransVector(camera.getEye(), model, camera) - TransVector(point, model, camera);

			AmbientLight = light.CalcAmbientReflection(model.gui.AmbientReflectionColor);
			DiffuseLight = light.CalcDiffuseReflection(model.gui.DiffuseReflectionColor, normal, lightDirection);
			SpecularLight = light.CalcSpecularReflection(model.gui.AmbientReflectionColor, normal, lightDirection, cameraDirection, model.gui.shininess);
		}
		else if (direction == LightType::DIRECTIONAL_LIGHT) {
			glm::vec3 lightDirection = -1.0f * ApplyTrans(normal, model.GetTransformation());
			glm::vec3 cameraDirection = TransVector(camera.getEye(), model, camera) - TransVector(point, model, camera);

			AmbientLight = light.CalcAmbientReflection(model.gui.AmbientReflectionColor);
			DiffuseLight = light.CalcDiffuseReflection(model.gui.DiffuseReflectionColor, normal, lightDirection);
			SpecularLight = light.CalcSpecularReflection(model.gui.AmbientReflectionColor, normal, lightDirection, cameraDirection, model.gui.shininess);
		}

	/*glm::vec3 AmbientLight = light.CalcAmbientReflection();
	glm::vec3 DiffuseLight = light.CalcDiffuseReflection(model.gui.DiffuseReflectionColor, normal, lightDirection);
	glm::vec3 SpecularLight = light.CalcSpecularReflection(model.gui.AmbientReflectionColor, normal, lightDirection, CameraDirection, Alpha);*/

	return AmbientLight + DiffuseLight + SpecularLight;
}

/* -------------------------------------------------- Edge Walking -----------------------------------------------*/
float Renderer::ComputeDepth(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec2& point)
{
	float area1 = Utils::CalcTriangleArea(v2, v3, point),
		area2 = Utils::CalcTriangleArea(v1, v3, point),
		area3 = Utils::CalcTriangleArea(v1, v2, point);
	float area = area1 + area2 + area3;

	return (area1 / area) * v1.z + (area2 / area) * v2.z + (area3 / area) * v3.z;
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

void Renderer::EdgeWalking(const Face& face, const MeshModel& model, const Scene& scene, const int& index)
{
	std::vector<glm::vec3> transformedVecs;
	Camera camera = scene.GetCamera(scene.GetActiveCameraIndex());

	for (int i = 0; i < 3; i++)
	{
		transformedVecs.push_back(TransVector(model.GetVertice(face.GetVertexIndex(i) - 1), model, camera));
	}

	auto color = Utils::GenerateRandomColor();
	auto boundingRect = model.GetBoundingRectangle(transformedVecs);
	for (int i = boundingRect[0].x; i < boundingRect[1].x; i++)
	{
		for (int j = boundingRect[2].y; j < boundingRect[1].y; j++)
		{
			glm::vec3 currPoint(i, j, 0);
			if (Overlaps(transformedVecs[0], transformedVecs[1], transformedVecs[2], currPoint))
			{
				float z = ComputeDepth(transformedVecs[0], transformedVecs[1], transformedVecs[2], glm::vec2(i, j));
				if (model.gui.Grayscale)
				{
					float min = std::get<2>(Utils::GetMinMax(transformedVecs).first);
					float max = std::get<2>(Utils::GetMinMax(transformedVecs).second);

					float diff = ((z - min) / (max - min));
					glm::vec3 grayScale((diff), (diff), (diff));
					PutPixel(i, j, grayScale, z);
				}
				else
				{
					glm::vec3 currPoint(i, j, z);
					PutPixel(i, j, GetColor(model, scene, face, currPoint, index), z);
				}
			}
		}
	}
}

/* -------------------------------------------------- Transformations & OpenGL -----------------------------------------------*/
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

glm::vec3 Renderer::TransVector(const glm::vec3& vec, const Light& light, const Camera& camera)
{
	glm::mat4x4 transform = camera.GetViewportTrans(viewport_width, viewport_height) * camera.GetProjectionTransformation()
		* glm::inverse(camera.GetViewTransformation()) * light.GetTransformation();
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

	InitBufferZ();
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
			zBuffer[i][j] = std::numeric_limits<float>::max();
		}
	}
}

void Renderer::Render(const Scene& scene)
{
	int half_width = viewport_width / 2;
	int half_height = viewport_height / 2;

	const Camera& camera = scene.GetCamera(scene.GetActiveCameraIndex());
	const Light& light = scene.GetLight(scene.GetActiveLightIndex());

	for (int i = 0; i < scene.GetLightCount(); i++)
	{
		Light currLight = scene.GetLight(i);
		DrawFilledRectangle(TransVector(currLight.GetSource(), currLight, camera), currLight.gui.LightSize, currLight.gui.LightSize, currLight.GetColor());
	}

	// Draw mesh triangles
	for (int i = 0; i < scene.GetModelCount(); i++)
	{
		MeshModel currModel = scene.GetModel(i);
		if (currModel.gui.IsOnScreen)
			DrawModel(currModel, scene);
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