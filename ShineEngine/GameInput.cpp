#include "GameInput.h"
#include <GLFW\glfw3.h>
#include "shine.h"
#include "Input.h"
#include "ConsoleInput.h"

CGameInput::CGameInput()
{
	gSys->pInput->addListener(this);
}


CGameInput::~CGameInput()
{
}

bool CGameInput::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_TAB && action == GLFW_RELEASE)
	{
		gSys->Log("Console active");
		return true;
	}
	
	return false;
}