#define _USE_MATH_DEFINES
#include <cmath>
#include <imgui/imgui.h>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nfd.h>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Renderer.h"
#include "Scene.h"
#include "Utils.h"

/**
 * Fields
 */
bool show_demo_window = false;
bool show_another_window = true;

/**
* Constants
*/
const int mouse_offset = 5;

const float scaleMax = 50.0f, scaleMin = -50.0f;
const float scaleMaxWorld = 5.0f, scaleMinWorld = -5.0;
const float translateMax = 50.0f, translateMin = -50.0f;
const float rotateMax = 360.0f, rotateMin = 0;
const float cameraMax = 30.0f, cameraMin = -30.0f;
const float widthMin = 1.0f, widthMax = 30.0f;
const float FovMin = -90.0f, FovMax = 0.0f;
const float AspectMin = 0.25f, AspectMax = 2.0f;
const float LookAtMax = 3.0f, LookAtMin = -3.0f;

const int ORTHO = 0, PERSPECTIVE = 1;

static int SCREEN_ASPECT = 80;
static int width = 0, height = 0;

/**
* Colors
*/
static const glm::vec3 backgroundColor = clear_color;

// ASCII values for keyboard events
static const int S_KEY_ASCII = int('S'),
				 W_KEY_ASCII = int('W'),
				 A_KEY_ASCII = int('A'),
				 U_KEY_ASCII = int('U'),
				 D_KEY_ASCII = int('D');

static const int ZOOM_IN_KEY_ASCII = int('['),
				 ZOOM_OUT_KEY_ASCII = int(']');

static float ModelScaleValue_array[5][3] = { {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1} };
static float ModelTransValue_array[5][3] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0} };
static float ModelRotateValue_array[5][3] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0} };

static float WorldScaleValue_array[5][3] = { {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1} };
static float WorldTransValue_array[5][3] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0} };
static float WorldRotateValue_array[5][3] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0} };

static float CameraTransValue_array[5][3] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0} };
static float CameraRotateValue_array[5][3] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0} };

static float CameraWorldTransValue_array[5][3] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0} };
static float CameraWorldRotateValue_array[5][3] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0} };

//static float CameraController_array[4][3] = { {30, 30, 30}, {30, 30, 30} , {30, 30, 30}};
static float ortho_array[3][3] = { {30, 30, 30}, {30, 30, 30} , {30, 30, 30} };
static float perspective_array[3][3] = { {30, 30, 30}, {30, 30, 30} , {30, 30, 30} };
static float eye_array[3][3] = { {0, 0, 0}, {0, 0, 0} , {0, 0, 0} };
static float at_array[3][3] = { {0, 0, 1}, {0, 0, 1} , {0, 0, 1} };
static float up_array[3][3] = { {0, 1, 0}, {0, 1, 0} , {0, 1, 0} };

static int cameraMode = ORTHO;
static float Fovy = -30.0f;
static float aspect = 1;
static float OrthoWidth = 1;
int windowWidth = 1280, windowHeight = 720;

/**
 * Function declarations
 */
static void GlfwErrorCallback(int error, const char* description);
GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name);
ImGuiIO& SetupDearImgui(GLFWwindow* window);
void StartFrame();
void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io);
void Cleanup(GLFWwindow* window);
void DrawImguiMenus(ImGuiIO& io, Scene& scene);

/**
 * Function implementation
 */
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	// TODO: Handle mouse scroll here
}

