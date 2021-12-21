#include <glad/glad.h>
#include <glm/glm.hpp>

#include "LightGUI_Attributes.h"

class Light
{
public:
	Light();

	LightGUI_Attributes gui;

	const glm::vec3& GetSource() const;
	void SetSource(const glm::vec3& source);

	const glm::vec3& GetColor() const;

	void SetDiffuse();
	void SetSpecular();
	void SetAmbient();
	const LightType& GetLightType() const;

protected:
	glm::vec3 Source;

	LightType type;

};