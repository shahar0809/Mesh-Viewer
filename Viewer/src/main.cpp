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
glm::vec4 clear_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);

static float ModelScaleValue_array[5][3] = { {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1} };
static float ModelTransValue_array[5][3] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0} };
static float ModelRotateValue_array[5][3] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0} };

static float WorldScaleValue_array[5][3] = { {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1} };
static float WorldTransValue_array[5][3] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0} };
static float WorldRotateValue_array[5][3] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0} };

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
	const std::string path = "C:/Users/karin/Documents/GitHub/computer-graphics-2022-shahar-and-iris/Data/banana.obj";
	int windowWidth = 1280, windowHeight = 720;
	GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, "Mesh Viewer");
	if (!window)
		return 1;

	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

	Renderer renderer = Renderer(frameBufferWidth, frameBufferHeight);
	Scene scene = Scene();

	std::shared_ptr<MeshModel> model1 = Utils::LoadMeshModel(path);
	scene.AddModel(model1);
	renderer.fitInScreen(scene.GetModel(0));

	std::shared_ptr<MeshModel> model2 = Utils::LoadMeshModel("C:/Users/karin/Documents/GitHub/computer-graphics-2022-shahar-and-iris/Data/bunny.obj");
	scene.AddModel(model2);
	renderer.fitInScreen(scene.GetModel(1));

	std::shared_ptr<MeshModel> model3 = Utils::LoadMeshModel("C:/Users/karin/Documents/GitHub/computer-graphics-2022-shahar-and-iris/Data/pawn.obj");
	scene.AddModel(model3);
	renderer.fitInScreen(scene.GetModel(2));



	

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
		// TODO: Handle keyboard events here
		if (io.KeysDown[83])
		{
			// A key is down (S)
			ModelTransValue_array[scene.GetActiveModelIndex()][1] -= 5;
			scene.GetActiveModel().SetModelTranslate(ModelTransValue_array[scene.GetActiveModelIndex()][0], ModelTransValue_array[scene.GetActiveModelIndex()][1], ModelTransValue_array[scene.GetActiveModelIndex()][2]);
		}
		else if (io.KeysDown[87])
		{
			// A key is up (W)
			ModelTransValue_array[scene.GetActiveModelIndex()][1] += 5;
			scene.GetActiveModel().SetModelTranslate(ModelTransValue_array[scene.GetActiveModelIndex()][0], ModelTransValue_array[scene.GetActiveModelIndex()][1], ModelTransValue_array[scene.GetActiveModelIndex()][2]);
		}
		else if (io.KeysDown[65])
		{
			// A key is left (A)
			ModelTransValue_array[scene.GetActiveModelIndex()][0] -= 5;
			scene.GetActiveModel().SetModelTranslate(ModelTransValue_array[scene.GetActiveModelIndex()][0], ModelTransValue_array[scene.GetActiveModelIndex()][1], ModelTransValue_array[scene.GetActiveModelIndex()][2]);
		}
		else if (io.KeysDown[68])
		{
			// A key is right (D)
			ModelTransValue_array[scene.GetActiveModelIndex()][0] += 5;
			scene.GetActiveModel().SetModelTranslate(ModelTransValue_array[scene.GetActiveModelIndex()][0], ModelTransValue_array[scene.GetActiveModelIndex()][1], ModelTransValue_array[scene.GetActiveModelIndex()][2]);
		}
		else if (io.KeysDown[91])
		{
			// A key is [
			if (ModelScaleValue_array[scene.GetActiveModelIndex()][0] < 3000.0f) {
				ModelScaleValue_array[scene.GetActiveModelIndex()][0] += 5;
				ModelScaleValue_array[scene.GetActiveModelIndex()][1] += 5;
				ModelScaleValue_array[scene.GetActiveModelIndex()][2] += 5;
			}
			scene.GetActiveModel().SetModelScale(ModelScaleValue_array[scene.GetActiveModelIndex()][0], ModelScaleValue_array[scene.GetActiveModelIndex()][1], ModelScaleValue_array[scene.GetActiveModelIndex()][2]);
		}
		else if (io.KeysDown[93])
		{
			// A key is ]
			if (ModelScaleValue_array[scene.GetActiveModelIndex()][0] > -3000.0f - 2 * scene.GetModel(scene.GetActiveModelIndex()).GetFirstScaleValue()) {
				ModelScaleValue_array[scene.GetActiveModelIndex()][0] -= 5;
				ModelScaleValue_array[scene.GetActiveModelIndex()][1] -= 5;
				ModelScaleValue_array[scene.GetActiveModelIndex()][2] -= 5;
			}
			scene.GetActiveModel().SetModelScale(ModelScaleValue_array[scene.GetActiveModelIndex()][0], ModelScaleValue_array[scene.GetActiveModelIndex()][1], ModelScaleValue_array[scene.GetActiveModelIndex()][2]);
		}
	}

	//if (!io.WantCaptureMouse)
	//{
	//	auto mouseDelta = ImGui::GetMouseDragDelta();
	//	// Translate active model on mouse drag
	//	if (mouseDelta.x != 0 || mouseDelta.y != 0)
	//	{
	//		scene.GetActiveModel().ApplyModelTranslate(mouseDelta.x, mouseDelta.x, 0);
	//	}
	//	// Scale active model on mouse scroll
	//	if (io.MouseWheel)
	//	{
	//		scene.GetActiveModel().ApplyModelScale(io.MouseWheel, io.MouseWheel, io.MouseWheel);
	//	}
	//}

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
	// TODO: Add more controls as needed

	ImGui::End();

	static bool isModelOnScreen[5] = { false };
	static std::vector<std::string> modelNames;
	static int numberOfModels = scene.GetModelCount();

	/*static float ModelScaleValue_array[5][3] = { {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1} };
	static float ModelTransValue_array[5][3] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0} };
	static float ModelRotateValue_array[5][3] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0} };

	static float WorldScaleValue_array[5][3] = { {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1} };
	static float WorldTransValue_array[5][3] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0} };
	static float WorldRotateValue_array[5][3] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0} };*/

	//static float ModelScaleValue[3] = { 1, 1, 1 }, ModelTransValue[3] = { 0, 0, 0 }, ModelRotateValue[3] = { 0, 0, 0 };
	{
		ImGui::Begin("Model and World Transformation");
		ImGui::Text("Modify values");

		if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
		{
			for (int i = 0; i < numberOfModels; i++)
			{
				string str = "model" + std::to_string(i);
				char* model = &str[0];
				if (ImGui::BeginTabItem(model))
				{
					scene.SetActiveModelIndex(i);
					ImGui::Checkbox("Add this model to screen", &isModelOnScreen[i]);
					/*scene.PutOnScene(&isModelOnScreen[i]);*/

					/*cout << ModelScaleValue_array[i][0] << " " << ModelScaleValue_array[i][1] << " " << ModelScaleValue_array[i][2] << endl << endl;*/
					/* Set new parameters for each transformation when the slider is changed [Model] */
					if (ImGui::SliderFloat3("Model Scale", ModelScaleValue_array[i], -3000.0f - 2 * scene.GetModel(i).GetFirstScaleValue(), 3000.0f))
					{
						if (ModelScaleValue_array[i][0] == 0.0)
							ModelScaleValue_array[i][0] = 1;
						if (ModelScaleValue_array[i][1] == 0.0)
							ModelScaleValue_array[i][1] = 1;
						if (ModelScaleValue_array[i][2] == 0.0)
							ModelScaleValue_array[i][2] = 1;

						scene.GetActiveModel().SetModelScale(ModelScaleValue_array[i][0], ModelScaleValue_array[i][1], ModelScaleValue_array[i][2]);
					}
					if (ImGui::SliderFloat3("Model Translate", ModelTransValue_array[i], -1000.0f, 1000.000f))
					{
						scene.GetActiveModel().SetModelTranslate(ModelTransValue_array[i][0], ModelTransValue_array[i][1], ModelTransValue_array[i][2]);
					}
					if (ImGui::SliderFloat3("Model Rotate", ModelRotateValue_array[i], 0.0f, 360.0f))
					{
						scene.GetActiveModel().SetModelRotate(ModelRotateValue_array[i][0], ModelRotateValue_array[i][1], ModelRotateValue_array[i][2]);
					}

					/* Set new parameters for each transformation when the slider is changed [World] */
					if (ImGui::SliderFloat3("World Scale", WorldScaleValue_array[i], -3000.0f, 3000.000f))
					{
						if (WorldScaleValue_array[i][0] == 0.0)
							WorldScaleValue_array[i][0] = 1;
						if (WorldScaleValue_array[i][1] == 0.0)
							WorldScaleValue_array[i][1] = 1;
						if (WorldScaleValue_array[i][2] == 0.0)
							WorldScaleValue_array[i][2] = 1;

						scene.GetActiveModel().SetWorldScale(WorldScaleValue_array[i][0], WorldScaleValue_array[i][1], WorldScaleValue_array[i][2]);
					}
					if (ImGui::SliderFloat3("World Translate", WorldTransValue_array[i], -1000.0f, 1000.000f))
					{
						scene.GetActiveModel().SetWorldTranslate(WorldTransValue_array[i][0], WorldTransValue_array[i][1], WorldTransValue_array[i][2]);
					}
					if (ImGui::SliderFloat3("World Rotate", WorldRotateValue_array[i], 0.0f, 360.0f))
					{
						scene.GetActiveModel().SetWorldRotate(WorldRotateValue_array[i][0], WorldRotateValue_array[i][1], WorldRotateValue_array[i][2]);
					}

					ImGui::EndTabItem();
				}

			}
		}
	}
}