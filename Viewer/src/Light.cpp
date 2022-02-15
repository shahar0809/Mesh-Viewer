#include "Light.h"
/*
Each light will have a direction and source, direction will be used when directional
*/
Light::Light(LightType type=LightType::LIGHT_POINT)
{
	lightType = type;
	Source = glm::vec3(0, 0, 0);
	shadingType = ShadingType::FLAT;
	direction = glm::vec3(1, 0, 0);

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

void Light::SetDirection(const glm::vec3& direction)
{
	this->direction = direction;
}

void Light::SetDirection(float arr[])
{
	this->direction.x = arr[0];
	this->direction.y = arr[1];
	this->direction.z = arr[2];
}

const glm::vec3& Light::GetDirection() const
{
	return direction;
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
	return gui.SpecularSourceColor + gui.AmbientSourceColor + gui.DiffuseSourceColor;
}

void Light::SetPoint()
{
	lightType = LightType::LIGHT_POINT;
}

void Light::SetDirectional()
{
	lightType = LightType::DIRECTIONAL_LIGHT;
}

const LightType& Light::GetLightType() const
{
	return lightType;
}

void Light::SetFlat()
{
	shadingType = ShadingType::FLAT;
}

void Light::SetGouraud()
{
	shadingType = ShadingType::GOURAUD;
}

void Light::SetPhong()
{
	shadingType = ShadingType::PHONG;
}

const ShadingType& Light::GetShadingType() const
{
	return shadingType;
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

/**
 * @brief Calculates Ambient reflection according to the Phong lighting model.
 * @param color Model's Ambient matrial color
 * @param normal Normal of point
 * @param lightDirection Direction of light
 * @return Color according to Ambient reflection
*/
glm::vec3 Light::CalcAmbientReflection(glm::vec3 color) const
{
	return Utils::AdditiveColor(color, AmbientColor);
}

/**
 * @brief Calculates Diffuse reflection according to the Phong lighting model.
 * @param color Model's diffuse matrial color
 * @param normal Normal of point
 * @param lightDirection Direction of light
 * @return Color according to diffuse reflection
*/
glm::vec3 Light::CalcDiffuseReflection(const glm::vec3& color, const glm::vec3& normal, const glm::vec3& lightDirection) const
{
	glm::vec3 res = Utils::AdditiveColor(color, DiffuseColor) * glm::dot(glm::normalize(lightDirection), glm::normalize(normal));

	if (res.x < 0)
	{
		res.x = 0;
	}
	if (res.y < 0)
	{
		res.y = 0;
	}
	if (res.z < 0)
	{
		res.z = 0;
	}
	return res;
}

/**
 * @brief Calculates Specular reflection according to the Phong lighting model.
 * @param color Model's Specular matrial color
 * @param normal Normal of point
 * @param lightDirection Direction of light
 * @return Color according to Specular reflection
*/
glm::vec3 Light::CalcSpecularReflection(const glm::vec3& color, const glm::vec3& normal, const glm::vec3& lightDirection, const glm::vec3& cameraDirection, const float& alpha) const
{
	//glm::vec3& newColor = color * this->SpecularColor;
	glm::vec3 lightReflection = glm::reflect(glm::normalize(lightDirection), glm::normalize(normal));
	float reflectionDegree = glm::clamp(glm::dot(lightReflection, glm::normalize(cameraDirection)), 0.0f, 360.0f);
	float shininessFactor = glm::pow(reflectionDegree, alpha);
	return Utils::AdditiveColor(SpecularColor, color) * shininessFactor;
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