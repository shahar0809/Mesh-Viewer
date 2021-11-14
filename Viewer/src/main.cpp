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

int main(int argc, char **argv)
{
	// TODO: Need to use relative path
	const std::string path = "C:\\Users\\משתמש\\Documents\\University\\Computerized Graphics\\computer-graphics-2022-shahar-and-iris\\Data\\demo.obj";
	int windowWidth = 1280, windowHeight = 720;
	GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, "Mesh Viewer");
	if (!window)
		return 1;

	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

	Renderer renderer = Renderer(frameBufferWidth, frameBufferHeight);
	Scene scene = Scene();
	
	std::shared_ptr<MeshModel> model = Utils::LoadMeshModel(path);
	std::vector<glm::vec3> transformedVecs;

	auto result = Utils::GetMin(model->GetVertices());
	double avgX = (std::get<0>(result.second) + std::get<0>(result.first)) / 2,
		avgY = (std::get<1>(result.second) + std::get<1>(result.first)) / 2,
		avgZ = (std::get<2>(result.second) + std::get<2>(result.first)) / 2;

	double scaleVal = (windowHeight / 2) / (std::get<1>(result.second) - std::get<1>(result.first));
	double transX = (windowWidth / 2) - int(avgX - int(avgX) * scaleVal),
		transY = (windowHeight / 2) - int(avgY - int(avgY) * scaleVal);

	glm::vec3 color{ 0, 0, 0 };
	model->ApplyModelTranslate(-int(avgX), -int(avgY), -int(avgZ));
	
	model->SetModelScale(scaleVal, scaleVal, 0);
	model->ApplyModelTranslate(200, 200, 1);
	

	scene.AddModel(model);
	
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
	
	#if __APPLE__
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
		if (io.KeysDown[65])
		{
			// A key is down
			// Use the ASCII table for more key codes (https://www.asciitable.com/)
		}
	}

	if (!io.WantCaptureMouse)
	{
		auto mouseDelta = ImGui::GetMouseDragDelta();
		// Translate active model on mouse drag
		if (mouseDelta.x != 0 || mouseDelta.y != 0)
		{
			scene.GetActiveModel().ApplyModelTranslate(mouseDelta.x, mouseDelta.x, 0);
		}
		// Scale active model on mouse scroll
		if (io.MouseWheel)
		{
			scene.GetActiveModel().ApplyModelScale(io.MouseWheel, io.MouseWheel, io.MouseWheel);
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
	// TODO: Add more controls as needed
	
	ImGui::End();

	static std::vector<std::string> modelNames;

	static float ModelScaleValue[3] = { 1 }, ModelTransValue[3] = { 0 }, ModelRotateValue = 0;

	ImGui::Begin("Model");
	ImGui::Text("Modify values");

	/* Set new parameters for each transformation when the slider is changed [Model] */
	if (ImGui::SliderFloat3("Scale", ModelScaleValue, 0.0f, 1000.000f))
	{
		scene.GetActiveModel().SetModelScale(ModelScaleValue[0], ModelScaleValue[1], ModelScaleValue[2]);
	}
	if (ImGui::SliderFloat3("Translate", ModelTransValue, 0.0f, 1000.000f))
	{
		scene.GetActiveModel().SetModelTranslate(ModelTransValue[0], ModelTransValue[1], ModelTransValue[2]);
	}
	if (ImGui::SliderFloat("Rotate", &ModelRotateValue, 0.0f, 360.0f))
	{
		scene.GetActiveModel().SetModelRotate(ModelRotateValue);
	}
	ImGui::End();

	static float WorldScaleValue[3] = { 1 }, WorldTransValue[3] = { 0 }, WorldRotateValue = 0;

	ImGui::Begin("World");      
	ImGui::Text("Modify values");

	/* Set new parameters for each transformation when the slider is changed [Model] */
	if (ImGui::SliderFloat3("Scale", WorldScaleValue, 0.0f, 1000.000f))
	{
		scene.GetActiveModel().SetWorldScale(WorldScaleValue[0], WorldScaleValue[1], WorldScaleValue[2]);
	}
	if (ImGui::SliderFloat3("Translate", WorldTransValue, 0.0f, 1000.000f))
	{
		scene.GetActiveModel().SetWorldTranslate(WorldTransValue[0], WorldTransValue[1], WorldTransValue[2]);
	}
	if (ImGui::SliderFloat("Rotate", &WorldRotateValue, 0.0f, 360.0f))
	{
		scene.GetActiveModel().SetWorldRotate(WorldRotateValue);
	}

	ImGui::End();
}