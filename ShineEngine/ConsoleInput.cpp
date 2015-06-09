#include "ConsoleInput.h"
#include <GLFW\glfw3.h>
#include <Windows.h>
#include "shine.h"
#include "Input.h"
#include "ConsoleSystem.h"

CConsoleInput::CConsoleInput()
	: caps((GetKeyState(VK_CAPITAL) & 0x0001) != 0) // Uses Windows API
{
	gSys->pInput->addListener(this);
}


CConsoleInput::~CConsoleInput()
{
}

bool CConsoleInput::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_TAB && action == GLFW_RELEASE)
	{
		gSys->pInput->removeListener(this);
		gSys->Log("Console inactive");
		text = "";
	}

	if (key == GLFW_KEY_ENTER && action == GLFW_RELEASE)
	{
		const char* str = text.c_str();
		gSys->pConsoleSystem->handleCommand(str);
		gSys->Log(str);
		gSys->Log("Console inactive");
		gSys->pInput->removeListener(this);
		text = "";
	}

	if (glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_RELEASE) // CAPS functionality
	{
		caps = (GetKeyState(VK_CAPITAL) & 0x0001) != 0; // caps = !caps fails because this gets called when keys are repeated, wierd indeed...
	}
	
	char character = -1;
	if ((action == GLFW_RELEASE || action == GLFW_REPEAT) && key >= GLFW_KEY_A && key <= GLFW_KEY_Z) // Letters, capital too
	{
		if ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) != caps) // Shift key capitalizes
		{
			character = key;
		}
		else
		{
			character = key + 32;
		}
	}
	else if ((action == GLFW_RELEASE || action == GLFW_REPEAT) && key >= GLFW_KEY_0 && key <= GLFW_KEY_9) // Numbers
	{
		character = key;
	}
	else if ((action == GLFW_RELEASE || action == GLFW_REPEAT) && key == GLFW_KEY_SPACE)
	{
		character = ' ';
	}
	else if ((action == GLFW_RELEASE || action == GLFW_REPEAT) && key == GLFW_KEY_SLASH) // '-' + shift => '_'
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			character = '_';
		}
	}
	else if ((action == GLFW_RELEASE || action == GLFW_REPEAT) && key == GLFW_KEY_BACKSPACE) // Deleting text
	{
		text = text.substr(0, text.length() - 1);
	}

	if (character == -1) // Check if accepted key was pressed
		return true;

	text += character;
	
	return true;
}