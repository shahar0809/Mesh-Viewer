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

int windowWidth = 1280, windowHeight = 720;
const std::string base_path = "C: ../../../Data/";


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
	GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, "Mesh Viewer");
	if (!window)
		return 1;

	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

	Renderer renderer = Renderer(frameBufferWidth, frameBufferHeight);
	Scene scene = Scene();

	CameraGUI_Attributes::width = renderer.GetViewportWidth(), CameraGUI_Attributes::height = renderer.GetViewportHeight();
	
	/* Load a few models */
	std::shared_ptr<MeshModel> model1 = Utils::LoadMeshModel(base_path + "demo.obj");
	scene.AddModel(model1);

	/* Load a camera */
	std::shared_ptr<Camera> camera1 = std::make_shared<Camera>();
	scene.AddCamera(camera1);

	/* Add a light */
	std::shared_ptr<Light> light1 = std::make_shared<Light>();
	scene.AddLight(light1);
	
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

	/* -------------------------------------------------- Keyboard and Mouse Interactions -----------------------------------------------*/
	if (!io.WantCaptureKeyboard)
	{
		// The key is down (S)
		if (io.KeysDown[S_KEY_ASCII])
		{
			scene.GetActiveModel().gui.ModelTransValue_array[1] -= mouse_offset;
			scene.GetActiveModel().SetModelTranslate(scene.GetActiveModel().gui.ModelTransValue_array[0], 
				scene.GetActiveModel().gui.ModelTransValue_array[1], scene.GetActiveModel().gui.ModelTransValue_array[2]);
		}
		// The key is up (W)
		else if (io.KeysDown[W_KEY_ASCII])
		{
			scene.GetActiveModel().gui.ModelTransValue_array[1] += mouse_offset;
			scene.GetActiveModel().SetModelTranslate(scene.GetActiveModel().gui.ModelTransValue_array[0], 
				scene.GetActiveModel().gui.ModelTransValue_array[1], scene.GetActiveModel().gui.ModelTransValue_array[2]);
		}
		// The key is left (A)
		else if (io.KeysDown[A_KEY_ASCII])
		{
			scene.GetActiveModel().gui.ModelTransValue_array[0] -= mouse_offset;
			scene.GetActiveModel().SetModelTranslate(scene.GetActiveModel().gui.ModelTransValue_array[0], 
				scene.GetActiveModel().gui.ModelTransValue_array[1], scene.GetActiveModel().gui.ModelTransValue_array[2]);
		}
		// The key is right (D)
		else if (io.KeysDown[D_KEY_ASCII])
		{
			scene.GetActiveModel().gui.ModelTransValue_array[0] += mouse_offset;
			scene.GetActiveModel().SetModelTranslate(scene.GetActiveModel().gui.ModelTransValue_array[0], 
				scene.GetActiveModel().gui.ModelTransValue_array[1], scene.GetActiveModel().gui.ModelTransValue_array[2]);
		}
		// The key is right (U)
		else if (io.KeysDown[U_KEY_ASCII])
		{
			scene.GetActiveModel().gui.ModelTransValue_array[0] += mouse_offset;
			scene.GetActiveCamera().ApplyLocalTranslate(1, 0, 0);
		}
		// Zoom in on model
		else if (io.KeysDown[ZOOM_IN_KEY_ASCII])
		{
		if (scene.GetActiveModel().gui.ModelScaleValue_array[0] < scaleMax)
		{
			scene.GetActiveModel().gui.ModelScaleValue_array[0] += mouse_offset;
			scene.GetActiveModel().gui.ModelScaleValue_array[1] += mouse_offset;
			scene.GetActiveModel().gui.ModelScaleValue_array[2] += mouse_offset;
		}
		scene.GetActiveModel().SetModelScale(scene.GetActiveModel().gui.ModelScaleValue_array[0], 
			scene.GetActiveModel().gui.ModelScaleValue_array[1],
			scene.GetActiveModel().gui.ModelScaleValue_array[2]);
		}
		// Zoom out on model
		else if (io.KeysDown[ZOOM_OUT_KEY_ASCII])
		{
		if (scene.GetActiveModel().gui.ModelScaleValue_array[0] < scaleMax)
		{
			scene.GetActiveModel().gui.ModelScaleValue_array[0] -= mouse_offset;
			scene.GetActiveModel().gui.ModelScaleValue_array[1] -= mouse_offset;
			scene.GetActiveModel().gui.ModelScaleValue_array[2] -= mouse_offset;
		}
		scene.GetActiveModel().SetModelScale(scene.GetActiveModel().gui.ModelScaleValue_array[0], 
			scene.GetActiveModel().gui.ModelScaleValue_array[1], scene.GetActiveModel().gui.ModelScaleValue_array[2]);
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
		if (ImGui::BeginMenu("Add"))
		{
			if (ImGui::MenuItem("Camera", "CTRL+SHIFT+C"))
			{
				scene.AddCamera(std::make_shared<Camera>());
			}
			if (ImGui::BeginMenu("Model"))
			{
				if (ImGui::MenuItem("Demo"))
				{
					std::shared_ptr<MeshModel> model = Utils::LoadMeshModel(base_path + "demo.obj");
					scene.AddModel(model);
				}
				
				if (ImGui::MenuItem("Cow"))
				{
					std::shared_ptr<MeshModel> model = Utils::LoadMeshModel(base_path + "cow.obj");
					scene.AddModel(model);
				}
				if (ImGui::MenuItem("Banana"))
				{
					std::shared_ptr<MeshModel> model = Utils::LoadMeshModel(base_path + "banana.obj");
					scene.AddModel(model);
				}
				if (ImGui::MenuItem("Sphere"))
				{
					std::shared_ptr<MeshModel> model = Utils::LoadMeshModel(base_path + "sphere.obj");
					scene.AddModel(model);
				}
				if (ImGui::MenuItem("Pawn"))
				{
					std::shared_ptr<MeshModel> model = Utils::LoadMeshModel(base_path + "pawn.obj");
					scene.AddModel(model);
				}
				if (ImGui::MenuItem("Teapot"))
				{
					std::shared_ptr<MeshModel> model = Utils::LoadMeshModel(base_path + "teapot.obj");
					scene.AddModel(model);
				}
				if (ImGui::MenuItem("Bunny"))
				{
					std::shared_ptr<MeshModel> model = Utils::LoadMeshModel(base_path + "bunny.obj");
					scene.AddModel(model);
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Light", "CTRL+SHIFT+L"))
			{
				scene.AddLight(std::make_shared<Light>());
			}
			ImGui::EndMenu();
		}

		// TODO: Add more menubar items (if you want to)
		ImGui::EndMainMenuBar();
	}

	/* -------------------------------------------------- Colors control -----------------------------------------------*/
	if (ImGui::ColorEdit3("Background", (float*)&backgroundColor))
	{
		clear_color = backgroundColor;
	}

	if (ImGui::ColorEdit3("Model", (float*)&scene.GetActiveModel().gui.color))
	{
		model_color = scene.GetActiveModel().gui.color;
	}

	if (ImGui::ColorEdit3("Bounding Box", (float*)&scene.GetActiveModel().gui.BoundingBoxColor))
	{
		bounding_box_color = scene.GetActiveModel().gui.BoundingBoxColor;
	}

	if (ImGui::ColorEdit3("Faces Normals", (float*)&scene.GetActiveModel().gui.FaceNormalsColor))
	{
		face_normals_color = scene.GetActiveModel().gui.FaceNormalsColor;
	}

	if (ImGui::ColorEdit3("Vertics Normals", (float*)&scene.GetActiveModel().gui.VerticsNormalsColor))
	{
		vertics_normals_color = scene.GetActiveModel().gui.VerticsNormalsColor;
	}
	ImGui::ColorEdit3("Reflection Vectors", (float*)&scene.GetActiveModel().gui.ReflectionVectorsColor);
	ImGui::End();

	static std::vector<std::string> modelNames;
	int numberOfModels = scene.GetModelCount();
	int numberOfCameras = scene.GetCameraCount();

	/* -------------------------------------------------- Control models -----------------------------------------------*/
	{
		ImGui::Begin("Models");

		if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
		{
			for (int i = 0; i < numberOfModels; i++)
			{
				string str = "Model " + std::to_string(i);
				char* model = &str[0];

				if (ImGui::BeginTabItem(model))
				{
					scene.SetActiveModelIndex(i);

					ImGui::Checkbox("Visible", &scene.GetActiveModel().gui.IsOnScreen);
					ImGui::SameLine();
					ImGui::Checkbox("BBox", &scene.GetActiveModel().gui.IsBoundingBoxOnScreen);
					ImGui::SameLine();
					ImGui::Checkbox("Reflection Vecs", &scene.GetActiveModel().gui.ShowReflectionVectors);

					ImGui::Checkbox("Model Frame", &scene.GetActiveModel().gui.IsFrameOnScreen);
					ImGui::SameLine();
					ImGui::Checkbox("Face Normals", &scene.GetActiveModel().gui.AreFaceNormalsOnScreen);
					ImGui::SameLine();
					ImGui::Checkbox("Vertices Normals", &scene.GetActiveModel().gui.AreVerticesNormalsOnScreen);

					ImGui::ColorEdit3("Ambient", (float*)&scene.GetActiveModel().gui.AmbientReflectionColor);
					ImGui::ColorEdit3("Specular", (float*)&scene.GetActiveModel().gui.SpecularReflectionColor);
					ImGui::ColorEdit3("Diffuse", (float*)&scene.GetActiveModel().gui.DiffuseReflectionColor);
					ImGui::SliderFloat("Shininess", &scene.GetActiveModel().gui.shininess, 1, 20);

					ImGui::Dummy(ImVec2(0.0f, 20.0f));

					/* Set new parameters for each transformation when the slider is changed [Model] */
					if (ImGui::SliderFloat3("Model Scale", scene.GetActiveModel().gui.ModelScaleValue_array, scaleMin, scaleMax))
					{
						if (scene.GetActiveModel().gui.ModelScaleValue_array[0] == 0.0)
							scene.GetActiveModel().gui.ModelScaleValue_array[0] = 1;
						if (scene.GetActiveModel().gui.ModelScaleValue_array[1] == 0.0)
							scene.GetActiveModel().gui.ModelScaleValue_array[1] = 1;
						if (scene.GetActiveModel().gui.ModelScaleValue_array[2] == 0.0)
							scene.GetActiveModel().gui.ModelScaleValue_array[2] = 1;

						scene.GetActiveModel().SetModelScale(scene.GetActiveModel().gui.ModelScaleValue_array[0], 
							scene.GetActiveModel().gui.ModelScaleValue_array[1],
							scene.GetActiveModel().gui.ModelScaleValue_array[2]);
					}
					if (ImGui::SliderFloat3("Model Translate", scene.GetActiveModel().gui.ModelTransValue_array, translateMin, translateMax))
					{
						scene.GetActiveModel().SetModelTranslate(scene.GetActiveModel().gui.ModelTransValue_array[0], 
							scene.GetActiveModel().gui.ModelTransValue_array[1],
							scene.GetActiveModel().gui.ModelTransValue_array[2]);
					}
					if (ImGui::SliderFloat3("Model Rotate", scene.GetActiveModel().gui.ModelRotateValue_array, rotateMin, rotateMax))
					{
						scene.GetActiveModel().SetModelRotate(scene.GetActiveModel().gui.ModelRotateValue_array[0], 
							scene.GetActiveModel().gui.ModelRotateValue_array[1],
							scene.GetActiveModel().gui.ModelRotateValue_array[2]);
					}

					ImGui::Dummy(ImVec2(0.0f, 20.0f));

					/* Set new parameters for each transformation when the slider is changed [World] */
					if (ImGui::SliderFloat3("World Scale", scene.GetActiveModel().gui.WorldScaleValue_array, scaleMinWorld, scaleMaxWorld))
					{
						if (scene.GetActiveModel().gui.WorldScaleValue_array[0] == 0.0)
							scene.GetActiveModel().gui.WorldScaleValue_array[0] = 1;
						if (scene.GetActiveModel().gui.WorldScaleValue_array[1] == 0.0)
							scene.GetActiveModel().gui.WorldScaleValue_array[1] = 1;
						if (scene.GetActiveModel().gui.WorldScaleValue_array[2] == 0.0)
							scene.GetActiveModel().gui.WorldScaleValue_array[2] = 1;

						scene.GetActiveModel().SetWorldScale(scene.GetActiveModel().gui.WorldScaleValue_array[0], scene.GetActiveModel().gui.WorldScaleValue_array[1], scene.GetActiveModel().gui.WorldScaleValue_array[2]);
					}
					if (ImGui::SliderFloat3("World Translate", scene.GetActiveModel().gui.WorldTransValue_array, translateMin, translateMax))
					{
						scene.GetActiveModel().SetWorldTranslate(scene.GetActiveModel().gui.WorldTransValue_array[0], scene.GetActiveModel().gui.WorldTransValue_array[1], scene.GetActiveModel().gui.WorldTransValue_array[2]);
					}
					if (ImGui::SliderFloat3("World Rotate", scene.GetActiveModel().gui.WorldRotateValue_array, rotateMin, rotateMax))
					{
						scene.GetActiveModel().SetWorldRotate(scene.GetActiveModel().gui.WorldRotateValue_array[0], scene.GetActiveModel().gui.WorldRotateValue_array[1], scene.GetActiveModel().gui.WorldRotateValue_array[2]);
					}

					ImGui::EndTabItem();
				}
			}
			ImGui::EndTabBar();
		}

		ImGui::End();
	
		/* -------------------------------------------------- Cameras Control -----------------------------------------------*/
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
					if (ImGui::RadioButton("Orthographic", &scene.GetActiveCamera().gui.cameraMode, ORTHO))
					{
						scene.GetActiveCamera().SetOrthoCamera();
					}
					ImGui::SameLine();
					if (ImGui::RadioButton("Perspective", &scene.GetActiveCamera().gui.cameraMode, PERSPECTIVE))
					{
						scene.GetActiveCamera().SetPerspectiveCamera();
					}

					if (ImGui::SliderFloat3("Camera Translate", scene.GetActiveCamera().gui.CameraTransValue_array, translateMin, translateMax))
					{
						scene.GetActiveCamera().SetLocalTranslate(scene.GetActiveCamera().gui.CameraTransValue_array[0], 
							scene.GetActiveCamera().gui.CameraTransValue_array[1],
							scene.GetActiveCamera().gui.CameraTransValue_array[2]);
					}
					if (ImGui::SliderFloat3("Camera Rotate", scene.GetActiveCamera().gui.CameraRotateValue_array, rotateMin, rotateMax))
					{
						scene.GetActiveCamera().SetLocalRotate(scene.GetActiveCamera().gui.CameraRotateValue_array[0], 
							scene.GetActiveCamera().gui.CameraRotateValue_array[1],
							scene.GetActiveCamera().gui.CameraRotateValue_array[2]);
					}

					ImGui::Dummy(ImVec2(0.0f, 20.0f));

					/* Set new parameters for each transformation when the slider is changed [World] */
					if (ImGui::SliderFloat3("World Translate", scene.GetActiveCamera().gui.CameraWorldTransValue_array, translateMin, translateMax))
					{
						scene.GetActiveCamera().SetWorldTranslate(scene.GetActiveCamera().gui.CameraWorldTransValue_array[0],
							scene.GetActiveCamera().gui.CameraWorldTransValue_array[1],
							scene.GetActiveCamera().gui.CameraWorldTransValue_array[2]);
					}
					if (ImGui::SliderFloat3("World Rotate", scene.GetActiveCamera().gui.CameraWorldRotateValue_array, rotateMin, rotateMax))
					{
						scene.GetActiveCamera().SetWorldRotate(scene.GetActiveCamera().gui.CameraWorldRotateValue_array[0],
							scene.GetActiveCamera().gui.CameraWorldRotateValue_array[1],
							scene.GetActiveCamera().gui.CameraWorldRotateValue_array[2]);
					}
					ImGui::Dummy(ImVec2(0.0f, 20.0f));

					if (scene.GetActiveCamera().gui.cameraMode == ORTHO)
					{
						/* Sliders to change view volume of projection */
						ImGui::SliderFloat("Camera X", &scene.GetActiveCamera().gui.ortho_array[0], cameraMin, cameraMax);
						ImGui::SliderFloat("Camera Y", &scene.GetActiveCamera().gui.ortho_array[1], cameraMin, cameraMax);
						ImGui::SliderFloat("Distance", &scene.GetActiveCamera().gui.ortho_array[2], cameraMin, cameraMax);
						
						scene.GetActiveCamera().SetDepth(scene.GetActiveCamera().gui.ortho_array[2] / 2, -scene.GetActiveCamera().gui.ortho_array[2] / 2);
						scene.GetActiveCamera().SetOrthoViewVolume(scene.GetActiveCamera().gui.ortho_array[0] / 2,
							-scene.GetActiveCamera().gui.ortho_array[0] / 2,
							-scene.GetActiveCamera().gui.ortho_array[1] / 2,
							scene.GetActiveCamera().gui.ortho_array[1] / 2);
					}
					else if (scene.GetActiveCamera().gui.cameraMode == PERSPECTIVE)
					{
						/* Sliders to change view volume of projection */
						ImGui::SliderFloat("Camera X", &scene.GetActiveCamera().gui.perspective_array[0], cameraMin, cameraMax);
						ImGui::SliderFloat("Camera Y", &scene.GetActiveCamera().gui.perspective_array[1], cameraMin, cameraMax);
						ImGui::SliderFloat("Distance", &scene.GetActiveCamera().gui.perspective_array[2], cameraMin, cameraMax);

						scene.GetActiveCamera().SetDepth(scene.GetActiveCamera().gui.perspective_array[2] / 2,
							-scene.GetActiveCamera().gui.perspective_array[2] / 2);
						scene.GetActiveCamera().SetPerspectiveViewVolume(scene.GetActiveCamera().gui.perspective_array[0] / 2,
							-scene.GetActiveCamera().gui.perspective_array[0] / 2,
							-scene.GetActiveCamera().gui.perspective_array[1] / 2,
							scene.GetActiveCamera().gui.perspective_array[1] / 2);
					}

					ImGui::Dummy(ImVec2(0.0f, 20.0f));

					glm::vec3 eye(scene.GetActiveCamera().gui.eye_array[0], scene.GetActiveCamera().gui.eye_array[1], scene.GetActiveCamera().gui.eye_array[2]);
					glm::vec3 at(scene.GetActiveCamera().gui.at_array[0], scene.GetActiveCamera().gui.at_array[1], scene.GetActiveCamera().gui.at_array[2]);
					glm::vec3 up(scene.GetActiveCamera().gui.up_array[0], scene.GetActiveCamera().gui.up_array[1], scene.GetActiveCamera().gui.up_array[2]);

					if (ImGui::SliderFloat3("Eye", scene.GetActiveCamera().gui.eye_array, LookAtMin, LookAtMax))
					{
						eye = glm::vec3(scene.GetActiveCamera().gui.eye_array[0], scene.GetActiveCamera().gui.eye_array[1], scene.GetActiveCamera().gui.eye_array[2]);
					}
					if (ImGui::SliderFloat3("At", scene.GetActiveCamera().gui.at_array, LookAtMin, LookAtMax))
					{
						at = glm::vec3(scene.GetActiveCamera().gui.at_array[0], scene.GetActiveCamera().gui.at_array[1], scene.GetActiveCamera().gui.at_array[2]);
					}
					if (ImGui::SliderFloat3("Up", scene.GetActiveCamera().gui.up_array, LookAtMin, LookAtMax))
					{
						up = glm::vec3(scene.GetActiveCamera().gui.up_array[0], scene.GetActiveCamera().gui.up_array[1], scene.GetActiveCamera().gui.up_array[2]);
					}
					scene.GetActiveCamera().SetCameraLookAt(eye, at, up);

					ImGui::EndTabItem();
				}
			}
			ImGui::EndTabBar();
		}
		ImGui::End();

		/* -------------------------------------------------- Lights Control -----------------------------------------------*/
		ImGui::Begin("Lights Control");

		if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
		{
			for (int i = 0; i < scene.GetLightCount(); i++)
			{
				string str = "Light " + std::to_string(i);
				char* light = &str[0];

				if (ImGui::BeginTabItem(light))
				{
					scene.SetActiveLightIndex(i);

					float* colorPointer = nullptr;

					if (ImGui::RadioButton("Point", (int*)(&scene.GetActiveLight().gui.lightType), (int)LightType::LIGHT_POINT))
					{
						scene.GetActiveLight().SetPoint();
					}
					ImGui::SameLine();
					if (ImGui::RadioButton("Directional", (int*)(&scene.GetActiveLight().gui.lightType), (int)LightType::DIRECTIONAL_LIGHT))
					{
						scene.GetActiveLight().SetDirectional();
					}

					// Radio buttons for shading type modes
					if (ImGui::RadioButton("Flat", (int*)(&scene.GetActiveLight().gui.shadingType), (int)ShadingType::FLAT))
					{
						scene.GetActiveLight().SetFlat();
					}
					ImGui::SameLine();
					if (ImGui::RadioButton("Gouraud", (int*)(&scene.GetActiveLight().gui.shadingType), (int)ShadingType::GOURAUD))
					{
						scene.GetActiveLight().SetGouraud();
					}
					ImGui::SameLine();
					if (ImGui::RadioButton("Phong", (int*)(&scene.GetActiveLight().gui.shadingType), (int)ShadingType::PHONG))
					{
						scene.GetActiveLight().SetPhong();
					}

					ImGui::ColorEdit3("Ambient", (float*)&scene.GetActiveLight().gui.AmbientSourceColor);
					ImGui::ColorEdit3("Specular", (float*)&scene.GetActiveLight().gui.SpecularSourceColor);
					ImGui::ColorEdit3("Diffuse", (float*)&scene.GetActiveLight().gui.DiffuseSourceColor);

					scene.GetActiveLight().SetAmbientColor(glm::vec3(scene.GetActiveLight().gui.AmbientSourceColor[0],
						scene.GetActiveLight().gui.AmbientSourceColor[1],
						scene.GetActiveLight().gui.AmbientSourceColor[2]));
					scene.GetActiveLight().SetSpecularColor(glm::vec3(scene.GetActiveLight().gui.SpecularSourceColor[0],
						scene.GetActiveLight().gui.SpecularSourceColor[1],
						scene.GetActiveLight().gui.SpecularSourceColor[2]));
					scene.GetActiveLight().SetDiffuseColor(glm::vec3(scene.GetActiveLight().gui.DiffuseSourceColor[0],
						scene.GetActiveLight().gui.DiffuseSourceColor[1],
						scene.GetActiveLight().gui.DiffuseSourceColor[2]));
			
					ImGui::Dummy(ImVec2(0.0f, 20.0f));

					if (ImGui::SliderFloat3("Local Translate", scene.GetActiveLight().gui.LightLocalTransValue_array, translateMin, translateMax))
					{
						scene.GetActiveLight().SetLocalTranslate(scene.GetActiveLight().gui.LightLocalTransValue_array[0],
							scene.GetActiveLight().gui.LightLocalTransValue_array[1],
							scene.GetActiveLight().gui.LightLocalTransValue_array[2]);
					}
					if (ImGui::SliderFloat3("Local Rotate", scene.GetActiveLight().gui.LightLocalRotateValue_array, rotateMin, rotateMax))
					{
						scene.GetActiveLight().SetLocalRotate(scene.GetActiveLight().gui.LightLocalRotateValue_array[0],
							scene.GetActiveLight().gui.LightLocalRotateValue_array[1],
							scene.GetActiveLight().gui.LightLocalRotateValue_array[2]);
					}

					/* Set new parameters for each transformation when the slider is changed [World] */
					if (ImGui::SliderFloat3("World Translate", scene.GetActiveLight().gui.LightWorldTransValue_array, translateMin, translateMax))
					{
						scene.GetActiveLight().SetWorldTranslate(scene.GetActiveLight().gui.LightWorldTransValue_array[0],
							scene.GetActiveLight().gui.LightWorldTransValue_array[1],
							scene.GetActiveLight().gui.LightWorldTransValue_array[2]);
					}
					if (ImGui::SliderFloat3("World Rotate", scene.GetActiveLight().gui.LightWorldRotateValue_array, rotateMin, rotateMax))
					{
						scene.GetActiveLight().SetWorldRotate(scene.GetActiveLight().gui.LightWorldRotateValue_array[0],
							scene.GetActiveLight().gui.LightWorldRotateValue_array[1],
							scene.GetActiveLight().gui.LightWorldRotateValue_array[2]);
					}
					ImGui::EndTabItem();
				}
			}
			ImGui::EndTabBar();
		}
		ImGui::End();



		/* -------------------------------------------------- Post Processing -----------------------------------------------*/

		ImGui::Begin("Post Processing");
		ImGui::Checkbox("Fog", &scene.isFog);
		ImGui::SameLine();
		/*ImGui::Checkbox("BBox", &scene.GetActiveModel().gui.IsBoundingBoxOnScreen);
		ImGui::SameLine();*/
		ImGui::End();

	}
}