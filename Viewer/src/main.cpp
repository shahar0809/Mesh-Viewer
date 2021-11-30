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
int mouse_offset = 5;

float scaleMax = 50.0f, scaleMin = -50.0f;
float scaleMaxWorld = 5.0f, scaleMinWorld = -5.0;
float translateMax = 50.0f, translateMin = -50.0f;
float rotateMax = 360.0f, rotateMin = 0;
float cameraMax = 30.0f, cameraMin = -30.0f;

static int SCREEN_ASPECT = 80;
static int width = 0, height = 0;

// ASCII values for keyboard events
static const int S_KEY_ASCII = int('S'),
				 W_KEY_ASCII = int('W'),
				 A_KEY_ASCII = int('A'),
				 D_KEY_ASCII = int('D');

static const int ZOOM_IN_KEY_ASCII = int('['),
				 ZOOM_OUT_KEY_ASCII = int(']');

static float ModelScaleValue_array[5][3] = { {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1} };
static float ModelTransValue_array[5][3] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0} };
static float ModelRotateValue_array[5][3] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0} };

static float WorldScaleValue_array[5][3] = { {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1} };
static float WorldTransValue_array[5][3] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0} };
static float WorldRotateValue_array[5][3] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0} };

static float CameraController[6] = { 0 };

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
	// TODO: Need to use relative path
	//const std::string base_path = "C:\\Users\\karin\\Documents\\GitHub\\computer-graphics-2022-shahar-and-iris\\Data\\";
	const std::string base_path = "C:\\Users\\משתמש\\Documents\\University\\Computerized Graphics\\computer-graphics-2022-shahar-and-iris\\Data\\";
	int windowWidth = 1280, windowHeight = 720;
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
	CameraController[0] = -width / SCREEN_ASPECT;
	CameraController[1] = width / SCREEN_ASPECT;
	CameraController[2] = -height / SCREEN_ASPECT;
	CameraController[3] = height / SCREEN_ASPECT;
	CameraController[4] = width / SCREEN_ASPECT;
	CameraController[5] = -width / SCREEN_ASPECT;
	
	/* Load a few models */
	std::shared_ptr<MeshModel> model1 = Utils::LoadMeshModel(base_path + "demo.obj");
	scene.AddModel(model1);
	//renderer.fitInScreen(scene.GetModel(0));

	//std::shared_ptr<MeshModel> model2 = Utils::LoadMeshModel(base_path + "bunny.obj");
	//scene.AddModel(model2);
	////renderer.fitInScreen(scene.GetModel(1));

	//std::shared_ptr<MeshModel> model3 = Utils::LoadMeshModel(base_path + "pawn.obj");
	//scene.AddModel(model3);
	////renderer.fitInScreen(scene.GetModel(2));

	/* Load a camera */
	std::shared_ptr<Camera> camera1 = std::make_shared<Camera>();;
	scene.AddCamera(camera1);
	
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

void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io)
{
	ImGui::Render();
	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

	if (frameBufferWidth != renderer.GetViewportWidth() || frameBufferHeight != renderer.GetViewportHeight())
	{
		// TODO: Set new aspect ratio
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
	ImGui::Begin("MeshViewer Menu");

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
	ImGui::ColorEdit3("Clear Color", (float*)&clear_color);
	ImGui::End();

	static std::vector<std::string> modelNames;
	static int numberOfModels = scene.GetModelCount();

	{
		ImGui::Begin("Model and World Transformation");

		if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
		{
			for (int i = 0; i < numberOfModels; i++)
			{
				string str = "Model " + std::to_string(i);
				char* model = &str[0];

				if (ImGui::BeginTabItem(model))
				{
					scene.SetActiveModelIndex(i);

					if (ImGui::Checkbox("Show on screen", &scene.GetActiveModel().IsOnScreen))
					{
						if (scene.GetActiveModel().IsOnScreen)
						{
							scene.GetActiveModel().color = model_color;

							if (scene.GetActiveModel().IsBoundingBoxOnScreen)
							{
								scene.GetActiveModel().BoundingBoxColor = bounding_box_color;
							}
						}
						else
						{
							scene.GetActiveModel().color = clear_color;
							scene.GetActiveModel().BoundingBoxColor = clear_color;
						}
					}

					if (ImGui::Checkbox("Bounding Box", &scene.GetActiveModel().IsBoundingBoxOnScreen))
					{
						if (scene.GetActiveModel().IsBoundingBoxOnScreen)
						{
							scene.GetActiveModel().BoundingBoxColor = bounding_box_color;
						}
						else
						{
							scene.GetActiveModel().BoundingBoxColor = clear_color;
						}
					}

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
		}

		ImGui::End();

		ImGui::Begin("Camera Control");

		/* change view volume */
		ImGui::SliderFloat("Left", &CameraController[0], cameraMin, cameraMax);
		ImGui::SliderFloat("Right", &CameraController[1], cameraMin, cameraMax);
		ImGui::SliderFloat("Bottom", &CameraController[2], cameraMin, cameraMax);
		ImGui::SliderFloat("Top", &CameraController[3], cameraMin, cameraMax);
		ImGui::SliderFloat("Near", &CameraController[4], cameraMin, cameraMax);
		ImGui::SliderFloat("Far", &CameraController[5], cameraMin, cameraMax);

		scene.GetActiveCamera().SetOrthoTrans(CameraController[0] / 2, CameraController[1] / 2,
			CameraController[2] / 2, CameraController[3] / 2, CameraController[4] / 2, CameraController[5] / 2);

		ImGui::End();
		

		//if (ImGui::SliderFloat("Left", CameraControler[i], cameraMin, cameraMax))
		//{
		//	
		//}
		//if (ImGui::SliderFloat("Right", CameraControler[i], cameraMin, cameraMax))
		//{

		//}
		//if (ImGui::SliderFloat("Bottom", CameraControler[i], cameraMin, cameraMax))
		//{
	
		//}
	}
}