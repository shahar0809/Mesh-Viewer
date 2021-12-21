#include "Light.h"

Light::Light()
{
	Source = glm::vec3(0);
}

const glm::vec3& Light::GetSource() const
{
	return Source;
}

void Light::SetSource(const glm::vec3& source)
{
	this->Source = source;
}

const glm::vec3& Light::GetColor() const
{
	switch (type)
	{
	case DIFFUSE:
		return gui.DiffuseSourceColor;
		break;
	case SPECULAR:
		return gui.SpecularSourceColor;
		break;
	case AMBIENT:
		return gui.AmbientSourceColor;
		break;
	default:
		break;
	}
}

void Light::SetDiffuse()
{
	type = DIFFUSE;
}

void Light::SetSpecular()
{
	type = SPECULAR;
}

void Light::SetAmbient()
{
	type = AMBIENT;
}

const LightType& Light::GetLightType() const
{
	return type;
}