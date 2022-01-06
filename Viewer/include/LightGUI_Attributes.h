enum LightType
{
	DIFFUSE = 0,
	SPECULAR,
	AMBIENT
};

enum ShadingType
{
	FLAT = 0,
	GOURAUD, 
	PHONG
};

enum LightDirection
{
	POINT = 0,
	DIRECTIONAL
};

class LightGUI_Attributes
{
public:
	glm::vec3 DiffuseSourceColor = glm::vec3(1, 1, 1);
	glm::vec3 SpecularSourceColor = glm::vec3(1, 1, 1);
	glm::vec3 AmbientSourceColor = glm::vec3(1, 1, 1);

	float AmbientIntensity = 1;
	float SpecularIntensity = 1;
	float DiffuseIntensity = 1;

	float LightWorldTransValue_array[3] = { 0 };
	float LightWorldRotateValue_array[3] = { 0 };

	float LightLocalTransValue_array[3] = { 0 };
	float LightLocalRotateValue_array[3] = { 0 };

	LightType lightType = AMBIENT;
	ShadingType shadingType = FLAT;

	float LightSize = 20;
};