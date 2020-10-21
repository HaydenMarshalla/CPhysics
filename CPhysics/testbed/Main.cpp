#include "Cereal/archives/binary.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

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
	//glViewport(0, 0, width, height);
}

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			//glfwSetWindowShouldClose(window, GL_TRUE);
			break;

		}
	}
	else if (action == GLFW_RELEASE)
	{
	}
}

static void character_callback(GLFWwindow* window, unsigned int codepoint)
{
	//ImGui_ImplGlfw_CharCallback(window, codepoint);
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

    //glClearColor(0.9645f, 0.902f, 0.84f, 1.0f);

    while (!glfwWindowShouldClose(window))
    {
      //  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

	saveSettings(fileName);

	return 0;
}
