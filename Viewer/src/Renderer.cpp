#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

#include "Renderer.h"
#include "InitShader.h"

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
				if ((deltaX < 0 && deltaY < 0) || (deltaX > 0 && deltaY > 0))
				{
					y = y + 1;
				}
				// Otherwise, we'll decrease it (in the direction of the line)
				else
				{
					y = y - 1;
				}
				decisionParam_X = decisionParam_X + 2 * (abs_deltaY - abs_deltaX);
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
				if ((deltaX < 0 && deltaY < 0) || (deltaX > 0 && deltaY > 0))
				{
					x = x + 1;
				}
				// Otherwise, we'll decrease it (in the direction of the line)
				else
				{
					x = x - 1;
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

void Renderer::DrawingRectangle(int length, int width, int bold, glm::ivec2& startingPoint) {
	//Drowing a Rectangle with a smoll filld piece on the to op it.
	glm::ivec2 nextPoint(startingPoint.x + length, startingPoint.y);
	glm::vec3 color({ 0, 0, 0 });
	

	for (int i = 0; i < bold + 1; i++) {
		if (i == 0) {
			DrawLine(startingPoint, nextPoint, color);
			nextPoint.y = nextPoint.y + width - bold;
			startingPoint.y = startingPoint.y + width - bold;
		}
		else {
			DrawLine(startingPoint, nextPoint, color);
			nextPoint.y = nextPoint.y + 1;
			startingPoint.y = startingPoint.y + 1;
		}
	}
	nextPoint.y = nextPoint.y - 1;
	startingPoint.y = startingPoint.y - 1;
	startingPoint.x = nextPoint.x;
	startingPoint.y = nextPoint.y - width;
	DrawLine(startingPoint, nextPoint, color);

	nextPoint.x = startingPoint.x - length;
	startingPoint.x = nextPoint.x;
	DrawLine(startingPoint, nextPoint, color);
}

void Renderer::DrawingCandle(const glm::ivec2& startPoint, const glm::ivec2& point1, const glm::ivec2& point2, const glm::ivec2& point3, const glm::ivec2& point4, const glm::ivec2& point5, const glm::ivec2& point6) {
	// print the flame of the candle
	glm::vec3 color({255, 0, 0 });
	DrawLine(startPoint, point1, color);
	DrawLine(startPoint, point2, color);
	DrawLine(point1, point3, color);
	DrawLine(point2, point4, color);
	DrawLine(point5, point6, color);
}

void Renderer::Draw() {
	//drawing a cake

	int rectangleIndex = 10;
	int rectangleWidth = 200;
	int rectanglength = 300;
	int length = 25;
	int width = 125;
	int betweenRectangles = 50;
	 
	int candleY = rectangleIndex + rectangleWidth + rectangleWidth * 2 / 3 - 1;

	int startX = rectangleIndex * 8 + betweenRectangles + betweenRectangles / 4 + length / 2;
	int startY = rectangleIndex + rectangleWidth + 2 * width + 2 * length;
	int startAdd = betweenRectangles / 4 + length + 1;

	int point3X = rectangleIndex * 8 + betweenRectangles + betweenRectangles / 4;
	int point3Y = candleY + width;

	int point1X = rectangleIndex * 8 + betweenRectangles + betweenRectangles / 4 - betweenRectangles / 16;
	int point1Y = startY - width * 3 / 16;
	 
	int point2X = rectangleIndex * 8 + betweenRectangles + betweenRectangles / 4 + length + betweenRectangles / 16;
	int point2Y = startY - width * 3 / 16;

	int point5Y = startY - width / 3;
	int point6Y = startY - width / 16;
	
	DrawingRectangle(rectanglength, rectangleWidth, rectangleWidth / 8, glm::ivec2(rectangleIndex * 8, rectangleIndex));
	DrawingRectangle(rectanglength * 2 / 3, rectangleWidth * 2 / 3, rectangleWidth / 12, glm::ivec2(rectangleIndex * 8 + betweenRectangles , rectangleIndex + rectangleWidth));
	
	DrawingRectangle(length, width, width / 8, glm::ivec2(rectangleIndex * 8 + betweenRectangles + betweenRectangles / 4, candleY));
	DrawingRectangle(length, width, width / 8, glm::ivec2(rectangleIndex * 8 + betweenRectangles + 2 * betweenRectangles / 4 + length, candleY));
	DrawingRectangle(length, width, width / 8, glm::ivec2(rectangleIndex * 8 + betweenRectangles + 3 * betweenRectangles / 4 + 2 * length, candleY));
	DrawingRectangle(length, width, width / 8, glm::ivec2(rectangleIndex * 8 + betweenRectangles + 4 * betweenRectangles / 4 + 3 * length, candleY));
	DrawingRectangle(length, width, width / 8, glm::ivec2(rectangleIndex * 8 + betweenRectangles + 5 * betweenRectangles / 4 + 4 * length, candleY));
	
	DrawingCandle(glm::ivec2(startX, startY), glm::ivec2(point1X, point1Y), glm::ivec2(point2X, point2Y), glm::ivec2(point3X, point3Y), glm::ivec2(point3X + length, point3Y), glm::vec2(startX, point5Y), glm::vec2(startX, point6Y));
	DrawingCandle(glm::ivec2(startX + startAdd, startY), glm::ivec2(point1X + startAdd, point1Y), glm::ivec2(point2X + startAdd, point2Y), glm::ivec2(point3X + startAdd, point3Y), glm::ivec2(point3X + startAdd + length, point3Y), glm::vec2(startX + startAdd, point5Y), glm::vec2(startX + startAdd, point6Y));
	DrawingCandle(glm::ivec2(startX + 2 * startAdd, startY), glm::ivec2(point1X + 2 * startAdd, point1Y), glm::ivec2(point2X + 2 * startAdd - 1, point2Y), glm::ivec2(point3X + 2 * startAdd - 1, point3Y), glm::ivec2(point3X + 2 * startAdd + length - 1, point3Y), glm::vec2(startX + 2 * startAdd, point5Y), glm::vec2(startX + 2 * startAdd, point6Y));
	DrawingCandle(glm::ivec2(startX + 3 * startAdd, startY), glm::ivec2(point1X + 3 * startAdd, point1Y), glm::ivec2(point2X + 3 * startAdd - 1, point2Y), glm::ivec2(point3X + 3 * startAdd - 1, point3Y), glm::ivec2(point3X + 3 * startAdd + length - 1, point3Y), glm::vec2(startX + 3 * startAdd, point5Y), glm::vec2(startX + 3 * startAdd, point6Y));
	DrawingCandle(glm::ivec2(startX + 4 * startAdd, startY), glm::ivec2(point1X + 4 * startAdd, point1Y), glm::ivec2(point2X + 4 * startAdd - 2, point2Y), glm::ivec2(point3X + 4 * startAdd - 2, point3Y), glm::ivec2(point3X + 4 * startAdd + length - 2, point3Y), glm::vec2(startX + 4 * startAdd, point5Y), glm::vec2(startX + 4 * startAdd, point6Y));
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

	// (-1, 1)____(1, 1)
	//	     |\  |
	//	     | \ | <--- The exture is drawn over two triangles that stretch over the screen.
	//	     |__\|
	// (-1,-1)    (1,-1)
	const GLfloat vtc[]={
		-1, -1,
		 1, -1,
		-1,  1,
		-1,  1,
		 1, -1,
		 1,  1
	};

	const GLfloat tex[]={
		0,0,
		1,0,
		0,1,
		0,1,
		1,0,
		1,1};

	// Makes this buffer the current one.
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// This is the opengl way for doing malloc on the gpu. 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc)+sizeof(tex), NULL, GL_STATIC_DRAW);

	// memcopy vtc to buffer[0,sizeof(vtc)-1]
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);

	// memcopy tex to buffer[sizeof(vtc),sizeof(vtc)+sizeof(tex)]
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);

	// Loads and compiles a sheder.
	GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );

	// Make this program the current one.
	glUseProgram(program);

	// Tells the shader where to look for the vertex position data, and the data dimensions.
	GLint  vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition,2,GL_FLOAT,GL_FALSE,0,0 );

	// Same for texture coordinates data.
	GLint  vTexCoord = glGetAttribLocation( program, "vTexCoord" );
	glEnableVertexAttribArray( vTexCoord );
	glVertexAttribPointer( vTexCoord,2,GL_FLOAT,GL_FALSE,0,(GLvoid *)sizeof(vtc) );

	//glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );

	// Tells the shader to use GL_TEXTURE0 as the texture id.
	glUniform1i(glGetUniformLocation(program, "texture"),0);
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
	// draw circle
	DrawLineSanityCheck();
	//DrawLine(glm::ivec2(500, 500), glm::ivec2(450, 500), { 0, 1, 0 });
	Draw();

}

int Renderer::GetViewportWidth() const
{
	return viewport_width;
}

int Renderer::GetViewportHeight() const
{
	return viewport_height;
}