#include <chrono>

#include "cereal/archives/binary.hpp"
#include "Settings.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Test.h"

#include <iostream>
#include <fstream>
#include <thread>

#include "Tests/pch.h"

GLFWwindow* window = nullptr;
std::string fileName = "../Rebuild of cphysics/testbed/settings.bin";
Settings settings;
Test* currentTest = nullptr;
static Vectors2D s_clickPointWS = Vectors2D();
bool rightMouseDown = false;
const int lastTestIndex = 15;

static void switchDemo(unsigned int i)
{
	delete currentTest;
	currentTest = nullptr;
	settings.testIndex = i;
	switch (i)
	{
	case (0): {
		currentTest = new Chains();
	} break;

	case (1): {
		currentTest = new Trebuchet();
	} break;
	case(2):
	{
		currentTest = new Friction();
	} break;
	case(3):
	{
		currentTest = new MixedShapes();
	} break;
	case(4):
	{
		currentTest = new Drag();
	} break;
	case(5):
	{
		currentTest = new LineOfSight();
	} break;
	case(6):
	{
		currentTest = new NewtonsCradle();
	} break;
	case(7):
	{
		currentTest = new ParticleExplosionTest();
	} break;
	case(8):
	{
		currentTest = new ProximityExplosionTest();
	} break;
	case(9):
	{
		currentTest = new RaycastExplosionTest();
	} break;
	case(10):
	{
		currentTest = new Raycast();
	} break;
	case(11):
	{
		currentTest = new Restitution();
	} break;
	case(12):
	{
		currentTest = new StackedObjects();
	} break;
	case(13):
	{
		currentTest = new BouncingBall();
	} break;
	case(14):
	{
		currentTest = new Wreckingball();
	} break;
	case(15):
	{
		currentTest = new SliceObjects();
	} break;
	}
}

static void glfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "GLFW error occured. Code: %d. Description: %s\n", error, description);
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	settings.window_width = width;
	settings.window_height = height;
	glViewport(0, 0, width, height);
}

static void MouseMotionCallback(GLFWwindow*, double dx, double dy)
{
	Vectors2D ps((float)dx, (float)dy);
	Vectors2D pw = camera.convertScreenToWorld(ps);

	if (rightMouseDown)
	{
		Vectors2D diff = pw - s_clickPointWS;
		camera.center.x -= diff.x;
		camera.center.y -= diff.y;
		s_clickPointWS = camera.convertScreenToWorld(ps);
	}
	else if (settings.testIndex == 5)
	{
		currentTest->getShadowcastings()[0].setStartPoint(pw);
	}
	else if (settings.testIndex == 8)
	{
		currentTest->updateProximity(pw);
	}
	else if (settings.testIndex == 9)
	{
		currentTest->updateRaycast(pw);
	}
	else if (settings.testIndex == 15 && currentTest->getSlices().size() == 1)
	{
		currentTest->getSlices()[0].setDirection(pw);
	}
}

static void MouseButtonCallback(GLFWwindow* window, signed int button, signed int action, signed int mods)
{
	ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);

	double xd, yd;
	glfwGetCursorPos(window, &xd, &yd);
	Vectors2D ps((float)xd, (float)yd);

	if (button == GLFW_MOUSE_BUTTON_1)
	{
		if (action == GLFW_PRESS)
		{
			Vectors2D clickedWorldPos = camera.convertScreenToWorld(ps);
			if (settings.testIndex == 3)
			{
				switch (coinFlip()) {
				case 0:
					currentTest->generateRandomCircle(clickedWorldPos, 0.5f, 10.0f);
					break;
				case 1:
					currentTest->generateRandomPolygon(clickedWorldPos, 0.5f, 10.0f);
					break;
				}
			}
			else if (settings.testIndex == 7)
			{
				currentTest->createParticleExplosion(clickedWorldPos);
			}
			else if (settings.testIndex == 8)
			{
				currentTest->getProximityExplosions()[0].applyBlastImpulse(5000000.0f);
			}
			else if (settings.testIndex == 9)
			{
				currentTest->getRaycastExplosions()[0].applyBlastImpulse(500000.0f);
			}

			else if (settings.testIndex == 15) {
				if (currentTest->getSlices().size() == 1) {
					currentTest->getSlices()[0].setDirection(clickedWorldPos);
					currentTest->slice(0);
					currentTest->getSlices().clear();
				}
				else {
					Slice s = Slice(clickedWorldPos, Vectors2D(1.0f, 0.0f), 0.0f);
					currentTest->getSlices().push_back(s);
				}
			}
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_2)
	{

		if (action == GLFW_PRESS)
		{
			s_clickPointWS = camera.convertScreenToWorld(ps);
			rightMouseDown = true;
		}

		if (action == GLFW_RELEASE)
		{
			rightMouseDown = false;
		}
	}
}

