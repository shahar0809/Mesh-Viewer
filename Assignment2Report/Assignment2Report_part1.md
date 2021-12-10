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
