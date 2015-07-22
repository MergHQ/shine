#include "Input.h"
#include "IInputListener.h"
#include "shine.h"

CInput::CInput() {}

CInput::~CInput(){}

void CInput::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	for (uint i = 0; i < listeners.size(); i++)
		if (listeners[i]->key_callback(window, key, scancode, action, mods))
			return;
}

void CInput::addListener(IInputListener *listener)
{
	listeners.push_back(listener);
}

void CInput::addListener(IInputListener *listener, int index)
{
	listeners.insert(listeners.begin() + index, listener);
}

void CInput::removeListener(const IInputListener* listener)
{
	for (uint i = 0; i < listeners.size(); i++)
	{
		if (listeners[i] == listener)
		{
			listeners.erase(listeners.begin() + i);
		}
	}
}

void CInput::clearListeners()
{
	listeners.clear();
}