int main(int argc, char** argv)
{
	const std::string base_path = "C: ../../../Data/";
	
	GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, "Mesh Viewer");
	if (!window)
		return 1;

	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

	Renderer renderer = Renderer(frameBufferWidth, frameBufferHeight);
	Scene scene = Scene();

	width = renderer.GetViewportWidth(), height = renderer.GetViewportHeight();

	// Initialize camera controllers
	for (int i = 0; i < 3; i++) 
	{
		ortho_array[i][0] = width / SCREEN_ASPECT;
		ortho_array[i][1] = height / SCREEN_ASPECT;
		ortho_array[i][2] = 15;

		perspective_array[i][0] = width / SCREEN_ASPECT;
		perspective_array[i][1] = height / SCREEN_ASPECT;
		perspective_array[i][2] = 15;
	}
	
	/* Load a few models */
	std::shared_ptr<MeshModel> model1 = Utils::LoadMeshModel(base_path + "demo.obj");
	scene.AddModel(model1);
	//std::shared_ptr<MeshModel> model2 = Utils::LoadMeshModel(base_path + "camera.obj");
	//scene.AddModel(model2);
	//std::shared_ptr<MeshModel> model3 = Utils::LoadMeshModel(base_path + "camera.obj");
	//scene.AddModel(model3);
	//std::shared_ptr<MeshModel> model4 = Utils::LoadMeshModel(base_path + "camera.obj");
	//scene.AddModel(model4);

	/* Load a camera */
	std::shared_ptr<Camera> camera1 = std::make_shared<Camera>();;
	scene.AddCamera(camera1);
	std::shared_ptr<Camera> camera2 = std::make_shared<Camera>();;
	scene.AddCamera(camera2);
	
	ImGuiIO& io = SetupDearImgui(window);
	glfwSetScrollCallback(window, ScrollCallback);
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		StartFrame();
		DrawImguiMenus(io, scene);
		RenderFrame(window, scene, renderer, io);
	}

	Cleanup(window);
	return 0;
}

static void GlfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name)
{
	glfwSetErrorCallback(GlfwErrorCallback);
	if (!glfwInit())
		return NULL;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if APPLE
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(w, h, window_name, NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync
						 // very importent!! initialization of glad
						 // https://stackoverflow.com/questions/48582444/imgui-with-the-glad-opengl-loader-throws-segmentation-fault-core-dumped

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	return window;
}

ImGuiIO& SetupDearImgui(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();
	return io;
}

void StartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ChangeFrameSize(int width, int height, Renderer& renderer)
{
	windowWidth = width;
	windowHeight = height;

	glViewport(0, 0, windowWidth, windowHeight);
	renderer.SetViewport(windowWidth, windowHeight);
}

void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io)
{
	ImGui::Render();
	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

	if (frameBufferWidth != renderer.GetViewportWidth() || frameBufferHeight != renderer.GetViewportHeight())
	{
		ChangeFrameSize(frameBufferWidth, frameBufferHeight, renderer);
	}

	if (!io.WantCaptureKeyboard)
	{
		// The key is down (S)
		if (io.KeysDown[S_KEY_ASCII])
		{
			ModelTransValue_array[scene.GetActiveModelIndex()][1] -= mouse_offset;
			scene.GetActiveModel().SetModelTranslate(ModelTransValue_array[scene.GetActiveModelIndex()][0], ModelTransValue_array[scene.GetActiveModelIndex()][1], ModelTransValue_array[scene.GetActiveModelIndex()][2]);
		}
		// The key is up (W)
		else if (io.KeysDown[W_KEY_ASCII])
		{
			ModelTransValue_array[scene.GetActiveModelIndex()][1] += mouse_offset;
			scene.GetActiveModel().SetModelTranslate(ModelTransValue_array[scene.GetActiveModelIndex()][0], ModelTransValue_array[scene.GetActiveModelIndex()][1], ModelTransValue_array[scene.GetActiveModelIndex()][2]);
		}
		// The key is left (A)
		else if (io.KeysDown[A_KEY_ASCII])
		{
		ModelTransValue_array[scene.GetActiveModelIndex()][0] -= mouse_offset;
		scene.GetActiveModel().SetModelTranslate(ModelTransValue_array[scene.GetActiveModelIndex()][0], ModelTransValue_array[scene.GetActiveModelIndex()][1], ModelTransValue_array[scene.GetActiveModelIndex()][2]);
		}
		// The key is right (D)
		else if (io.KeysDown[D_KEY_ASCII])
		{
		ModelTransValue_array[scene.GetActiveModelIndex()][0] += mouse_offset;
		scene.GetActiveModel().SetModelTranslate(ModelTransValue_array[scene.GetActiveModelIndex()][0], ModelTransValue_array[scene.GetActiveModelIndex()][1], ModelTransValue_array[scene.GetActiveModelIndex()][2]);
		}
		// The key is right (U)
		else if (io.KeysDown[U_KEY_ASCII])
		{
			ModelTransValue_array[scene.GetActiveModelIndex()][0] += mouse_offset;
			scene.GetActiveCamera().ApplyLocalTranslate(1, 0, 0);
		}
		// Zoom in on model
		else if (io.KeysDown[ZOOM_IN_KEY_ASCII])
		{
		if (ModelScaleValue_array[scene.GetActiveModelIndex()][0] < scaleMax)
		{
			ModelScaleValue_array[scene.GetActiveModelIndex()][0] += mouse_offset;
			ModelScaleValue_array[scene.GetActiveModelIndex()][1] += mouse_offset;
			ModelScaleValue_array[scene.GetActiveModelIndex()][2] += mouse_offset;
		}
		scene.GetActiveModel().SetModelScale(ModelScaleValue_array[scene.GetActiveModelIndex()][0], ModelScaleValue_array[scene.GetActiveModelIndex()][1], ModelScaleValue_array[scene.GetActiveModelIndex()][2]);
		}
		// Zoom out on model
		else if (io.KeysDown[ZOOM_OUT_KEY_ASCII])
		{
		if (ModelScaleValue_array[scene.GetActiveModelIndex()][0] < scaleMax)
		{
			ModelScaleValue_array[scene.GetActiveModelIndex()][0] -= mouse_offset;
			ModelScaleValue_array[scene.GetActiveModelIndex()][1] -= mouse_offset;
			ModelScaleValue_array[scene.GetActiveModelIndex()][2] -= mouse_offset;
		}
		scene.GetActiveModel().SetModelScale(ModelScaleValue_array[scene.GetActiveModelIndex()][0], ModelScaleValue_array[scene.GetActiveModelIndex()][1], ModelScaleValue_array[scene.GetActiveModelIndex()][2]);
		}
	}

	renderer.ClearColorBuffer(clear_color);
	renderer.Render(scene);
	renderer.SwapBuffers();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwMakeContextCurrent(window);
	glfwSwapBuffers(window);
}

