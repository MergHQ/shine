#ifndef CONSOLE_INPUT_H
#define CONSOLE_INPUT_H
#pragma once

#include "IInputListener.h"
#include <string>

class CConsoleInput : public IInputListener
{
public:
	CConsoleInput();
	~CConsoleInput();

	virtual bool key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) override;
protected:
private:
	bool caps = false;
	std::string text;
};
#endif