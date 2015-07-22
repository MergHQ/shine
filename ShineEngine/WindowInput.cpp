#include <GLFW\glfw3.h>
#include "WindowInput.h"
#include "shine.h"
#include "Input.h"

CWindowInput::CWindowInput()
{
	gSys->pInput->addListener(this);
}

CWindowInput::~CWindowInput()
{
}

bool CWindowInput::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	return true;
}