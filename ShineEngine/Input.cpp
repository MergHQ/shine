#include "Input.h"
#include "IInputListener.h"

CInput::CInput() {}

CInput::~CInput(){}

void CInput::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	for (unsigned int i = 0; i < listeners.size(); i++)
	{
		if (listeners.at(i)->key_callback(window, key, scancode, action, mods))
			return;
	}
}

int CInput::addListener(IInputListener *listener, int index)
{
	listeners.insert(listeners.begin() + index, listener);
	// Setting up indices for listeners
	for (unsigned int i = 0; i < listeners.size(); i++)
	{
		listeners.at(i)->setIndex(i);
	}
	return listeners.size() - 1;
}

void CInput::removeListener(int index)
{
	listeners.erase(listeners.begin() + index);
}

void CInput::clearListeners()
{
	listeners.clear();
}