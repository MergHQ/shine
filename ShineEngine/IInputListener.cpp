#include "IInputListener.h"

IInputListener::IInputListener()
{
}

IInputListener::~IInputListener()
{
}

bool IInputListener::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	return false;
}

void IInputListener::setIndex(int index)
{
	m_index = index;
}