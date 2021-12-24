#include <glad/glad.h>
#include <glm/glm.hpp>

#include "LightGUI_Attributes.h"
#include "Utils.h"

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
	LightType type;

	// Attributes to keep translate and rotation values
	glm::mat4x4 TranslateLocal, RotateLocal, RotateLocalX, RotateLocalY, RotateLocalZ;
	glm::mat4x4 TranslateWorld, RotateWorld, RotateWorldX, RotateWorldY, RotateWorldZ;

	// Store current rotate values (to avoid using arcsin, arccos)
	glm::vec3 LocalRotateVal, WorldRotateVal;
};