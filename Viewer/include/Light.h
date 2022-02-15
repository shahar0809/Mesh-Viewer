#include <glad/glad.h>
#include <glm/glm.hpp>

#include "LightGUI_Attributes.h"
#include "Utils.h"

class Light
{
public:
	Light(LightType type);

	LightGUI_Attributes gui;

	const glm::vec3& GetSource() const;
	void SetSource(const glm::vec3& source);

	const glm::vec3& GetColor() const;

	void SetPoint();
	void SetDirectional();
	const LightType& GetLightType() const;
	void SetFlat();
	void SetGouraud();
	void SetPhong();
	const ShadingType& GetShadingType() const;

	const glm::vec3& GetAmbientColor() const;
	const glm::vec3& GetSpecularColor() const;
	const glm::vec3& GetDiffuseColor() const;

	void SetAmbientColor(const glm::vec3& color);
	void SetSpecularColor(const glm::vec3& color);
	void SetDiffuseColor(const glm::vec3& color);

	void SetDirection(const glm::vec3& direction);
	void SetDirection(float arr[]);
	const glm::vec3& GetDirection() const;

	glm::vec3 CalcAmbientReflection(glm::vec3 color) const;
	glm::vec3 CalcDiffuseReflection(const glm::vec3& color, const glm::vec3& normal, const glm::vec3& lightDirection) const;
	glm::vec3 CalcSpecularReflection(const glm::vec3& color, const glm::vec3& normal, const glm::vec3& lightDirection, const glm::vec3& cameraDirection, const float& alpha) const;

	glm::mat4x4 GetTransformation() const;

	// Apply transformations on both model and world (dependant on previous values)
	void ApplyLocalRotate(double rotateX, double rotateY, double rotateZ);
	void ApplyLocalTranslate(double transX, double transY, double transZ);

	void ApplyWorldRotate(double rotateX, double rotateY, double rotateZ);
	void ApplyWorldTranslate(double transX, double transY, double transZ);

	// Set transformations - without depending on previous values
	void SetLocalRotate(double rotateX, double rotateY, double rotateZ);
	void SetLocalTranslate(double transX, double transY, double transZ);

	void SetWorldRotate(double rotateX, double rotateY, double rotateZ);
	void SetWorldTranslate(double transX, double transY, double transZ);

protected:
	glm::vec3 Source;
	LightType lightType;
	ShadingType shadingType;

	glm::vec3 direction;

	glm::vec3 AmbientColor = glm::vec3(0), SpecularColor = glm::vec3(0), DiffuseColor = glm::vec3(0);

	// Attributes to keep translate and rotation values
	glm::mat4x4 TranslateLocal, RotateLocal, RotateLocalX, RotateLocalY, RotateLocalZ;
	glm::mat4x4 TranslateWorld, RotateWorld, RotateWorldX, RotateWorldY, RotateWorldZ;

	// Store current rotate values (to avoid using arcsin, arccos)
	glm::vec3 LocalRotateVal, WorldRotateVal;
};