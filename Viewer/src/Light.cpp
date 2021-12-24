#include "Light.h"

Light::Light()
{
	Source = glm::vec3(0, 0, 0);
	type = AMBIENT;

	WorldRotateVal = glm::vec3(0);
	LocalRotateVal = glm::vec3(0);

	TranslateLocal = glm::mat4x4{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	TranslateWorld = glm::mat4x4{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	RotateLocalX = glm::mat4x4{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	RotateLocalY = glm::mat4x4{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	RotateLocalZ = glm::mat4x4{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	RotateLocal = glm::mat4x4{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	RotateWorldX = glm::mat4x4{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	RotateWorldY = glm::mat4x4{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	RotateWorldZ = glm::mat4x4{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	RotateWorld = glm::mat4x4{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
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

const float& Light::GetAmbientIntensity() const
{
	return AmbientIntensity;
}

const float& Light::GetSpecularIntensity() const
{
	return SpecularIntensity;
}

const float& Light::GetDiffuseIntensity() const
{
	return DiffuseIntensity;
}

void Light::SetAmbientIntensity(const float& intensity)
{
	this->AmbientIntensity = intensity;
}

void Light::SetSpecularIntensity(const float& intensity)
{
	this->SpecularIntensity = intensity;
}

void Light::SetDiffuseIntensity(const float& intensity)
{
	this->DiffuseIntensity = intensity;
}

const glm::vec3& Light::GetAmbientColor() const
{
	return AmbientColor;
}

const glm::vec3& Light::GetSpecularColor() const
{
	return SpecularColor;
}

const glm::vec3& Light::GetDiffuseColor() const
{
	return DiffuseColor;
}

void Light::SetAmbientColor(const glm::vec3& color)
{
	this->AmbientColor = color;
}

void Light::SetSpecularColor(const glm::vec3& color)
{
	this->SpecularColor = color;
}

void Light::SetDiffuseColor(const glm::vec3& color)
{
	this->DiffuseColor = color;
}

//##############################
//## Controling position      ##
//##############################

glm::mat4x4 Light::GetTransformation() const
{
	glm::mat4x4 ModelTrans = TranslateLocal * RotateLocal;
	glm::mat4x4 WorldTrans = TranslateWorld * RotateWorld;

	return WorldTrans * ModelTrans;
}

void Light::ApplyLocalRotate(double rotateX, double rotateY, double rotateZ)
{
	LocalRotateVal.x += rotateX;
	LocalRotateVal.y += rotateY;
	LocalRotateVal.z += rotateZ;

	double radiansValX = Utils::ToRadians(LocalRotateVal.x);

	RotateLocalX[1][1] = cos(radiansValX);
	RotateLocalX[1][2] = -sin(radiansValX);
	RotateLocalX[2][1] = sin(radiansValX);
	RotateLocalX[2][2] = cos(radiansValX);

	double radiansValY = Utils::ToRadians(LocalRotateVal.y);

	RotateLocalY[0][0] = cos(radiansValY);
	RotateLocalY[0][2] = sin(radiansValY);
	RotateLocalY[2][0] = -sin(radiansValY);
	RotateLocalY[2][2] = cos(radiansValY);

	double radiansValZ = Utils::ToRadians(LocalRotateVal.z);

	RotateLocalZ[0][0] = cos(radiansValZ);
	RotateLocalZ[1][0] = -sin(radiansValZ);
	RotateLocalZ[0][1] = sin(radiansValZ);
	RotateLocalZ[1][1] = cos(radiansValZ);

	RotateLocal = RotateLocalZ * RotateLocalY * RotateLocalX;
}

void Light::ApplyLocalTranslate(double transX, double transY, double transZ)
{
	// Add current translation parameters (in last column) by new parameters
	TranslateLocal[3][0] += transX;
	TranslateLocal[3][1] += transY;
	TranslateLocal[3][2] += transZ;
}

void Light::ApplyWorldRotate(double rotateX, double rotateY, double rotateZ)
{
	WorldRotateVal.x += rotateX;
	WorldRotateVal.y += rotateY;
	WorldRotateVal.z += rotateZ;

	double radiansValX = Utils::ToRadians(WorldRotateVal.x);

	RotateWorldX[1][1] = cos(radiansValX);
	RotateWorldX[1][2] = -sin(radiansValX);
	RotateWorldX[2][1] = sin(radiansValX);
	RotateWorldX[2][2] = cos(radiansValX);

	double radiansValY = Utils::ToRadians(WorldRotateVal.y);

	RotateWorldY[0][0] = cos(radiansValY);
	RotateWorldY[0][2] = sin(radiansValY);
	RotateWorldY[2][0] = -sin(radiansValY);
	RotateWorldY[2][2] = cos(radiansValY);

	double radiansValZ = Utils::ToRadians(WorldRotateVal.z);

	RotateWorldZ[0][0] = cos(radiansValZ);
	RotateWorldZ[1][0] = -sin(radiansValZ);
	RotateWorldZ[0][1] = sin(radiansValZ);
	RotateWorldZ[1][1] = cos(radiansValZ);

	RotateWorld = RotateWorldZ * RotateWorldY * RotateWorldX;
}

void Light::ApplyWorldTranslate(double transX, double transY, double transZ)
{
	// Add current translation parameters (in last column) by new parameters
	TranslateLocal[3][0] += transX;
	TranslateLocal[3][1] += transY;
	TranslateLocal[3][2] += transZ;
}

void Light::SetLocalRotate(double rotateX, double rotateY, double rotateZ)
{
	LocalRotateVal.x = rotateX;
	LocalRotateVal.y = rotateY;
	LocalRotateVal.z = rotateZ;

	double radiansValX = Utils::ToRadians(LocalRotateVal.x);

	RotateLocalX[1][1] = cos(radiansValX);
	RotateLocalX[1][2] = -sin(radiansValX);
	RotateLocalX[2][1] = sin(radiansValX);
	RotateLocalX[2][2] = cos(radiansValX);

	double radiansValY = Utils::ToRadians(LocalRotateVal.y);

	RotateLocalY[0][0] = cos(radiansValY);
	RotateLocalY[0][2] = sin(radiansValY);
	RotateLocalY[2][0] = -sin(radiansValY);
	RotateLocalY[2][2] = cos(radiansValY);

	double radiansValZ = Utils::ToRadians(LocalRotateVal.z);

	RotateLocalZ[0][0] = cos(radiansValZ);
	RotateLocalZ[1][0] = -sin(radiansValZ);
	RotateLocalZ[0][1] = sin(radiansValZ);
	RotateLocalZ[1][1] = cos(radiansValZ);

	RotateLocal = RotateLocalZ * RotateLocalY * RotateLocalX;
}

void Light::SetLocalTranslate(double transX, double transY, double transZ)
{
	// Set current translation parameters (in last column) to be new parameters
	TranslateLocal[3][0] = transX;
	TranslateLocal[3][1] = transY;
	TranslateLocal[3][2] = transZ;
}

void Light::SetWorldRotate(double rotateX, double rotateY, double rotateZ)
{
	WorldRotateVal.x = rotateX;
	WorldRotateVal.y = rotateY;
	WorldRotateVal.z = rotateZ;

	double radiansValX = Utils::ToRadians(WorldRotateVal.x);

	RotateWorldX[1][1] = cos(radiansValX);
	RotateWorldX[1][2] = -sin(radiansValX);
	RotateWorldX[2][1] = sin(radiansValX);
	RotateWorldX[2][2] = cos(radiansValX);

	double radiansValY = Utils::ToRadians(WorldRotateVal.y);

	RotateWorldY[0][0] = cos(radiansValY);
	RotateWorldY[0][2] = sin(radiansValY);
	RotateWorldY[2][0] = -sin(radiansValY);
	RotateWorldY[2][2] = cos(radiansValY);

	double radiansValZ = Utils::ToRadians(WorldRotateVal.z);

	RotateWorldZ[0][0] = cos(radiansValZ);
	RotateWorldZ[1][0] = -sin(radiansValZ);
	RotateWorldZ[0][1] = sin(radiansValZ);
	RotateWorldZ[1][1] = cos(radiansValZ);

	RotateWorld = RotateWorldZ * RotateWorldY * RotateWorldX;
}

void Light::SetWorldTranslate(double transX, double transY, double transZ)
{
	// Set current translation parameters (in last column) to be new parameters
	TranslateWorld[3][0] = transX;
	TranslateWorld[3][1] = transY;
	TranslateWorld[3][2] = transZ;
}