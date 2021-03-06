#pragma once

#include <vector>
#include <memory>

#include "Camera.h"
#include "MeshModel.h"
#include "Light.h"

using namespace std;

class Scene {
public:
	Scene();

	bool isFog = false;

	void AddModel(const shared_ptr<MeshModel>& mesh_model);
	int GetModelCount() const;
	MeshModel& GetModel(int index) const;
	MeshModel& GetActiveModel() const;

	//void fitModel(shared_ptr<MeshModel>& mesh_model);
	
	void AddCamera(const shared_ptr<Camera>& camera);
	int GetCameraCount() const;
	Camera& GetCamera(int index) const;
	Camera& GetActiveCamera();

	void AddLight(const shared_ptr<Light>& camera);
	int GetLightCount() const;
	Light& GetLight(int index) const;
	Light& GetActiveLight();

	void SetActiveCameraIndex(int index);
	int GetActiveCameraIndex() const;

	void SetActiveModelIndex(int index);
	int GetActiveModelIndex() const;

	void SetActiveLightIndex(int index);
	int GetActiveLightIndex() const;

	glm::vec3 fogColor = glm::vec3(0.4, 0.4, 0.4);
	float fogDensity = 1.0f;
	float fogStart = 90.0f;
	float fogEnd = 95.0f; // check what value should be initialized with
	float fogDistance = 0;

	TextureMapping textureMappingMode = TextureMapping::COLOR;

	
private:
	vector<shared_ptr<MeshModel>> mesh_models;
	vector<shared_ptr<Camera>> cameras;
	vector<shared_ptr<Light>> lights;

	int active_camera_index;
	int active_model_index;
	int active_light_index;
};