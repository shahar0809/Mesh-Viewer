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

	LightType lightType = AMBIENT;

	float LightSize = 10;
};