static void ScrollCallback(GLFWwindow* window, double dx, double dy)
{
	ImGui_ImplGlfw_ScrollCallback(window, dx, dy);
	if (ImGui::GetIO().WantCaptureMouse)
	{
		return;
	}

	if (dy > 0)
	{
		camera.zoom = max(0.9f * camera.zoom, 0.1f);
	}
	else
	{
		camera.zoom = min(1.1f * camera.zoom, 30.0f);
	}
}

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
	if (ImGui::GetIO().WantCaptureKeyboard)
	{
		return;
	}

	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;

		case GLFW_KEY_HOME:
			currentTest->resetCamera();
			break;

		case GLFW_KEY_Z:
			camera.zoom = min(1.1f * camera.zoom, 30.0f);
			break;

		case GLFW_KEY_X:
			camera.zoom = max(0.9f * camera.zoom, 1.0f);
			break;

		case GLFW_KEY_R:
			switchDemo(settings.testIndex);
			break;

		case GLFW_KEY_P:
			settings.pause = !settings.pause;
			settings.singleStep = false;
			break;
		case GLFW_KEY_TAB:
			settings.showUI = !settings.showUI;
			break;
		case GLFW_KEY_COMMA:
			if (settings.testIndex != 0)
			{
				--settings.testIndex;
			}
			switchDemo(settings.testIndex);
			break;

		case GLFW_KEY_PERIOD:
			++settings.testIndex;
			if (settings.testIndex > lastTestIndex)
			{
				settings.testIndex = lastTestIndex;
			}
			switchDemo(settings.testIndex);
			break;

		case GLFW_KEY_B:
			if (settings.testIndex == 1 && currentTest->getWorld().getJoints().size() == 3) {
				currentTest->breakJoint(2);
			}
			break;
		}
	}
}

//Loads settings via binary serialization
void loadSettings(const std::string& fileName) {
	std::ifstream os(fileName, std::ios::binary);
	{
		cereal::BinaryInputArchive ar(os);
		ar(settings);
	}
}

inline bool checkFileExists(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

//Saves settings via binary serialization
void saveSettings(const std::string& fileName) {
	std::ofstream is(fileName, std::ios::binary);
	{
		cereal::BinaryOutputArchive ar(is);
		ar(settings);
	}
}

static void initGlfw() {
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

	window = glfwCreateWindow(settings.window_width, settings.window_height, "CPhysics", NULL, NULL);

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
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSetCursorPosCallback(window, MouseMotionCallback);
	glfwSetScrollCallback(window, ScrollCallback);
}

static void initGlad() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		throw std::exception();
	}

	printf("GL %d.%d ", GLVersion.major, GLVersion.minor);
	printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
}

static void initImgui() {
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

	settings.showUI = true;
}

static void applyImguiStyle()
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

	//TO DO: fonts
}

