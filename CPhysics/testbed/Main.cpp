#include "Cereal/archives/binary.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "glad/glad.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Settings.h"

#include <fstream>

GLFWwindow* window = nullptr;
std::string fileName = "../CPhysics/testbed/settings.bin";
Settings settings;

unsigned int screenWidth = 1920;
unsigned int screenHeight = 1080;

static void glfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "GLFW error occured. Code: %d. Description: %s\n", error, description);
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;

		}
	}
	else if (action == GLFW_RELEASE)
	{
	}
}

static void character_callback(GLFWwindow* window, unsigned int codepoint)
{
	ImGui_ImplGlfw_CharCallback(window, codepoint);
}

static void MouseButtonCallback(GLFWwindow* window, signed int button, signed int action, signed int mods)
{
	double x, y;
	glfwGetCursorPos(window, &x, &y);
}

static void MouseMotionCallback(GLFWwindow*, double dx, double dy)
{

}

static void ScrollCallback(GLFWwindow* window, double dx, double dy)
{
}

void initGlfw() {
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		throw std::exception();
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(screenWidth, screenHeight, "CPhysics", NULL, NULL);

	if (!window)
	{
		std::cout << "Failed to open window" << std::endl;
		glfwTerminate();
		throw std::exception();
	}

	glfwMakeContextCurrent(window);

	glfwSetErrorCallback(glfwErrorCallback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCharCallback(window, character_callback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSetCursorPosCallback(window, MouseMotionCallback);
	glfwSetScrollCallback(window, ScrollCallback);
}


void initGlad() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		throw std::exception();
	}

	printf("GL %d.%d ", GLVersion.major, GLVersion.minor);
	printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
}


void initImgui() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

#if __APPLE__
	const char* glslVersion = "#version 150";
#else
	const char* glslVersion = NULL;
#endif

	bool success = ImGui_ImplGlfw_InitForOpenGL(window, false);

	if (success == false)
	{
		printf("ImGui_ImplGlfw_InitForOpenGL failed\n");
		assert(false);
	}

	success = ImGui_ImplOpenGL3_Init(glslVersion);
	if (success == false)
	{
		printf("ImGui_ImplOpenGL3_Init failed\n");
		assert(false);
	}
}

inline void applyImguiStyle()
{
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.70f, 0.4f, 0.4f, 0.55f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.83f, 0.67f, 0.67f, 0.4f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(1.0f, 0.67f, 0.67f, 0.62f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.47f, 0.22f, 0.22f, 0.67f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.47f, 0.22f, 0.22f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.47f, 0.22f, 0.22f, 0.67f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.71f, 0.39f, 0.39f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.84f, 0.66f, 0.66f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.47f, 0.22f, 0.22f, 0.65f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.71f, 0.39f, 0.39f, 0.65f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.50f);
	colors[ImGuiCol_Header] = ImVec4(0.71f, 0.39f, 0.39f, 0.54f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.84f, 0.66f, 0.66f, 0.65f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.84f, 0.66f, 0.66f, 0.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.71f, 0.39f, 0.39f, 0.54f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.71f, 0.39f, 0.39f, 0.54f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.71f, 0.39f, 0.39f, 0.54f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.84f, 0.66f, 0.66f, 0.66f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.84f, 0.66f, 0.66f, 0.66f);
	colors[ImGuiCol_Tab] = ImVec4(0.71f, 0.39f, 0.39f, 0.54f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.84f, 0.66f, 0.66f, 0.66f);
	colors[ImGuiCol_TabActive] = ImVec4(0.84f, 0.66f, 0.66f, 0.66f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);

	//TODO: fonts
}

static void updateUI() {
	if (settings.showUI)
	{
		ImGui::SetNextWindowPos(ImVec2(10.0f, 10.0f));
		ImGui::SetNextWindowSize(ImVec2(200.0f, 700.0f));
		ImGui::Begin("Settings", &settings.showUI, ImGuiWindowFlags_NoResize);

		if (ImGui::BeginTabBar("ControlTabs", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem("Settings"))
			{
				ImGui::SliderInt("Iterations", &settings.iterations, 1, 50);
				ImGui::SliderFloat("Hertz", &settings.hertz, 1.0f, 144.0f, "%.0f hz");

				ImGui::Separator();

				ImGui::Checkbox("Shapes", &settings.drawShapes);
				ImGui::Checkbox("Joints", &settings.drawJoints);
				ImGui::Checkbox("AABBs", &settings.drawAABBs);
				ImGui::Checkbox("Contact points", &settings.drawContactPoints);
				ImGui::Checkbox("Contact normals", &settings.drawContactNormals);
				ImGui::Checkbox("Contact impulses", &settings.drawContactImpulse);
				ImGui::Checkbox("Friction impulses", &settings.drawFrictionImpulse);
				ImGui::Checkbox("Center of masses", &settings.drawCOMs);

				ImGui::Separator();

				ImVec2 imguiButtons = ImVec2(-1, 0);
				if (ImGui::Button("Pause", imguiButtons))
				{
					settings.pause = !settings.pause;
				}

				if (ImGui::Button("Single Step", imguiButtons))
				{
					settings.singleStep = !settings.singleStep;
				}

				if (ImGui::Button("Restart", imguiButtons))
				{
				}

				if (ImGui::Button("Quit", imguiButtons))
				{
					glfwSetWindowShouldClose(window, GL_TRUE);
				}
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Tests"))
			{
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
	}
}

//Loads settings via serializatin
void loadSettings(const std::string& fileName) {
	std::ifstream os(fileName, std::ios::binary);
	{
		cereal::BinaryInputArchive ar(os);
		ar(settings);
	}
}

//Saves settings via serializatin
void saveSettings(const std::string& fileName) {
	std::ofstream is(fileName, std::ios::binary);
	{
		cereal::BinaryOutputArchive ar(is);
		ar(settings);
	}
}

inline bool checkFileExists(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

int main(void)
{
	if (checkFileExists(fileName)) {
		loadSettings(fileName);
	}

    initGlfw();
	initGlad();
	initImgui();

	applyImguiStyle();

	glClearColor(0.9645f, 0.902f, 0.84f, 1.0f);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//TODO: World step

		updateUI();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	glfwTerminate();

	saveSettings(fileName);

	return 0;
}
