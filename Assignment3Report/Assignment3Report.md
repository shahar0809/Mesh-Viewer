# Assignment 3
Shahar Tefler & Iris Berger

## 1 - InitOpenGLRendering()
The function maps the texture over the screen.
We assign screen coordinates as 2 triangles, with the following coordinates:

```cpp
// (-1, 1)___(1, 1)
//       |\  |
//       | \ | <--- The exture is drawn over two triangles that stretch over the screen.
//       |__\|
// (-1,-1)   (1,-1)
```

Then, we define the coordinates of the texture:

```cpp
const GLfloat tex[] = {
		0,0,
		1,0,
		0,1,
		0,1,
		1,0,
		1,1 };
```

Then, we bind both coordinate buffers to OpenGL, allocate space on the GPU
and copy the coordinate buffers there.

Now, we initialize the shader, and tell OpenGL to use it:

Finally, we pass the screen and texture coordinates to the shader.

## 2 - Load mesh on the GPU

Firstly, we need to create a shader which accepts vertexes, and outputs them.
```cpp
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texCoords;

out vec2 texCoord;

void main()
{
    gl_Position.xyz = pos;
    gl_Position.z = 0;
    gl_Position.w = 1;
    texCoord = texCoords;
}
```

This basic shader outputs the homogenous coordinates, without depth.

Then, we need to pass the model's vertexs to the shader.

Firstly, we created struct for the layout inputs in the shader:
```cpp
struct Vertex
{
	glm::vec3 pos;
	glm::vec3 textureCoords;
};
```

Then, we created VAO and VBO in MeshModel for the vertexes, and specified
the offsets of the inputs in the layout:

```cpp
/* Initialize VAO and VBO for the vertexs */
glGenVertexArrays(1, &vao);
glGenBuffers(1, &vbo);

glBindVertexArray(vao);
glBindBuffer(GL_ARRAY_BUFFER, vbo);
glBufferData(GL_ARRAY_BUFFER, modelVertexes.size() * sizeof(Vertex), &modelVertexes[0], GL_STATIC_DRAW);

/* Initialize input positions of Vertex for shader */
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
glEnableVertexAttribArray(0);

glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
glEnableVertexAttribArray(1);

glBindVertexArray(0)
```

Finally, we pass the vertexes in Renderer, and draw it wireframe:

```cpp
const Camera& camera = scene.GetCamera(scene.GetActiveCameraIndex());
	const Light& light = scene.GetLight(scene.GetActiveLightIndex());

	for (int i = 0; i < scene.GetModelCount(); i++)
	{
		MeshModel& currModel = scene.GetModel(i);
		if (currModel.gui.IsOnScreen)
		{
			vertexShader.use();
			texture.bind(0);

			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glBindVertexArray(currModel.GetVAO());
			glDrawArrays(GL_TRIANGLES, 0, currModel.GetVerticesCount());
			glBindVertexArray(0);

			texture.unbind(0);
		}
	}
}
```
  
## 3 - Supporting transformations all over again :)
In order to support transformations, we need to pass the model's 
and camera's transformations (MVP) to the vertex shader.

As said in the lecture, we will pass it using uniforms. The shader now looks like this:

```cpp
#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texCoords;

// MVP for transformations
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 texCoord;

void main()
{
    gl_Position = projection * view * model * vec4(pos, 1.0f);
}
```

Then, we need to pass the transformations as uniforms in the Render() function:

```cpp
const Camera& camera = scene.GetCamera(scene.GetActiveCameraIndex());
	const Light& light = scene.GetLight(scene.GetActiveLightIndex());

	for (int i = 0; i < scene.GetModelCount(); i++)
	{
		MeshModel& currModel = scene.GetModel(i);
		if (currModel.gui.IsOnScreen)
		{
			vertexShader.use();

			// Pass transformatins to shader (MVP)
			vertexShader.setUniform("model", currModel.GetTransformation());
			vertexShader.setUniform("view", camera.GetViewTransformation());
			vertexShader.setUniform("projection", camera.GetProjectionTransformation());

			texture.bind(0);

			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glBindVertexArray(currModel.GetVAO());
			glDrawArrays(GL_TRIANGLES, 0, currModel.GetVerticesCount());
			glBindVertexArray(0);

			texture.unbind(0);
		}
	}
}
```



![Rectangles](part1_images/color_rectangles.gif)


## *fin*
This is a tradition by now:

![gif](part1_images/happy_gif.gif)
