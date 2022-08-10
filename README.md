[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-f059dc9a6f8d3a56e377f745f24479a46679e63a5d9fe6f495e02850cd0d8118.svg)](https://classroom.github.com/online_ide?assignment_repo_id=5969051&assignment_repo_type=AssignmentRepo)
# Computer Graphics 203.3710
## Students Team data

Name: Shahar Tefler

Name: Iris Berger

### Lecturer

*Roi Poranne*

## Course Description

This course provides an introduction to the foundations of Computer Graphics. We will cover the basic mathematical concepts, such as 2D and 3D transformations, shading models, and rendering techniques. The ultimate goal of this course is to develop a basic model viewer.

Students will experiment with modern graphics programming and build a renderer using C++ and OpenGL.

By the end of the course, you will be able to:

* Explain and apply the fundamental mathematical concepts used in rendering.
* Implement a basic graphics pipeline based on rasterization.
* Develop simple graphics programs in C++ using OpenGL and GLSL.

# Results

The images here are the main features of the project, more explanation and features are at each Assignment folder. Some of the code is missing because of deadlines :(

## Transformations and Projections

We implemented Scale, Rotation and Translate transformations, in world and model frame:

![World](Assignment1Report/part3_images/world_trans.gif)
![Model](Assignment1Report/part3_images/model_transf.gif)

And orthographic and perspective projections as well. We added a bunch of features such as bounding box and normals:

![box](Assignment1Report/part3_images/bbox.gif)
![face_normals](Assignment1Report/part3_images/face_norm.gif)
![vertex_normals](Assignment1Report/part3_images/vertex_norm.gif)

## Coloring

We implemented the coloring of each face by the Edge Walking algorithm:

[!edge](Assignment2Report/part1_images/edge_walking.gif)

And then implemented Z-Buffer to avoid faces overlapping:

| Without Z-Buffer      | With Z-Buffer |
| ----------- | ----------- |
| ![Without Z-Buffer Teapot](Assignment2Report/part1_images/teapot.jpeg)    | ![Z-Buffer Teapot](Assignment2Report/part1_images/z-buffer_teapot.jpeg)       |
| ![Without Z-Buffer Cow](Assignment2Report/part1_images/cow.jpeg)    | ![Z-Buffer Cow](Assignment2Report/part1_images/z-buffer_cow.jpeg)       |

## Advenced Coloring and Shading

We implemented all three types of lights: Ambient, Diffuse and Specular.
We also added three types of Shading: Flat, Grouraud and Pong.

### Flat

![a](Assignment2Report/part2_images/flat_shading.png)

### Grouraud

![b](Assignment2Report/part2_images/Grouraud.jpeg)

### Pong

![c](Assignment2Report/part2_images/Phong.jpeg)

## Using Shaders

Finally, we implemented all of the above using OpenGL and shaders.

Pong shading:

![pong](Assignment3Report/images/phong.png)

## Texture Mappings

We implemented three types of texture mappings: Planar, Cylindrical and Spherical:

### Planar

![Planar](Assignment3Report/images/planar.png)

### Cylindrical

![Cylinder](Assignment3Report/images/cylinder.gif)

### Spherical

![Sphere](Assignment3Report/images/sphere.png)

## Cool Post Processing

### Nomral Mapping

![Normal Mapping](Assignment3Report/images/normal_map.jpeg)

### Environment Mapping

![Env Map](Assignment3Report/images/env_map.png)

### Toon Shading

![Toon Shading](Assignment3Report/images/toon_shading.jpeg)
