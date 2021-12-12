# Assignment 2 - Part 1
*Shahar Tefler & Iris Berger*

## Bounding Rectangles per Face
### Drawing the bounding rectangle
We drew the bounding box around each face the same as the bounding box of the model (except the z-axis):

```cpp
std::vector<glm::vec3> MeshModel::GetBoundingRectangle(std::vector<glm::vec3> vectors) const
{
	auto minMax = GetMinMax(vectors);
	std::vector<glm::vec3> points;

	glm::vec3 p1(std::get<0>(minMax.first), std::get<1>(minMax.second), std::get<2>(minMax.first));
	glm::vec3 p2(std::get<0>(minMax.second), std::get<1>(minMax.second), std::get<2>(minMax.first));
	glm::vec3 p3(std::get<0>(minMax.second), std::get<1>(minMax.first), std::get<2>(minMax.first));
	glm::vec3 p4(std::get<0>(minMax.first), std::get<1>(minMax.first), std::get<2>(minMax.first));

	points.push_back(p1);
	points.push_back(p2);
	points.push_back(p3);
	points.push_back(p4);

	return points;
}
```

### Drawing with diferent colors
We created a function that returnes a random color, and then, drew each bounding rectangle
with a random color:

```cpp
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
```

The result:

![World Trans](part1_images/color_rectangles.gif)

## Edge Walking
### The Algorithm

We decided to implement the edge walking algorithm. 
In this algorithm we want to find for every point whether it inside the triangle or not. 
We did a function that gets two points that form a triangle edge, and a third point that we want the position related to this edge. 
The function returns a negative value if the point is on the left of the line, a positive value when the point is on the right side of this line, and zero when it is on the line.

```cpp
float Renderer::EdgeFunction(glm::vec3 v1, glm::vec3 v2, glm::vec3 p)
{
	return (p.x - v1.x) * (v2.y - v1.y) - (p.y - v1.y) * (v2.x - v1.x);
}
```

Now we need to check whether a given point is inside a triangle.
In The function, we get the there vertices of the triangle and the point to check for overlapping. From the three given vertices, we can find the triangle's edges. Then using the first function we talk about, we can decide if the given point is inside the triangle, check if the value is positive or the point is on the left triangle's edge. 

```cpp
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
```

The last thing left to do is start painting all the model's faces. 
We do it by going over each face in the model and coloring every pixel inside the face. 
We find the right pixels to paint by going over all the pixels in the face bounding rectangles (we did it in section 1) and chacking using the overlap function if it is inside the face and then coloring only these pixels. 

```cpp
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
```

Each face gets a different random color which the function gets from the DrawFace function in Renderer.

The result:
