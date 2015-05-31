#include "WindowInput.h"
#include <GLFW\glfw3.h>

CWindowInput::CWindowInput()
	: IInputListener()
{

}

CWindowInput::~CWindowInput()
{

}

bool CWindowInput::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	return false;
}