static void updateUI() {
	if (settings.showUI)
	{
		ImGui::SetNextWindowPos(ImVec2(10.0f, 10.0f));
		ImGui::SetNextWindowSize(ImVec2(220.0f, static_cast<float>(settings.window_height) - 20.0f));
		ImGui::Begin("Controls", &settings.showUI, ImGuiWindowFlags_NoResize);

		ImGui::Text("Solver options");
		ImGui::Separator();

		ImGui::SliderInt("Solver iter", &settings.solverIterations, 1, 50);
		ImGui::SliderFloat("Hertz", &settings.hertz, 60.0f, 144.0f, "%.0f hz");
		ImGui::SliderFloat("Pos resolution", &settings.PENETRATION_CORRECTION, 0.1f, 1.0f, "%f");
		ImGui::Spacing();

		ImGui::Text("Other options");
		ImGui::Separator();

		ImGui::Checkbox("Shapes", &settings.drawShapes);
		ImGui::Checkbox("Joints", &settings.drawJoints);
		ImGui::Checkbox("AABBs", &settings.drawAABBs);
		ImGui::Checkbox("Contact points", &settings.drawContacts);
		ImGui::Checkbox("Center of masses", &settings.drawCOMs);
		ImGui::Spacing();
		ImGui::Text("Demos");
		ImGui::Separator();

		const char* testNames[] = { "Chains", "Trebuchet","Friction", "Mixed shapes", "Drag", "Line of sight", "Newtons cradle", "Particle explosion", "Proximity explosion", "Raycast explosion", "Raycast", "Restitution", "Stacked objects", "Bouncing ball", "Wrecking ball", "Slice Objects" };
		const char* current_item = testNames[settings.testIndex];

		if (ImGui::BeginCombo("##combo", current_item))
		{
			for (int n = 0; n < IM_ARRAYSIZE(testNames); n++)
			{
				bool is_selected = (current_item == testNames[n]);
				if (ImGui::Selectable(testNames[n], is_selected)) {
					current_item = testNames[n];
					switchDemo(n);
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::Spacing();
		ImGui::Separator();

		ImVec2 imguiButtons = ImVec2(-1, 0);
		if (ImGui::Button("Pause", imguiButtons))
		{
			settings.pause = !settings.pause;
			settings.singleStep = false;
		}

		if (ImGui::Button("Single Step", imguiButtons))
		{
			settings.pause = true;
			settings.singleStep = true;
		}

		if (ImGui::Button("Restart", imguiButtons))
		{
			switchDemo(settings.testIndex);
		}

		if (ImGui::Button("Quit", imguiButtons))
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		ImGui::End();
	}
}

void instructions()
{
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowSize(ImVec2(float(settings.window_width), float(settings.window_height)));
	ImGui::SetNextWindowBgAlpha(0.0f);
	ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar);
	ImGui::End();
	if (settings.pause) {
		debugDraw.renderString(240.0f, 30.0f, "-------PAUSED-------");
	}

	if (settings.showUI) {
		debugDraw.renderString(240.0f, 10.0, "tab: hide/show controls | comma: --demo | period: ++demo | p: pause | r: restart | z: zoom in | x: zoom out | home: reset camera | escape: quit");
		currentTest->drawInstructions();
	}
}

void step()
{
	instructions();

	float dt = settings.hertz > 0.0f ? 1.0f / settings.hertz : 0.0f;

	if (settings.pause)
	{
		if (settings.singleStep)
		{
			currentTest->step(dt, settings.solverIterations);
			settings.singleStep = false;
		}
	}
	else { currentTest->step(dt, settings.solverIterations); }

	currentTest->render();
}

int main()
{
	if (checkFileExists(fileName)) {
		loadSettings(fileName);
	}

	switchDemo(0);

	initGlfw();
	initGlad();
	initImgui();

	applyImguiStyle();

	debugDraw.Create();

	glClearColor(settings.background.r, settings.background.g, settings.background.b, settings.background.a);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		step();

		updateUI();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	debugDraw.Destroy();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	glfwTerminate();

	saveSettings(fileName);

	delete currentTest;
	currentTest = nullptr;

	return 0;
}