void Cleanup(GLFWwindow* window)
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

void DrawImguiMenus(ImGuiIO& io, Scene& scene)
{
	/**
	 * MeshViewer menu
	 */
	ImGui::Begin("Color Menu");

	// Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open", "CTRL+O"))
			{
				nfdchar_t* outPath = NULL;
				nfdresult_t result = NFD_OpenDialog("obj;", NULL, &outPath);
				if (result == NFD_OKAY)
				{
					scene.AddModel(Utils::LoadMeshModel(outPath));
					free(outPath);
				}
				else if (result == NFD_CANCEL)
				{
				}
				else
				{
				}

			}
			ImGui::EndMenu();
		}

		// TODO: Add more menubar items (if you want to)
		ImGui::EndMainMenuBar();
	}

	// Controls
	if (ImGui::ColorEdit3("Background Color", (float*)&backgroundColor))
	{
		clear_color = backgroundColor;
	}

	if (ImGui::ColorEdit3("Model Color", (float*)&scene.GetActiveModel().color))
	{
		model_color = scene.GetActiveModel().color;
	}

	if (ImGui::ColorEdit3("Bounding Box Color", (float*)&scene.GetActiveModel().BoundingBoxColor))
	{
		bounding_box_color = scene.GetActiveModel().BoundingBoxColor;
	}

	if (ImGui::ColorEdit3("Faces Normals Color", (float*)&scene.GetActiveModel().FaceNormalsColor))
	{
		face_normals_color = scene.GetActiveModel().FaceNormalsColor;
	}

	if (ImGui::ColorEdit3("Vertics Normals Color", (float*)&scene.GetActiveModel().VerticsNormalsColor))
	{
		vertics_normals_color = scene.GetActiveModel().VerticsNormalsColor;
	}
	ImGui::End();

	static std::vector<std::string> modelNames;
	static int numberOfModels = scene.GetModelCount();
	static int numberOfCameras = scene.GetCameraCount();

	{
		ImGui::Begin("Model and World Transformation");

		if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
		{
			for (int i = 0; i < numberOfModels; i++)
			{
				//string str = scene.GetActiveModel().GetModelName();
				string str = "Model " + std::to_string(i);
				char* model = &str[0];

				if (ImGui::BeginTabItem(model))
				{
					scene.SetActiveModelIndex(i);

					ImGui::Checkbox("Show on screen", &scene.GetActiveModel().IsOnScreen);
					ImGui::SameLine();
					ImGui::Checkbox("Bounding Box", &scene.GetActiveModel().IsBoundingBoxOnScreen);
					ImGui::SameLine();
					ImGui::Checkbox("Model Frame", &scene.GetActiveModel().IsFrameOnScreen);

					ImGui::Checkbox("Face Normals", &scene.GetActiveModel().AreFaceNormalsOnScreen);
					ImGui::SameLine();
					ImGui::Checkbox("Vertices Normals", &scene.GetActiveModel().AreVerticesNormalsOnScreen);

					/* Set new parameters for each transformation when the slider is changed [Model] */
					if (ImGui::SliderFloat3("Model Scale", ModelScaleValue_array[i], scaleMin, scaleMax))
					{
						if (ModelScaleValue_array[i][0] == 0.0)
							ModelScaleValue_array[i][0] = 1;
						if (ModelScaleValue_array[i][1] == 0.0)
							ModelScaleValue_array[i][1] = 1;
						if (ModelScaleValue_array[i][2] == 0.0)
							ModelScaleValue_array[i][2] = 1;

						scene.GetActiveModel().SetModelScale(ModelScaleValue_array[i][0], ModelScaleValue_array[i][1], ModelScaleValue_array[i][2]);
					}
					if (ImGui::SliderFloat3("Model Translate", ModelTransValue_array[i], translateMin, translateMax))
					{
						scene.GetActiveModel().SetModelTranslate(ModelTransValue_array[i][0], ModelTransValue_array[i][1], ModelTransValue_array[i][2]);
					}
					if (ImGui::SliderFloat3("Model Rotate", ModelRotateValue_array[i], rotateMin, rotateMax))
					{
						scene.GetActiveModel().SetModelRotate(ModelRotateValue_array[i][0], ModelRotateValue_array[i][1], ModelRotateValue_array[i][2]);
					}

					ImGui::Dummy(ImVec2(0.0f, 20.0f));

					/* Set new parameters for each transformation when the slider is changed [World] */
					if (ImGui::SliderFloat3("World Scale", WorldScaleValue_array[i], scaleMinWorld, scaleMaxWorld))
					{
						if (WorldScaleValue_array[i][0] == 0.0)
							WorldScaleValue_array[i][0] = 1;
						if (WorldScaleValue_array[i][1] == 0.0)
							WorldScaleValue_array[i][1] = 1;
						if (WorldScaleValue_array[i][2] == 0.0)
							WorldScaleValue_array[i][2] = 1;

						scene.GetActiveModel().SetWorldScale(WorldScaleValue_array[i][0], WorldScaleValue_array[i][1], WorldScaleValue_array[i][2]);
					}
					if (ImGui::SliderFloat3("World Translate", WorldTransValue_array[i], translateMin, translateMax))
					{
						scene.GetActiveModel().SetWorldTranslate(WorldTransValue_array[i][0], WorldTransValue_array[i][1], WorldTransValue_array[i][2]);
					}
					if (ImGui::SliderFloat3("World Rotate", WorldRotateValue_array[i], rotateMin, rotateMax))
					{
						scene.GetActiveModel().SetWorldRotate(WorldRotateValue_array[i][0], WorldRotateValue_array[i][1], WorldRotateValue_array[i][2]);
					}

					ImGui::EndTabItem();
				}
			}
			ImGui::EndTabItem();
		}

		ImGui::End();
	
		ImGui::Begin("Camera Control");

		if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
		{
			for (int i = 0; i < numberOfCameras; i++)
			{
				string str = "Camera " + std::to_string(i);
				char* camera = &str[0];

				if (ImGui::BeginTabItem(camera))
				{
					scene.SetActiveCameraIndex(i);

					// Radio buttons for camera projection modes
					if (ImGui::RadioButton("Orthographic", &cameraMode, ORTHO))
					{
						scene.GetActiveCamera().SetOrthoCamera();
					}
					ImGui::SameLine();
					if (ImGui::RadioButton("Perspective", &cameraMode, PERSPECTIVE))
					{
						scene.GetActiveCamera().SetPerspectiveCamera();
					}

					if (ImGui::SliderFloat3("Camera Translate", CameraTransValue_array[i], translateMin, translateMax))
					{
						scene.GetActiveCamera().SetLocalTranslate(CameraTransValue_array[i][0], CameraTransValue_array[i][1], CameraTransValue_array[i][2]);
					}
					if (ImGui::SliderFloat3("Camera Rotate", CameraRotateValue_array[i], rotateMin, rotateMax))
					{
						scene.GetActiveCamera().SetLocalRotate(CameraRotateValue_array[i][0], CameraRotateValue_array[i][1], CameraRotateValue_array[i][2]);
					}

					//ImGui::Dummy(ImVec2(0.0f, 20.0f));

					/* Set new parameters for each transformation when the slider is changed [World] */
					if (ImGui::SliderFloat3("World Translate", CameraWorldTransValue_array[i], translateMin, translateMax))
					{
						scene.GetActiveCamera().SetWorldTranslate(CameraWorldTransValue_array[i][0], CameraWorldTransValue_array[i][1], CameraWorldTransValue_array[i][2]);
					}
					if (ImGui::SliderFloat3("World Rotate", CameraWorldRotateValue_array[i], rotateMin, rotateMax))
					{
						scene.GetActiveCamera().SetWorldRotate(CameraWorldRotateValue_array[i][0], CameraWorldRotateValue_array[i][1], CameraWorldRotateValue_array[i][2]);
					}
					ImGui::Dummy(ImVec2(0.0f, 20.0f));

					if (cameraMode == ORTHO)
					{
						/* Sliders to change view volume of projection */
						ImGui::SliderFloat("Camera X", &ortho_array[i][0], cameraMin, cameraMax);
						ImGui::SliderFloat("Camera Y", &ortho_array[i][1], cameraMin, cameraMax);
						ImGui::SliderFloat("Distance", &ortho_array[i][2], cameraMin, cameraMax);
						
						scene.GetActiveCamera().SetDepth(ortho_array[i][2] / 2, -ortho_array[i][2] / 2);
						scene.GetActiveCamera().SetOrthoViewVolume(ortho_array[i][0] / 2, -ortho_array[i][0] / 2,
							-ortho_array[i][1] / 2, ortho_array[i][1] / 2);
					}
					else if (cameraMode == PERSPECTIVE)
					{
						/* Sliders to change view volume of projection */
						ImGui::SliderFloat("Camera X", &perspective_array[i][0], cameraMin, cameraMax);
						ImGui::SliderFloat("Camera Y", &perspective_array[i][1], cameraMin, cameraMax);
						ImGui::SliderFloat("Distance", &perspective_array[i][2], cameraMin, cameraMax);

						scene.GetActiveCamera().SetDepth(perspective_array[i][2] / 2, -perspective_array[i][2] / 2);
						scene.GetActiveCamera().SetPerspectiveViewVolume(perspective_array[i][0] / 2, -perspective_array[i][0] / 2,
							-perspective_array[i][1] / 2, perspective_array[i][1] / 2);
					}

					ImGui::Dummy(ImVec2(0.0f, 20.0f));

					glm::vec3 eye(eye_array[i][0], eye_array[i][1], eye_array[i][2]);
					glm::vec3 at(at_array[i][0], at_array[i][1], at_array[i][2]);
					glm::vec3 up(up_array[i][0], up_array[i][1], up_array[i][2]);

					if (ImGui::SliderFloat3("Eye", eye_array[i], LookAtMin, LookAtMax))
					{
						eye = glm::vec3(eye_array[i][0], eye_array[i][1], eye_array[i][2]);
					}
					if (ImGui::SliderFloat3("At", at_array[i], LookAtMin, LookAtMax))
					{
						at = glm::vec3(at_array[i][0], at_array[i][1], at_array[i][2]);
					}
					if (ImGui::SliderFloat3("Up", up_array[i], LookAtMin, LookAtMax))
					{
						up = glm::vec3(up_array[i][0], up_array[i][1], up_array[i][2]);
					}
					scene.GetActiveCamera().SetCameraLookAt(eye, at, up);

					ImGui::EndTabItem();
				}
			}
			ImGui::EndTabItem();
		}
		ImGui::End();
	}
}