enum LightType
{
	DIFFUSE,
	SPECULAR,
	AMBIENT
};

class LightGUI_Attributes
{
public:
	glm::vec3 DiffuseSourceColor = glm::vec3(1, 1, 1);
	glm::vec3 SpecularSourceColor = glm::vec3(1, 1, 1);
	glm::vec3 AmbientSourceColor = glm::vec3(1, 1, 1);

	float LightWorldTransValue_array[3] = { 0 };
	float LightWorldRotateValue_array[3] = { 0 };

	float LightLocalTransValue_array[3] = { 0 };
	float LightLocalRotateValue_array[3] = { 0 };

	LightType lightType = AMBIENT;

	float LightSize = 20;
};