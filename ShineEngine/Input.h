#ifndef INPUT_H
#define INPUT_H

#pragma once

class IInputListener;
struct GLFWwindow;
#include <vector>

class CInput
{
public:
	CInput();
	~CInput();

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	int addListener(IInputListener *listener, int index);
	void removeListener(int index);
	void clearListeners();
private:
	std::vector<IInputListener*> listeners;
};

#endif