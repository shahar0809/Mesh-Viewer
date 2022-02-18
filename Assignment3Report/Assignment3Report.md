# Assignment 3
Shahar Tefler & Iris Berger

## 1 - InitOpenGLRendering()
The function maps the texture over the screen.
We assign screen coordinates as 2 triangles, with the following coordinates:

```cpp
// (-1, 1)___(1, 1)
//       |\  |
//       | \ | <--- The texture is drawn over two triangles that stretch over the screen.
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

This function is called once, upon initialization, and creates a mapping for the texture.

So the pipeline is as follows:

`InitOpenGLRendering` defines the mapping of a texture over the screen.

The `Render` function updates the color buffer, which is a buffer the size of the screen
which contatins a color for each pixel.

Right after `Render`, we call `SwapBuffers`, which converts the 
color buffer into a texture.

Finally, we get the color buffer as a texture over the screen.

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

## 4 - Constant color in Fragment Shader
We created a super simple fragment shader, which only has one output:

```cpp
#version 330 core

out vec4 fColor;

void main() 
{ 
   fColor = vec4(0);
} 
```

It outputs constant black color for each pixel.

We also added drawing in fill mode, coloring the model:

```cpp
glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
glBindVertexArray(currModel.GetVAO());
glDrawArrays(GL_TRIANGLES, 0, currModel.GetVerticesCount());
glBindVertexArray(0);
```

## 5 - Update Renderer
We did it in the previous sections.

## 6 - Phong Shading
Phong shading needs the following parameters:
- Model's and Light's ambient, diffuse and specular colors
- Camera direction from point
- Light direction from point
- Specular's alpha

We passed the colors of the different lightings to the shader.

Since the fragment shader goes through every pixel, the camera and light direction should be calculated there.
So, we passed the positions of the light and the camera.

Here is the updated fragment shader:
```cpp
#version 330 core

struct Material
{
	sampler2D textureMap;
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
	float alpha;
};

struct Light
{
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;

	vec3 source;
};

// We set this field's properties from the C++ code
uniform Material material;
uniform Light light;
uniform vec3 cameraPosition;

// Inputs from vertex shader (after interpolation was applied)
in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoords;
in vec3 orig_fragPos;
// The final color of the fragment (pixel)
out vec4 frag_color;

void main()
{
	// Sample the texture-map at the UV coordinates given by 'fragTexCoords'
	vec3 textureColor = vec3(texture(material.textureMap, fragTexCoords));

	vec3 lightDirection = fragPos - light.source;
	vec3 cameraDirection = light.source - cameraPosition;

	vec3 ambient = light.ambientColor * material.ambientColor;
	vec3 diffuse = light.diffuseColor * material.diffuseColor * dot(normalize(lightDirection), normalize(fragNormal));

	vec3 lightReflection = reflect(normalize(lightDirection), normalize(fragNormal));
	float reflectionDegree = clamp(dot(lightReflection, normalize(cameraDirection)), 0.0f, 360.0f);
	float shininessFactor = pow(reflectionDegree, material.alpha);
	vec3 specular = light.specularColor * material.specularColor * shininessFactor; 

	frag_color = vec4(ambient + diffuse + specular, 1);
}

```
Then, we calculated each light type as we did in the previous assignment! Easy Peasy

Now, we need to pass these parameters in the renderer:

```cpp
vertexShader.setUniform("model", currModel->GetTransformation());
vertexShader.setUniform("view", camera.GetViewTransformation());
vertexShader.setUniform("projection", camera.GetProjectionTransformation());

vertexShader.setUniform("light.ambientColor", currLight->GetAmbientColor());
vertexShader.setUniform("light.diffuseColor", currLight->GetDiffuseColor());
vertexShader.setUniform("light.specularColor", currLight->GetSpecularColor());
vertexShader.setUniform("light.source", currLight->GetSource());

vertexShader.setUniform("material.ambientColor", currModel->gui.AmbientReflectionColor);
vertexShader.setUniform("material.diffuseColor", currModel->gui.DiffuseReflectionColor);
vertexShader.setUniform("material.specularColor", currModel->gui.SpecularReflectionColor);
vertexShader.setUniform("material.alpha", currModel->gui.shininess);

vertexShader.setUniform("cameraPosition", camera.getEye());
```


Here is a really beautiful pink/magneta (not purple :) ) with a bit of a blue light:

![Phong](images/phong.png)

[So smooth!!!]


## *fin*
Ah one last time:

Us when something works:

![gif1](images/theoffice2.gif)


Us when we finished just one exercise out of 8:

![gif2](images/a.gif)



