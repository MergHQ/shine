#include "CameraInput.h"
#include "shine.h"
#include "Camera.h"
#include <GLFW\glfw3.h>

CCameraInput::CCameraInput()
{
}


CCameraInput::~CCameraInput()
{
}

bool CCameraInput::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	bool move =
		glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;


	gSys->GetCamera()->setMovement(
		glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS, glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS,
		glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS, glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS
	);
	return move;
}
