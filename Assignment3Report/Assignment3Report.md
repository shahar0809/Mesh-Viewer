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
in  vec2 vTexCoord;
in  vec2 vPosition;

out vec2 texCoord;

void main()
{
    gl_Position.xy = vPosition;
    gl_Position.z=0;
    gl_Position.w=1;
    texCoord = vTexCoord;
}
```

This basic shader outputs the homogenous coordinates, without depth.

Then, we need to pass the model's vertexs to the shader.


## 
![Rectangles](part1_images/color_rectangles.gif)


## *fin*
This is a tradition by now:

![gif](part1_images/happy_gif.gif)
